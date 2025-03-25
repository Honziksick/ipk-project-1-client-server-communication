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

#include "Networking/InterfaceManager.hpp"
#include "Networking/InterfaceInfo.hpp"
#include "Networking/AddressInfo.hpp"
#include "Exceptions/OmegaExceptions.hpp"
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
            const string interfaceName = pCurrentInterface->ifa_name;
            if(!interfaceMap.contains(interfaceName)) {
                interfaceMap[interfaceName] = InterfaceInfo(interfaceName, pCurrentInterface->ifa_flags);
            }

            // Determine the address family (IPv4 or IPv6)
            const int addressFamily = pCurrentInterface->ifa_addr->sa_family;

            // Get the IP address of the interface
            AddressInfo addressInfo;
            char ipBuffer[INET6_ADDRSTRLEN]{};  // both IPv4 and IPv6 will fit

            if(NetUtils::socketaddressToString(pCurrentInterface->ifa_addr, addressFamily,
                                               ipBuffer, sizeof(ipBuffer))) {
                addressInfo.mIpAddress = ipBuffer;
            }

            // Proccess IPv4 address
            if(addressFamily == AF_INET) {
                // Initialize the netmask for IPv4
                if(pCurrentInterface->ifa_netmask) {
                    char netmaskBuffer[INET_ADDRSTRLEN]{};
                    const auto *pSocketAddressIn =
                            reinterpret_cast<struct sockaddr_in*>(pCurrentInterface->ifa_netmask);

                    // Convert the netmask to a string
                    if(inet_ntop(AF_INET, &pSocketAddressIn->sin_addr, netmaskBuffer,
                                 sizeof(netmaskBuffer)) != nullptr) {
                        addressInfo.mNetmask = netmaskBuffer;
                    }
                }

                // Initialize the broadcast address for IPv4 (only if IFF_BROADCAST is set)
                if(pCurrentInterface->ifa_ifu.ifu_broadaddr && (pCurrentInterface->ifa_flags & IFF_BROADCAST)) {
                    char broadcastBuffer[INET_ADDRSTRLEN]{};
                    const auto *pSocketAddressIn =
                            reinterpret_cast<struct sockaddr_in*>(pCurrentInterface->ifa_ifu.ifu_broadaddr);

                    // Convert the broadcast address to a string
                    if(inet_ntop(AF_INET, &pSocketAddressIn->sin_addr, broadcastBuffer,
                                 sizeof(broadcastBuffer)) != nullptr) {
                        addressInfo.mBroadcastAddress = broadcastBuffer;
                    }
                }
                else {
                    addressInfo.mBroadcastAddress = AddressInfo::N_A;
                }

                // Initialize the destination address for IPv4 point-to-point interfaces
                if(pCurrentInterface->ifa_ifu.ifu_dstaddr && (pCurrentInterface->ifa_flags & IFF_POINTOPOINT)) {
                    char destinationBuffer[INET_ADDRSTRLEN]{};
                    const auto *pSocketAddressIn =
                            reinterpret_cast<sockaddr_in*>(pCurrentInterface->ifa_ifu.ifu_dstaddr);

                    if(inet_ntop(AF_INET, &pSocketAddressIn->sin_addr, destinationBuffer,
                                 sizeof(destinationBuffer)) != nullptr) {
                        addressInfo.mDestinationAddress = destinationBuffer;
                    }
                }
                else {
                    addressInfo.mDestinationAddress = AddressInfo::N_A;
                }
            }
            // Proccess IPv6 address
            else if(addressFamily == AF_INET6) {
                // Initialize the netmask for IPv6
                addressInfo.mNetmask = AddressInfo::N_A;
                addressInfo.mBroadcastAddress = AddressInfo::N_A;
                addressInfo.mDestinationAddress = AddressInfo::N_A;
            }
            // Skip the first blank address of interface
            else {
                continue;
            }
            // Initialize the flags in InterfaceInfo
            interfaceMap[interfaceName].mIpAddresses.emplace_back(addressInfo);
        }

        freeifaddrs(pInterfaceAddresses);

        // Convert the unordered_map to a vector
        for(auto &interface : interfaceMap) {
            activeInterfaces.push_back(interface.second);
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
