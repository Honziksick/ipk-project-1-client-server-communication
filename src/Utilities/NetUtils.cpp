/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         NetUtils.cpp                                                  *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    21.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the implementation of the NetUtils class,  *
 *               which provides utility functions for network operations.      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file NetUtils.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation of the NetUtils class for network operations.
 */

#include "Utilities/NetUtils.hpp"
#include <arpa/inet.h>  // inet_ntop(), sockaddr_in, sockaddr_in6,
#include <sys/socket.h> // AF_INET, AF_INET6

namespace OmegaL4Scanner::Utilities
{
    bool NetUtils::socketAdressToString(const sockaddr *pSocketAddress, const int addressFamily,
                                        char *pAddressBuffer, const size_t bufferSize) {
        const void *pIpAdress;

        // Get the IP address based on the address family
        if(addressFamily == AF_INET) {
            pIpAdress = static_cast<const void*>(&reinterpret_cast<const sockaddr_in*>(pSocketAddress)->sin_addr);
        }
        else {
            pIpAdress = static_cast<const void*>(&reinterpret_cast<const sockaddr_in6*>(pSocketAddress)->sin6_addr);
        }

        return inet_ntop(addressFamily, pIpAdress, pAddressBuffer, bufferSize) != nullptr;
    }
} // OmegaL4Scanner::Utilities

/*** end of file NetUtils.hpp ***/
