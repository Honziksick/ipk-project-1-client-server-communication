/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         PortScanner.cpp                                               *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    26.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the PortScanner class, which is             *
 *               responsible for scanning network ports.                       *
 *                                                                             *
 ******************************************************************************/
/**
 * @file PortScanner.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the PortScanner class.
 */

#include "Scanning/PortScanner.hpp"
#include "Scanning/ScanResult.hpp"
#include "Networking/InterfaceInfo.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include "Common/OmegaDataTypes.hpp"
#include "Enums/PortStatus.hpp"
#include "Constants/ProtocolTypes.hpp"
#include "Constants/IPAddressVersion.hpp"
#include "Constants/ColorEscapeSequences.hpp"
#include "Utilities/RandomNumberGenerator.hpp"
#include <string>     // std::string
#include <vector>     // std::vector
#include <array>      // std::array
#include <chrono>     // std::chrono::milliseconds
#include <algorithm>  // std::clamp
#include <limits>     // std::numeric_limits
#include <iostream>   // std::cout, std::cerr, std::endl
#include <cstring>    // strerror()
#include <fcntl.h>    // fcntl(), F_GETFL, F_SETFL, O_NONBLOCK
#include <libnet.h>   // libnet_t, libnet_in6_addr

using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Common;
using namespace OmegaL4Scanner::Enums;
using namespace OmegaL4Scanner::Constants;
using namespace OmegaL4Scanner::Utilities;
using namespace std;

namespace OmegaL4Scanner::Scanning
{
    PortScanner::PortScanner(const vector<string> &ipaddressesToScan,
                             const InterfaceInfo &interfaceInfo,
                             const chrono::milliseconds waitTimeout)
        : mIpaddressesToScan{ipaddressesToScan}, mInterfaceInfo{interfaceInfo}, mWaitTimeout{waitTimeout} {}

    void PortScanner::scanPorts(const vector<PortRange> &portRanges) {
        for(const auto &portVariant : portRanges) {
            int rangeStart{0};
            int rangeEnd{0};

            if(holds_alternative<int>(portVariant)) {
                rangeStart = rangeEnd = get<int>(portVariant);
            }
            else {
                auto [start, end] = get<pair<int, int>>(portVariant);
                rangeStart = start;
                rangeEnd = end;
            }

            for(const auto &ipAddress : mIpaddressesToScan) {
                for(int iPort = rangeStart; iPort <= rangeEnd; iPort++) {
                    cout << scanPort(ipAddress, iPort).scanResultToString() << endl;
                }
            }
        }
    } // PortScanner::scanPorts()

    long int PortScanner::millisecondsToLongInt(const chrono::milliseconds waitTimeout) {
        // Clamps long long waitTimeout to an long integer (truncate overflow)
        const auto clampedValue = clamp(
                waitTimeout.count(),
                static_cast<int64_t>(numeric_limits<long int>::min()),
                static_cast<int64_t>(numeric_limits<long int>::max())
                );

        // Check if overflow occurred
        if(clampedValue != waitTimeout.count()) {
            cerr << COLOR_MAGENTA << "Warning: Timeout " << waitTimeout.count() <<
                    " ms is out of 'long int' range and was clamped to " <<
                    clampedValue << " ms." << RESET << endl;
        }

        // Return the clamped value as an long integer
        return static_cast<long int>(clampedValue);
    } // PortScanner::millisecondsToLongInt()

    string PortScanner::getSourceAddress(const InterfaceInfo &interfaceInfo, const IPAddressVersion ipAddressType) {
        // Select the first IPv4/IPv6 address from the interface
        for(const auto &ipAddress : interfaceInfo.mIpAddresses) {
            if(ipAddressType == IPv4) {
                if(ipAddress.mIpAddress.find(':') == string::npos) {
                    return ipAddress.mIpAddress;
                }
            }
            else {
                if(ipAddress.mIpAddress.find(':') != string::npos) {
                    return ipAddress.mIpAddress;
                }
            }
        }
        // Throw an exception if no matching IP address is found
        const string ipVersionStr = (ipAddressType == IPv4) ? "IPv4" : "IPv6";
        throw InternalErrorException("Interface does not contain any " + ipVersionStr + " address.");
    } // PortScanner::getSourceAddress()

    IPAddressVersion PortScanner::getIpAddressVersion(const string &ipAddress) {
        return (ipAddress.find(':') != string::npos) ? IPv6 : IPv4;
    } // PortScanner::getIpAddressType()

    void PortScanner::useNonBlockingMode(const int socket) {
        // Get current socket flags
        const int currentFlags = fcntl(socket, F_GETFL, 0);
        if(currentFlags < 0) {
            throw SocketErrorException(
                    "Failed to get socket flags: " + string(strerror(errno))
                    );
        }
        // Set socket to non-blocking mode
        const int result = fcntl(socket, F_SETFL, currentFlags | O_NONBLOCK);
        if(result < 0) {
            throw SocketErrorException(
                    "Failed to set socket to non-blocking mode: " + string(strerror(errno))
                    );
        }
    } // PortScanner::useNonBlockingMode()

    libnet_t *PortScanner::initLibnetContext(const InterfaceInfo &interfaceInfo, const IPAddressVersion ipAddressVersion) {
        // Set the injection type based on IP version
        const int injectionType = (ipAddressVersion == IPv4) ? LIBNET_RAW4 : LIBNET_RAW6;

        // Buffer for libnet error messages
        char libnetErrorMsgBuffer[LIBNET_ERRBUF_SIZE]{};

        // Initialize libnet context for IPv4/IPv6 (IPv4/IPv6 raw socket interface)
        libnet_t *pLibnetContext = libnet_init(injectionType, interfaceInfo.mName.c_str(), libnetErrorMsgBuffer);

        // Check if libnet context was created successfully
        if(!pLibnetContext) {
            const string ipAddressVersionStr = (ipAddressVersion == IPv4) ? "IPv4" : "IPv6";
            throw LibnetErrorException(
                    "libnet_init() error for " + ipAddressVersionStr +
                    ": " + string(libnetErrorMsgBuffer)
                    );
        }
        return pLibnetContext;
    } // PortScanner::initLibnetContext()

    int PortScanner::createRawSocket(const IPAddressVersion ipAddressVersion, const ProtocolType &protocolType) {
        // Determine the socket domain based on IP version
        const int domain = (ipAddressVersion == IPv4) ? AF_INET : AF_INET6;

        // Determine the socket protocol based on IP version and protocol type
        int protocol{0};
        if(protocolType == UDP) {
            protocol = (ipAddressVersion == IPv4)
                           ? static_cast<int>(IPPROTO_ICMP)
                           : static_cast<int>(IPPROTO_ICMPV6);
        }
        else if(protocolType == TCP) {
            protocol = IPPROTO_TCP;
        }
        else {
            throw InternalErrorException(
                    "Trying to create raw socket with invalid protocol type: " + protocolType
                    );
        }
        // Create a raw socket for ICMP/ICMPv6 (IPv4/IPv6 version)
        const int rawSocket = socket(domain, SOCK_RAW, protocol);

        // socket() returns -1 on error
        if(rawSocket < 0) {
            const string ipAddressVersionStr = (ipAddressVersion == IPv4) ? "IPv4" : "IPv6";
            string protocolStr;
            if(protocolType == UDP) {
                protocolStr = (ipAddressVersion == IPv4) ? "ICMP" : "ICMPv6";
            }
            else {
                protocolStr = "TCP";
            }
            throw SocketErrorException(
                    "socket() error: Failed to create raw socket for " +
                    protocolStr + "messages (" + ipAddressVersionStr + "): " +
                    string(strerror(errno)));
        }
        return rawSocket;
    } // PortScanner::createRawSocket()

    void PortScanner::createIpv4Header(libnet_t *pLibnetContext, const uint32_t sourceIpv4,
                                       const uint32_t destinationIpv4, const ProtocolType &protocolType) {
        // Determine the protocol and header type for the IPv4 header
        const uint8_t protocol = (protocolType == UDP) ? IPPROTO_UDP : IPPROTO_TCP;
        const uint16_t header = (protocolType == UDP) ? LIBNET_UDP_H : LIBNET_TCP_H;

        // Calculate the total length of the IP packet
        const uint16_t totalLength = LIBNET_IPV4_H + header;

        // Build the IPv4 header using libnet
        const libnet_ptag_t ipv4Tag = libnet_build_ipv4(
                totalLength,                                 // Total length of the IP packet
                0,                                           // Type of service (TOS)
                RandomNumberGenerator::getEphemeralPort16(), // IP ID (randomly generated)
                0,                                           // Fragmentation bits and offset
                64,                                          // Time to live (TTL)
                protocol,                                    // Protocol (UDP/TCP)
                0,                                           // Checksum (auto-calculated)
                sourceIpv4,                                  // Source IP address
                destinationIpv4,                             // Destination IP address
                nullptr,                                     // Payload (none)
                0,                                           // Length of payload
                pLibnetContext,                              // Libnet context
                0                                            // Protocol tag (0 for a new protocol)
                );
        // libnet_build_ipv4() returns -1 on error
        if(ipv4Tag < 0) {
            throw LibnetErrorException(
                    "libnet_build_ipv4() failed: " + string(libnet_geterror(pLibnetContext))
                    );
        }
    } // PortScanner::createIpv4Header()

    void PortScanner::createIpv6Header(libnet_t *pLibnetContext, const libnet_in6_addr sourceIpv6,
                                       const libnet_in6_addr destinationIpv6, const ProtocolType &protocolType) {
        // Determine the protocol and header type for the IPv6 header
        const uint8_t protocol = (protocolType == UDP) ? IPPROTO_UDP : IPPROTO_TCP;
        const uint16_t header = (protocolType == UDP) ? LIBNET_UDP_H : LIBNET_TCP_H;

        // Build the IPv6 header using libnet
        const libnet_ptag_t ipv6Tag = libnet_build_ipv6(
                0,                // Traffic class
                0,                // Flow label
                header,           // Total length of the IP packet (length of the UDP/TCP header)
                protocol,         // Next header (UDP/TCP)
                64,               // Hop limit (TTL)
                sourceIpv6,       // Source IPv6 address
                destinationIpv6,  // Destination IPv6 address
                nullptr,          // Payload (none)
                0,                // Length of payload (0)
                pLibnetContext,   // Libnet context
                0                 // Protocol tag (0 for a new protocol)
                );
        // libnet_build_ipv6() returns -1 on error
        if(ipv6Tag < 0) {
            throw LibnetErrorException(
                    "libnet_build_ipv6() failed: " + string(libnet_geterror(pLibnetContext))
                    );
        }
    } // PortScanner::createIpv6Header()

    void PortScanner::createIpHeader(libnet_t *pLibnetContext, const IPAddressVersion ipAddressVersion,
                                     const InterfaceInfo &interfaceInfo, const string &ipAddressToScan,
                                     const ProtocolType &protocolType) {
        // Check if the protocol type is valid (UDP or TCP)
        if(protocolType != UDP && protocolType != TCP) {
            throw InternalErrorException(
                    "Trying to create IP header with invalid protocol type: " + protocolType
                    );
        }
        // Create the IPv4 header
        if(ipAddressVersion == IPv4) {
            // Get the source address for IPv4
            const string sourceIPv4Str = getSourceAddress(interfaceInfo, IPv4);

            // Convert the source IP address to binary form (returns -1 on error)
            const uint32_t sourceIpv4 = libnet_name2addr4(pLibnetContext,
                                                          const_cast<char*>(sourceIPv4Str.c_str()),
                                                          LIBNET_DONT_RESOLVE);
            if(sourceIpv4 == static_cast<uint32_t>(-1)) {
                throw LibnetErrorException(
                        "libnet_name2addr4() error: Invalid source IPv4 address: " + sourceIPv4Str
                        );
            }
            // Convert the destination IP address to binary form (returns -1 on error)
            const uint32_t destinationIpv4 = libnet_name2addr4(pLibnetContext,
                                                               const_cast<char*>(ipAddressToScan.c_str()),
                                                               LIBNET_DONT_RESOLVE);
            if(destinationIpv4 == static_cast<uint32_t>(-1)) {
                throw LibnetErrorException(
                        "libnet_name2addr4() error: Invalid destination IPv4 address: " + ipAddressToScan
                        );
            }

            // Create the IPv4 header using libnet (validity check)
            createIpv4Header(pLibnetContext, sourceIpv4, destinationIpv4, protocolType);
        }
        // Create the IPv6 header
        else {
            // Get the source address for IPv6
            const string sourceIPv6Str = getSourceAddress(interfaceInfo, IPv6);

            // Convert the source IP addresses to binary form (returns -1 on error)
            const libnet_in6_addr sourceIpv6 = libnet_name2addr6(pLibnetContext, sourceIPv6Str.c_str(),
                                                                 LIBNET_DONT_RESOLVE);
            if(IN6_IS_ADDR_UNSPECIFIED(&sourceIpv6)) {
                throw LibnetErrorException(""
                        "libnet_name2addr6() error: Invalid source IPv6 address: " + sourceIPv6Str
                        );
            }
            // Convert the destination IP addresses to binary form (returns -1 on error)
            const libnet_in6_addr destinationIpv6 = libnet_name2addr6(pLibnetContext, ipAddressToScan.c_str(),
                                                                      LIBNET_DONT_RESOLVE);
            if(IN6_IS_ADDR_UNSPECIFIED(&destinationIpv6)) {
                throw LibnetErrorException(""
                        "libnet_name2addr6() error: Invalid destination IPv6 address: " + ipAddressToScan
                        );
            }

            // Create the IPv6 header using libnet (validity check)
            createIpv6Header(pLibnetContext, sourceIpv6, destinationIpv6, protocolType);
        }
    } // PortScanner::createIpHeader()

    void PortScanner::initializeFdSet(fd_set &readfds, const int rawSocket) {
        FD_ZERO(&readfds);
        FD_SET(rawSocket, &readfds);
    } // PortScanner::initializeFdSet()

    void PortScanner::setTimeval(timeval &tv, const int waitTimeoutMilliseconds) {
        tv.tv_sec = waitTimeoutMilliseconds / 1000;
        tv.tv_usec = (waitTimeoutMilliseconds % 1000) * 1000;
    } // PortScanner::setTimeval()

    int PortScanner::selectSocket(const int rawSocket, fd_set &fileDescriptorSet, timeval &timeout) {
        // Wait for an ICMP packet: 'rawSocket + 1' is used in select() because
        //                          the first argument must be 1 greater than
        //                          the highest file descriptor in the set.
        return select(rawSocket + 1, &fileDescriptorSet, nullptr, nullptr, &timeout);
    } // PortScanner::selectSocket()

    PortStatus PortScanner::checkRawResponse(const int rawSocket, const long int waitTimeoutMilliseconds,
                                             const uint16_t sourcePort, const int destinationPort,
                                             const IPAddressVersion ipAddressVersion,
                                             const ProtocolType &protocolType) {
        // Initialize file descriptor set for select()
        fd_set fileDescriptorSet{};
        initializeFdSet(fileDescriptorSet, rawSocket);

        // Set up the timeout for select()
        timeval timeout{};
        setTimeval(timeout, waitTimeoutMilliseconds);

        // Wait for packet
        const int fileDescriptorCount = selectSocket(rawSocket, fileDescriptorSet, timeout);

        // Check if select() failed
        if(fileDescriptorCount < 0) {
            throw SocketErrorException("select() error: " + string(strerror(errno)));
        }

        // Check if select() timed-out or no packet was received
        if(fileDescriptorCount == 0 || !FD_ISSET(rawSocket, &fileDescriptorSet)) {
            return (protocolType == UDP) ? PortStatus::OPEN : PortStatus::FILTERED;
        }

        // Prepare buffer for the received packet (maximum size of 256 bytes + 1)
        array<uint8_t, numeric_limits<uint8_t>::max() + 1> buffer{};

        // Receive the packet
        const ssize_t bytesReceived = recv(rawSocket, buffer.data(), buffer.size(), 0);

        // recv() returns -1 on error
        if(bytesReceived < 0) {
            throw SocketErrorException("recv() error: " + string(strerror(errno)));
        }

        // Determine the port status based on the received packet
        return determinePortStatus(buffer.data(), bytesReceived, destinationPort, sourcePort, ipAddressVersion);
    } // PortScanner::checkRawResponse()
} // OmegaL4Scanner::Scanning

/*** end of file PortScanner.cpp ***/
