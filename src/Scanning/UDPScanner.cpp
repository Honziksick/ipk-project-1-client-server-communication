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
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the UDPScanner class, which is              *
 *               responsible for scanning UDP network ports.                   *
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
#include "Enums/PortStatus.hpp"
#include <string>       // std::string, std::to_string()
#include <chrono>       // std::chrono::milliseconds
#include <thread>       // std::this_thread::sleep_for()
#include <iostream>     // std::cout, std::endl
#include <arpa/inet.h>  // inet_pton()
#include <sys/socket.h> // socket(), sendto(), close()
#include <netinet/in.h> // sockaddr_in, sockaddr_in6
#include <libnet.h>     // libnet_init()
#include <pcap.h>       // pcap_open_live(), pcap_compile(), pcap_setfilter(), pcap_next_ex()

using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Constants;
using namespace OmegaL4Scanner::Enums;
using namespace std;

namespace OmegaL4Scanner::Scanning
{
    UdpScanner::UdpScanner(const vector<string> &ipaddressesToScan,
                           const InterfaceInfo &interfaceInfo,
                           const chrono::milliseconds waitTimeout)
        : PortScanner(ipaddressesToScan, interfaceInfo, waitTimeout) {}

    // Study materials:
    //   - https://www.geeksforgeeks.org/udp-server-client-implementation-c/
    //   - man7.org/linux/man-pages
    //   - IPK Lecture 4: Programming Network Applications
    ScanResult UdpScanner::scanPort(const string &ipAddressToScan, const int portToScan) {
        bool scanningIPv6 = ipAddressToScan.find(':') != string::npos;

        // Capturing ICMP response (type 3, code 3) with LibPcap
        //   - 65535: Recommended maximum number of bytes to capture from each packet.
        //   - 1: Set the interface in 'promiscuous mode' (0 to disable).
        char pcapErrorMsgBuffer[PCAP_ERRBUF_SIZE];
        const int waitTimeoutInt = millisecondsToInt(mWaitTimeout);
        pcap_t *pHandle = pcap_open_live(mInterfaceInfo.mName.c_str(), 65535, 1,
                                         waitTimeoutInt, pcapErrorMsgBuffer);

        // Check if pcap_open_live() was successful
        if(pHandle == nullptr) {
            throw PcapErrorException(string("pcap_open_live() error: ") + pcapErrorMsgBuffer);
        }

        // Set the pcap handle to non-blocking mode
        if(pcap_setnonblock(pHandle, 1, pcapErrorMsgBuffer) == PCAP_ERROR) {
            const string pcapErrorMsg = pcap_geterr(pHandle);
            pcap_close(pHandle);
            throw PcapErrorException(string("pcap_setnonblock() error: ") + pcapErrorMsg);
        }

        // Get ICMP response of type=3 and code=3 (filter in BPF syntax)
        //   - IPv4: "icmp and src host X and icmp[0] == 3 and icmp[1] == 3"
        //   - IPv6: "icmp6 and src host X and icmp6[0] == 1 and icmp6[1] == 4"
        // See: https://biot.com/capstats/bpf.html
        string filter;
        if(scanningIPv6) {
            filter = "icmp6 and src host " + ipAddressToScan + " and icmp6[0] == 1 and icmp6[1] == 4";
        }
        else {
            filter = "icmp and src host " + ipAddressToScan + " and icmp[0] == 3 and icmp[1] == 3";
        }

        // Compile the filter
        //   - 0: Filter optimization (0 for no optimization).
        //   - PCAP_NETMASK_UNKNOWN: Network mask (unknown in this case).
        bpf_program compiledFilter{};
        if(pcap_compile(pHandle, &compiledFilter, filter.c_str(), 0, PCAP_NETMASK_UNKNOWN) == PCAP_ERROR) {
            const string pcapErrorMsg = pcap_geterr(pHandle);
            pcap_close(pHandle); // Close the pcap handle
            throw PcapErrorException("pcap_compile() error: " + pcapErrorMsg);
        }

        // Set the compiled filter
        if(pcap_setfilter(pHandle, &compiledFilter) == PCAP_ERROR) {
            const string pcapErrorMsg = pcap_geterr(pHandle);
            pcap_freecode(&compiledFilter);  // Free the compiled filter
            pcap_close(pHandle); // Close the pcap handle
            throw PcapErrorException("pcap_setfilter() error: " + pcapErrorMsg);
        }

        // Free the compiled filter
        pcap_freecode(&compiledFilter);

        int udpSocket;
        sockaddr_storage destinationAddress{}; // Either IPv4 or IPv6
        socklen_t addressLength{0};

        // Create an IPv6 socket
        if(scanningIPv6) {
            scanningIPv6 = true;
            udpSocket = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
        }
        // Create an IPv4 socket
        else {
            udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        }

        // Check if socket creation was successful
        if(udpSocket < 0) {
            throw SocketErrorException("Failed to create an UDP socket");
        }

        // Set destination address based on whether the IP address is IPv6 or IPv4
        if(scanningIPv6) {
            // Cast destinationAddress to sockaddr_in6 for IPv6
            auto *pDestinationAddressIPv6 = reinterpret_cast<sockaddr_in6*>(&destinationAddress);

            // Set address family to AF_INET6 and port number for IPv6
            pDestinationAddressIPv6->sin6_family = AF_INET6;
            pDestinationAddressIPv6->sin6_port = htons(portToScan);

            // Convert and set the IPv6 address
            if(inet_pton(AF_INET6, ipAddressToScan.c_str(), &pDestinationAddressIPv6->sin6_addr) <= 0) {
                close(udpSocket);
                throw CommunicationErrorException("inet_pton() error: failed for IPv6: " + ipAddressToScan);
            }

            // Set the address length
            addressLength = sizeof(sockaddr_in6);
        }
        else {
            auto *pDestinationAddressIPv4 = reinterpret_cast<sockaddr_in*>(&destinationAddress);
            pDestinationAddressIPv4->sin_family = AF_INET;
            pDestinationAddressIPv4->sin_port = htons(portToScan);
            if(inet_pton(AF_INET, ipAddressToScan.c_str(), &pDestinationAddressIPv4->sin_addr) <= 0) {
                close(udpSocket);
                throw CommunicationErrorException("inet_pton() error: failed for IPv4: " + ipAddressToScan);
            }
            addressLength = sizeof(sockaddr_in);
        }

        // Ping the target IP address to check if it is reachable
        const ssize_t sent = sendto(udpSocket, nullptr, 0, 0,
                                    reinterpret_cast<sockaddr*>(&destinationAddress), addressLength);

        // Correctly close the socket
        close(udpSocket);

        // sendto() returns -1 on error
        if(sent < 0) {
            throw CommunicationErrorException(
                    "Failed to send UDP packet to " + ipAddressToScan +
                    " on port " + to_string(portToScan) + "."
                    );
        }

        // Capture the next packet
        pcap_pkthdr *pPacketHeader{nullptr};  // structure where info about the captured packet will be stored.
        const u_char *pPacketData{nullptr};   // pointer to the captured packet data
        int elapsedMilliseconds{0};           // Elapsed time in milliseconds
        int pcapResult{0};                    // pcap_next_ex() return value

        while(elapsedMilliseconds < mWaitTimeout.count()) {
            constexpr int pollInterval{100};
            pPacketData = nullptr;
            pcapResult = pcap_next_ex(pHandle, &pPacketHeader, &pPacketData);

            // TODO: Remove debug messages
            // clog << "[DEBUG] pcap_next_ex returned: " << pcapResult
            //         << ", elapsed: " << elapsedMilliseconds << " ms" << endl;

            // ICMP/ICMPv6 captured => port is CLOSED
            if(pcapResult == 1 && pPacketData != nullptr) {
                // TODO: Remove debug messages
                // if(pPacketHeader != nullptr) {
                //     clog << "[DEBUG] Captured packet length: " << pPacketHeader->len << endl;
                // }

                pcap_close(pHandle);
                return ScanResult(ipAddressToScan, portToScan, UDP, PortStatus::CLOSED);
            }
            // Check for errors
            if(pcapResult < 0) {
                // TODO: Remove debug messages
                // clog << "[DEBUG] pcap_next_ex error: " << pcap_geterr(pHandle) << endl;
                break;
            }

            // Check for timeout
            this_thread::sleep_for(chrono::milliseconds(pollInterval));
            elapsedMilliseconds += pollInterval;
        }

        // Close the pcap handle
        pcap_close(pHandle);

        // If no response - considering the port is OPEN.
        // TODO: Remove debug messages
        // cout << "OK: No ICMP message received from " << ipAddressToScan << endl;
        return ScanResult(ipAddressToScan, portToScan, UDP, PortStatus::OPEN);
    } // UdpScanner::scanPort()
} // OmegaL4Scanner::Scanning

/*** end of file UDPScanner.cpp ***/
