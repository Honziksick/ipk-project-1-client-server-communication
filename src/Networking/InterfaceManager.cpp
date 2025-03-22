/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         InterfaceManager.cpp                                          *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the InterfaceManager class, which is        *
 *               responsible for managing network interfaces.                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file InterfaceManager.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the InterfaceManager class.
 */

#include "Exceptions/OmegaExceptions.hpp"
#include "Networking/InterfaceManager.hpp"
#include "Networking/InterfaceInfo.hpp"
#include "Utilities/StringUtils.hpp"
#include <string>       // std::string
#include <vector>       // std::vector
#include <cstring>      // std::strerror()
#include <arpa/inet.h>  // inet_ntop(), sockaddr_in, sockaddr_in6, INET6_ADDRSTRLEN
#include <ifaddrs.h>    // ifaddrs, getifaddrs(), freeifaddrs()
#include <sys/socket.h> // AF_INET, AF_INET6
#include <net/if.h>     // IFF_UP

#include "Utilities/NetUtils.hpp"

using namespace std;
using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Utilities;

namespace OmegaL4Scanner::Networking
{
    // My implementation inspired by: https://dev.to/fmtweisszwerg/cc-how-to-get-all-interface-addresses-on-the-local-device-3pki
    vector<InterfaceInfo> InterfaceManager::getActiveInterfaces() {
        vector<InterfaceInfo> activeInterfaces{};  // Vector/list of active interfaces
        ifaddrs *pInterfaceAddresses{};  // Pointer to the linked list of interface addresses

        // getifaddrs() returns -1 on error
        if(getifaddrs(&pInterfaceAddresses) == -1) {
            throw InternalErrorException(
                    "getifaddrs() error:" + string(strerror(errno))
                    );
        }

        // Iterate through the linked list of loaded interface addresses
        const ifaddrs *pCurrentInterface = pInterfaceAddresses;
        while(pCurrentInterface != nullptr) {
            // If the interface address is not set, skip it
            if(pCurrentInterface->ifa_addr == nullptr) {
                pCurrentInterface = pCurrentInterface->ifa_next;
                continue;
            }

            // If the interface is not up, skip it
            if(!(pCurrentInterface->ifa_flags & IFF_UP)) {
                pCurrentInterface = pCurrentInterface->ifa_next;
                continue;
            }

            // Add the interface to the list of active interfaces
            InterfaceInfo interfaceInfo(pCurrentInterface->ifa_name);

            // Get the IP address of the interface
            char ipAddressBuffer[INET6_ADDRSTRLEN]{};
            if(const int addressFamily = pCurrentInterface->ifa_addr->sa_family; addressFamily == AF_INET || addressFamily == AF_INET6) {
                if(NetUtils::socketaddressToString(pCurrentInterface->ifa_addr, addressFamily, ipAddressBuffer, sizeof(ipAddressBuffer))) {
                    interfaceInfo.mIpAddresses.emplace_back(ipAddressBuffer);
                }
            }

            // Initialize the netmask in InterfaceInfo
            if(pCurrentInterface->ifa_netmask) {
                const int addressFamily = pCurrentInterface->ifa_addr->sa_family;

                // Get the netmask based on the address family
                if(addressFamily == AF_INET) {
                    const auto *sockAddrIn = reinterpret_cast<struct sockaddr_in*>(pCurrentInterface->ifa_netmask);

                    // Convert the netmask to a string
                    if(inet_ntop(AF_INET,
                                 &sockAddrIn->sin_addr,
                                 ipAddressBuffer,
                                 sizeof(ipAddressBuffer)) != nullptr) {
                        interfaceInfo.mNetmask = ipAddressBuffer;
                    }
                }
                else if(addressFamily == AF_INET6) {
                    const auto *sockAddrIn6 = reinterpret_cast<struct sockaddr_in6*>(pCurrentInterface->ifa_netmask);

                    // Convert the netmask to a string
                    if(inet_ntop(AF_INET6,
                                 &sockAddrIn6->sin6_addr,
                                 ipAddressBuffer,
                                 sizeof(ipAddressBuffer)) != nullptr) {
                        interfaceInfo.mNetmask = ipAddressBuffer;
                    }
                }
            }

            // Initialize the broadcast address in InterfaceInfo
            if(pCurrentInterface->ifa_ifu.ifu_broadaddr && pCurrentInterface->ifa_addr->sa_family == AF_INET) {
                if(inet_ntop(AF_INET,
                             &reinterpret_cast<sockaddr_in*>(pCurrentInterface->ifa_ifu.ifu_broadaddr)->sin_addr,
                             ipAddressBuffer,
                             sizeof(ipAddressBuffer)) != nullptr) {
                    interfaceInfo.mBroadcastAddress = ipAddressBuffer;
                }
            }

            // Initialize the destination address in InterfaceInfo
            if(pCurrentInterface->ifa_ifu.ifu_dstaddr && pCurrentInterface->ifa_addr->sa_family == AF_INET) {
                if(inet_ntop(AF_INET,
                             &reinterpret_cast<sockaddr_in*>(pCurrentInterface->ifa_ifu.ifu_dstaddr)->sin_addr,
                             ipAddressBuffer,
                             sizeof(ipAddressBuffer)) != nullptr) {
                    interfaceInfo.mDestinationAddress = ipAddressBuffer;
                }
            }

            // Initialize the flags in InterfaceInfo
            interfaceInfo.mFlags = pCurrentInterface->ifa_flags;

            // Add the interface to the list of active interfaces
            activeInterfaces.emplace_back(interfaceInfo);

            // Move to the next interface in the linked list
            pCurrentInterface = pCurrentInterface->ifa_next;
        }

        freeifaddrs(pInterfaceAddresses);
        return activeInterfaces;
    } // InterfaceManager::getActiveInterfaces()

    InterfaceInfo InterfaceManager::getInterfaceByName(const string &name) {
        const string searchName = StringUtils::toLower(name);
        for(auto interfaces = getActiveInterfaces(); auto &interface : interfaces) {
            if(StringUtils::toLower(interface.mName) == searchName) {
                return interface;
            }
        }
        throw InterfaceErrorException(
                "Active interface with name '" + name + "' not found."
                );
    } // InterfaceManager::getInterfaceByName()
} // OmegaL4Scanner::Networking

/*** end of file InterfaceManager.cpp ***/
