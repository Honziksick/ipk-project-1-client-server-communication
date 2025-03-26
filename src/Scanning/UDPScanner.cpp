/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         UDPScanner.cpp                                                *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    26.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the UdpScanner class, which uses the        *
 *               libnet library to construct and send a UDP packet. If an      *
 *               ICMP response is received (IPv4: type 3, code 3;              *
 *               IPv6: type 1, code 4), the port is considered closed;         *
 *               otherwise, it is considered open.                             *
 *                                                                             *
 ******************************************************************************/
/**
 * @file UDPScanner.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the UDPScanner class.
 */

#include "Scanning/UDPScanner.hpp"
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
#include <cerrno>            // errno
#include <cstring>           // strerror()
#include <unistd.h>          // close()
#include <sys/socket.h>      // socket(), AF_INET, AF_INET6, SOCK_RAW
#include <sys/select.h>      // select(), fd_set, FD_ZERO(), FD_SET(), FD_ISSET(), timeval
#include <netinet/udp.h>     // udphdr
#include <netinet/ip.h>      // ip
#include <netinet/ip6.h>     // ip6_hdr
#include <netinet/ip_icmp.h> // icmp
#include <netinet/icmp6.h>   // icmp6_hdr
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
    UdpScanner::UdpScanner(const vector<string> &ipAddressesToScan,
                           const InterfaceInfo &interfaceInfo,
                           const chrono::milliseconds waitTimeout)
        : PortScanner(ipAddressesToScan, interfaceInfo, waitTimeout) {}

    ScanResult UdpScanner::scanPort(const string &ipAddressToScan, const int portToScan) {
        // Determine if we are scanning an IPv4/IPv6 address
        const IPAddressVersion ipAddressVersion = getIpAddressVersion(ipAddressToScan);

        // Initialize libnet context
        libnet_t *pLibnetContext = initLibnetContext(mInterfaceInfo, ipAddressVersion);

        // Create a raw socket for ICMP messages
        int rawSocket{0};
        try {
            rawSocket = createRawSocket(ipAddressVersion, UDP);
        }
        catch(const exception &) {
            libnet_destroy(pLibnetContext);
            throw;
        }

        PortStatus portStatus{PortStatus::UNKNOWN};
        try {
            // Set the raw socket to non-blocking mode
            useNonBlockingMode(rawSocket);

            // Generate a source port
            const uint16_t sourcePort = RandomNumberGenerator::getEphemeralPort16();

            // Clear any previous packet from the libnet context
            libnet_clear_packet(pLibnetContext);

            // Build the UDP header
            createUdpHeader(pLibnetContext, ipAddressVersion, sourcePort, portToScan);

            // Build the IP header based on the IP version
            createIpHeader(pLibnetContext, ipAddressVersion, mInterfaceInfo, ipAddressToScan, UDP);

            // Send the UDP packet
            const int bytesWritten = libnet_write(pLibnetContext);

            // libnet_write() returns -1 on error
            if(bytesWritten < 0) {
                throw LibnetErrorException(
                        "libnet_write() error: " + string(libnet_geterror(pLibnetContext))
                        );
            }
            // Waiting for ICMP response
            const long int waitTimeoutMs = millisecondsToLongInt(mWaitTimeout);
            portStatus = checkRawResponse(rawSocket, waitTimeoutMs, sourcePort,
                                          portToScan, ipAddressVersion, UDP);
        }
        catch(const exception &) {
            close(rawSocket);
            libnet_destroy(pLibnetContext);
            throw;
        }

        // Clean up libnet context and raw socket
        close(rawSocket);
        libnet_destroy(pLibnetContext);

        return
                ScanResult(ipAddressToScan, portToScan, UDP, portStatus);
    } // UdpScanner::scanPort()

    PortStatus UdpScanner::determinePortStatus(const uint8_t *pBuffer, const ssize_t bytesReceived,
                                               const int destinationPort, const uint16_t sourcePort,
                                               const IPAddressVersion ipAddressVersion) {
        // For IPv4
        if(ipAddressVersion == IPv4) {
            // If the received bytes are less than the minimum required for 'IPv4 + ICMP + UDP header' => port OPEN
            if((static_cast<size_t>(bytesReceived)) < (sizeof(ip) + sizeof(icmp) + sizeof(udphdr))) {
                return PortStatus::OPEN;
            }

            // Interpret the buffer as an IPv4 header
            const auto pIpv4Header = reinterpret_cast<const ip*>(pBuffer);

            // Check if the IP version is 4 as expected
            if(pIpv4Header->ip_v != 4) {
                return PortStatus::OPEN; // Invalid IP version, assume port is open
            }

            // The 'ip_hl' field represents the header length in 32-bit words,
            // so we multiply by 4 to get the length in bytes.
            const int ipv4HeaderLength = pIpv4Header->ip_hl * 4;

            // If the received bytes are less than the total length of 'IPv4 header length + ICMP + UDP header' => port OPEN
            if((static_cast<size_t>(bytesReceived)) < (static_cast<size_t>(ipv4HeaderLength) + sizeof(icmp) + sizeof(udphdr))) {
                return PortStatus::OPEN;
            }

            // Interpret the buffer as an ICMP header
            const auto icmpHeader = reinterpret_cast<const icmp*>(pBuffer + ipv4HeaderLength);

            // Check if the ICMP type is 3 (Destination Unreachable) and code is 3 (Port Unreachable)
            if(icmpHeader->icmp_type == 3 && icmpHeader->icmp_code == 3) {
                const uint8_t *pOriginalUdp = pBuffer + ipv4HeaderLength + sizeof(icmp);
                const auto originalUdpHeader = reinterpret_cast<const udphdr*>(pOriginalUdp);
                const uint16_t originalDestinationPort = ntohs(originalUdpHeader->uh_dport);
                const uint16_t originalSourcePort = ntohs(originalUdpHeader->uh_sport);

                // If the original destination and source ports match the given ports => port CLOSED
                if(originalDestinationPort == static_cast<uint16_t>(destinationPort) &&
                    originalSourcePort == sourcePort) {
                    return PortStatus::CLOSED;
                }
            }
        }
        // For IPv6
        else {
            // If the received bytes are less than the minimum required for 'IPv6 + ICMPv6 + UDP header' => port OPEN
            if(static_cast<size_t>(bytesReceived) < sizeof(ip6_hdr) + sizeof(icmp6_hdr) + sizeof(udphdr)) {
                return PortStatus::OPEN;
            }

            // Interpret the buffer as an IPv6 header
            const auto pIpv6Header = reinterpret_cast<const ip6_hdr*>(pBuffer);

            // Check if the IP version is 6 as expected
            if((pIpv6Header->ip6_vfc >> 4) != 6) {
                return PortStatus::OPEN; // Invalid IP version, assume port is open
            }

            // If the received bytes are less than the total length of 'IPv6 header length + ICMPv6 + UDP header' => port OPEN
            if(static_cast<size_t>(bytesReceived) < (sizeof(ip6_hdr) + sizeof(icmp6_hdr) + sizeof(udphdr))) {
                return PortStatus::OPEN;
            }

            // Interpret the buffer as an ICMPv6 header
            const auto icmpv6Header = reinterpret_cast<const icmp6_hdr*>(pBuffer + sizeof(ip6_hdr));

            // Check if the ICMPv6 type is 1 (Destination Unreachable) and code is 4 (Port Unreachable)
            if(icmpv6Header->icmp6_type == 1 && icmpv6Header->icmp6_code == 4) {
                const uint8_t *originalUdpPtr = pBuffer + sizeof(ip6_hdr) + sizeof(icmp6_hdr);
                const auto originalUdpHeader = reinterpret_cast<const udphdr*>(originalUdpPtr);
                const uint16_t originalDestinationPort = ntohs(originalUdpHeader->uh_dport);
                const uint16_t originalSourcePort = ntohs(originalUdpHeader->uh_sport);

                // If the original destination and source ports match the given ports => port CLOSED
                if(originalDestinationPort == static_cast<uint16_t>(destinationPort) &&
                    originalSourcePort == sourcePort) {
                    return PortStatus::CLOSED;
                }
            }
        }
        return PortStatus::OPEN;
    } // UdpScanner::determinePortStatus()

    void UdpScanner::createUdpHeader(libnet_t *pLibnetContext, const IPAddressVersion ipAddressVersion,
                                     const uint16_t sourcePort, const int destinationPort) {
        // Build the UDP header using libnet
        const libnet_ptag_t udpTag = libnet_build_udp(
                sourcePort,       // source port
                destinationPort,  // destination port
                LIBNET_UDP_H,     // length of UDP header
                0,                // checksum (auto)
                nullptr,          // payload
                0,                // length of payload
                pLibnetContext,   // libnet context
                0                 // protocol tag (0 for a new protocol)
                );
        // libnet_build_udp() returns -1 on error
        if(udpTag < 0) {
            const string ipAddressVersionStr = (ipAddressVersion == IPv4) ? "IPv4" : "IPv6";
            throw LibnetErrorException(
                    "libnet_build_udp() error for" + ipAddressVersionStr +
                    ": " + string(libnet_geterror(pLibnetContext))
                    );
        }
    } // UdpScanner::createUdpHeader()
} // OmegaL4Scanner::Scanning
