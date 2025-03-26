/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         TCPScanner.cpp                                                *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    26.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the implementation of the TCPScanner       *
 *               class, which is responsible for scanning TCP ports on         *
 *               specified IP addresses. The TCPScanner class uses raw sockets *
 *               and the libnet library to send TCP SYN packets and determine  *
 *               the status of the ports based on the responses received.      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file TCPScanner.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation of the TCPScanner class for scanning TCP ports.
 */

#include "Scanning/TCPScanner.hpp"
#include "Scanning/ScanResult.hpp"
#include "Networking/InterfaceInfo.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include "Constants/ProtocolTypes.hpp"
#include "Constants/IPAddressVersion.hpp"
#include "Enums/PortStatus.hpp"
#include "Utilities/RandomNumberGenerator.hpp"
#include <string>            // std::string
#include <vector>            // std::vector
#include <chrono>            // std::chrono:milliseconds
#include <thread>            // std::this_thread::sleep_for()
#include <cstring>           // strerror()
#include <cerrno>            // errno
#include <unistd.h>          // close()
#include <fcntl.h>           // fcntl(), F_GETFL, F_SETFL, O_NONBLOCK
#include <sys/socket.h>      // socket(), AF_INET, AF_INET6, SOCK_RAW
#include <sys/select.h>      // select(), fd_set, FD_ZERO(), FD_SET(), FD_ISSET(), timeval
#include <netinet/tcp.h>     // tcphdr
#include <netinet/ip.h>      // ip
#include <netinet/ip6.h>     // ip6_hdr
#include <libnet.h>          // libnet_t, libnet_init(), libnet_*, ...

using namespace OmegaL4Scanner::Common;
using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Constants;
using namespace OmegaL4Scanner::Utilities;
using namespace OmegaL4Scanner::Enums;
using namespace std;

namespace OmegaL4Scanner::Scanning
{
    TcpScanner::TcpScanner(const vector<string> &ipaddressesToScan,
                           const InterfaceInfo &interfaceInfo,
                           const chrono::milliseconds waitTimeout)
        : PortScanner(ipaddressesToScan, interfaceInfo, waitTimeout) {}

    ScanResult TcpScanner::scanPort(const string &ipAddressToScan, const int portToScan) {
        // Determine if we are scanning an IPv6 address
        const IPAddressVersion ipAddressVersion = getIpAddressVersion(ipAddressToScan);

        // Initialize libnet context
        libnet_t *pLibnetContext = initLibnetContext(mInterfaceInfo, ipAddressVersion);

        // Create a raw socket for TCP messages
        int rawSocket{0};
        try {
            rawSocket = createRawSocket(ipAddressVersion, TCP);
        }
        catch(const exception &) {
            libnet_destroy(pLibnetContext);
            throw;
        }

        PortStatus portStatus{PortStatus::UNKNOWN};
        try {
            // Set the raw socket to non-blocking mode
            useNonBlockingMode(rawSocket);

            // Generate random ephemeral source port and TCP sequence number
            // Note: Retransmission from the same port for better results
            const uint16_t sourcePort = RandomNumberGenerator::getEphemeralPort16();
            const uint32_t sequenceNumber = RandomNumberGenerator::getSequenceNumber32();

            // Set retransmission parameters
            int attemptCounter = 0;
            const long int waitTimeoutMs = millisecondsToLongInt(mWaitTimeout);

            while(attemptCounter < MAX_TRANSMIT_ATTEMPTS) {
                // Clear any previous packet from the libnet context
                libnet_clear_packet(pLibnetContext);

                // Build the TCP header (SYN)
                createTcpHeader(pLibnetContext, sourcePort, portToScan, sequenceNumber);

                // Build the IP header based on the IP version.
                createIpHeader(pLibnetContext, ipAddressVersion, mInterfaceInfo, ipAddressToScan, TCP);

                // Send the TCP SYN packet.
                const int bytesWritten = libnet_write(pLibnetContext);

                // libnet_write() returns -1 on error
                if(bytesWritten < 0) {
                    throw LibnetErrorException(
                            "libnet_write() error: " + string(libnet_geterror(pLibnetContext))
                            );
                }

                // Wait for a TCP response (SYN/ACK or RST)
                portStatus = checkRawResponse(rawSocket, waitTimeoutMs, sourcePort,
                                              portToScan, ipAddressVersion, TCP);
                if(portStatus != PortStatus::FILTERED) {
                    break;
                }

                // If no response is received, assume the port is filtered a
                attemptCounter++;
                this_thread::sleep_for(chrono::milliseconds(100));
            }
        }
        catch(const exception &) {
            close(rawSocket);
            libnet_destroy(pLibnetContext);
            throw;
        }

        // Clean up libnet context and raw socket
        close(rawSocket);
        libnet_destroy(pLibnetContext);

        return ScanResult(ipAddressToScan, portToScan, TCP, portStatus);
    } // TcpScanner::scanPort()

    PortStatus TcpScanner::determinePortStatus(const uint8_t *pBuffer, const ssize_t bytesReceived,
                                               const int destinationPort, const uint16_t sourcePort,
                                               const IPAddressVersion ipAddressVersion) {
        // prepare variable for IP header length
        size_t ipHeaderLength{0};

        // For IPv4 - we assume no IPv4 header truncation
        if(ipAddressVersion == IPv4) {
            // If the received bytes are less than the minimum required for 'IPv4 + TCP header' => port FILTERED
            if(static_cast<size_t>(bytesReceived) < (sizeof(ip) + sizeof(tcphdr))) {
                return PortStatus::FILTERED;
            }

            // Interpret the buffer as an IPv4 header
            const auto pIpv4Header = reinterpret_cast<const ip*>(pBuffer);

            // Check if the IP version is 4 as expected
            if(pIpv4Header->ip_v != 4) {
                return PortStatus::FILTERED; // Invalid IP version, assume port is filtered
            }

            // The 'ip_hl' field represents the header length in 32-bit words,
            // so we multiply by 4 to get the length in bytes.
            ipHeaderLength = pIpv4Header->ip_hl * 4;
        }
        // For IPv6 - IPv6 header truncation is possible and accounted for
        else {
            // We expect IPv6 header (40B) + TCP header (20B) = 60B, but if we receive less...
            if(static_cast<size_t>(bytesReceived) < (sizeof(ip6_hdr) + sizeof(tcphdr))) {
                // ...if it is 20B (TCP header), we assume the IPv6 header was truncated (e.g. localhost).
                if(static_cast<size_t>(bytesReceived) >= sizeof(tcphdr)) {
                    ipHeaderLength = 0;
                }
                // ...otherwise, we assume the port is FILTERED.
                else {
                    return PortStatus::FILTERED;
                }
            }
            else {
                ipHeaderLength = sizeof(ip6_hdr);
            }
        }

        // Interpret the buffer as a TCP header
        const auto tcpHeader = reinterpret_cast<const tcphdr*>(pBuffer + ipHeaderLength);

        // Get the source and destination port from the TCP header
        const uint16_t sourcePortResponse = ntohs(tcpHeader->th_sport);
        const uint16_t destinationPortResponse = ntohs(tcpHeader->th_dport);

        // Check if the ports match the expected ports => port FILTERED
        if(sourcePort != destinationPortResponse ||
            sourcePortResponse != destinationPort) {
            return PortStatus::FILTERED;
        }

        // Check which flags are set in the TCP header
        const bool rstFlag = (tcpHeader->th_flags & TH_RST);
        const bool synFlag = (tcpHeader->th_flags & TH_SYN);
        const bool ackFlag = (tcpHeader->th_flags & TH_ACK);

        // Determine the port status based on the TCP flags
        if(rstFlag) {
            return PortStatus::CLOSED; // the RST flag is set => port CLOSED
        }

        if(synFlag && ackFlag) {
            return PortStatus::OPEN; // both the SYN and ACK flags are set => port OPEN
        }

        return PortStatus::FILTERED; // none of the above conditions are met => port FILTERED
    } // TcpScanner::determinePortStatus()

    void TcpScanner::createTcpHeader(libnet_t *pLibnetContext, const uint16_t sourcePort,
                                     const int destinationPort, const uint32_t sequenceNumber) {
        // Build the TCP header using libnet
        const libnet_ptag_t tcpTag = libnet_build_tcp(
                sourcePort,         // Source port
                destinationPort,    // Destination port
                sequenceNumber,     // Sequence number
                0,                  // Acknowledgment number
                TH_SYN,             // TCP flags: SYN
                65535,              // Window size
                0,                  // Checksum (auto)
                0,                  // Urgent pointer
                LIBNET_TCP_H,       // TCP header length
                nullptr,            // Payload
                0,                  // Payload length
                pLibnetContext,     // Libnet context
                0                   // Protocol tag (0 to build new)
                );
        // libnet_build_udp() returns -1 on error
        if(tcpTag < 0) {
            throw LibnetErrorException(
                    "libnet_build_udp() error for: " + string(libnet_geterror(pLibnetContext))
                    );
        }
    } // TcpScanner::createTcpHeader()
} // OmegaL4Scanner::Scanning
