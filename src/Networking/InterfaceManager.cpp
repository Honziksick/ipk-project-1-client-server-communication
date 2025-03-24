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
 * Last edit:    23.03.2025                                                    *
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
#include <string>        // std::string
#include <vector>        // std::vector
#include <unordered_map> // std::unordered_map
#include <cstring>       // std::strerror()
#include <arpa/inet.h>   // inet_ntop(), sockaddr_in, sockaddr_in6, INET6_ADDRSTRLEN
#include <ifaddrs.h>     // ifaddrs, getifaddrs(), freeifaddrs()
#include <sys/socket.h>  // AF_INET, AF_INET6
#include <net/if.h>      // IFF_UP

#include "Utilities/NetUtils.hpp"

using namespace std;
using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Utilities;

namespace OmegaL4Scanner::Networking
{
    // My implementation inspired by: https://dev.to/fmtweisszwerg/cc-how-to-get-all-interface-addresses-on-the-local-device-3pki
    vector<InterfaceInfo> InterfaceManager::getActiveInterfaces() {
        vector<InterfaceInfo> activeInterfaces{};  // Vector/list of active interfaces
        ifaddrs *pInterfaceAddresses{nullptr};     // Pointer to the linked list of interface addresses

        // getifaddrs() returns -1 on error
        if(getifaddrs(&pInterfaceAddresses) == -1) {
            throw InternalErrorException(
                    "getifaddrs() error:" + string(strerror(errno))
                    );
        }

        // Group the interfaces by name
        unordered_map<string, InterfaceInfo> interfaceMap;

        // Iterate through the linked list of loaded interface addresses
        for(const ifaddrs *pCurrentInterface = pInterfaceAddresses;
            pCurrentInterface != nullptr; pCurrentInterface = pCurrentInterface->ifa_next) {
            // If the interface address is not set, skip it
            if(pCurrentInterface->ifa_addr == nullptr) {
                continue;
            }

            // If the interface is not up, skip it
            if(!(pCurrentInterface->ifa_flags & IFF_UP)) {
                continue;
            }

            // Create new InterfaceInfo object for new interface name
            string interfaceName = pCurrentInterface->ifa_name;
            if(!interfaceMap.contains(interfaceName)) {
                interfaceMap[interfaceName] = InterfaceInfo(interfaceName);
            }

            // Get the IP address of the interface
            const int addressFamily = pCurrentInterface->ifa_addr->sa_family;
            if(addressFamily == AF_INET || addressFamily == AF_INET6) {
                char ipBuffer[INET6_ADDRSTRLEN]{};
                if(NetUtils::socketaddressToString(pCurrentInterface->ifa_addr, addressFamily, ipBuffer, sizeof(ipBuffer))) {
                    interfaceMap[interfaceName].mIpAddresses.emplace_back(ipBuffer);
                }
            }

            // Initialize the netmask in InterfaceInfo
            if(pCurrentInterface->ifa_netmask) {
                const int family = pCurrentInterface->ifa_addr->sa_family;
                char netmaskBuffer[INET6_ADDRSTRLEN]{};

                // Get the netmask based on the address family
                if(family == AF_INET) {
                    auto *sockAddrIn = reinterpret_cast<struct sockaddr_in*>(pCurrentInterface->ifa_netmask);

                    // Convert the netmask to a string
                    if(inet_ntop(AF_INET, &sockAddrIn->sin_addr, netmaskBuffer, sizeof(netmaskBuffer)) != nullptr) {
                        interfaceMap[interfaceName].mNetmask = netmaskBuffer;
                    }
                }
                else if(family == AF_INET6) {
                    const auto *sockAddrIn6 = reinterpret_cast<struct sockaddr_in6*>(pCurrentInterface->ifa_netmask);
                    if(inet_ntop(AF_INET6, &sockAddrIn6->sin6_addr, netmaskBuffer, sizeof(netmaskBuffer)) != nullptr) {
                        interfaceMap[interfaceName].mNetmask = netmaskBuffer;
                    }
                }
            }

            // Initialize the broadcast address in InterfaceInfo
            if(pCurrentInterface->ifa_ifu.ifu_broadaddr && pCurrentInterface->ifa_addr->sa_family == AF_INET) {
                char broadBuffer[INET_ADDRSTRLEN]{};
                if(inet_ntop(AF_INET,
                             &reinterpret_cast<sockaddr_in*>(pCurrentInterface->ifa_ifu.ifu_broadaddr)->sin_addr,
                             broadBuffer, sizeof(broadBuffer)) != nullptr) {
                    interfaceMap[interfaceName].mBroadcastAddress = broadBuffer;
                }
            }

            // Initialize the destination address in InterfaceInfo
            if(pCurrentInterface->ifa_ifu.ifu_dstaddr && pCurrentInterface->ifa_addr->sa_family == AF_INET) {
                char destBuffer[INET_ADDRSTRLEN]{};
                if(inet_ntop(AF_INET,
                             &reinterpret_cast<sockaddr_in*>(pCurrentInterface->ifa_ifu.ifu_dstaddr)->sin_addr,
                             destBuffer, sizeof(destBuffer)) != nullptr) {
                    interfaceMap[interfaceName].mDestinationAddress = destBuffer;
                }
            }

            // Initialize the flags in InterfaceInfo
            interfaceMap[interfaceName].mFlags = pCurrentInterface->ifa_flags;
        }

        freeifaddrs(pInterfaceAddresses);

        // Convert the unordered_map to a vector
        for(auto &entry : interfaceMap) {
            activeInterfaces.push_back(entry.second);
        }

        return activeInterfaces;
    } // InterfaceManager::getActiveInterfaces()

    InterfaceInfo InterfaceManager::getInterfaceByName(const string &name) {
        const string searchName = StringUtils::toLower(name);
        auto interfaces = getActiveInterfaces();
        for(auto &interface : interfaces) {
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
