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
 * Last edit:    24.03.2025                                                    *
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
#include "Exceptions/OmegaExceptions.hpp"
#include <arpa/inet.h>   // inet_ntop(), sockaddr_in, sockaddr_in6,
#include <sys/socket.h>  // AF_INET, AF_INET6

using namespace OmegaL4Scanner::Exceptions;

namespace OmegaL4Scanner::Utilities
{
    bool NetUtils::socketaddressToString(const sockaddr *pSocketAddress, const int addressFamily,
                                         char *pAddressBuffer, const size_t bufferSize) {
        if(!pSocketAddress || !pAddressBuffer) {
            throw SocketErrorException("Invalid socket address or address buffer");
            return false;
        }

        // Get the IP address based on the address family
        const void *pIpaddress;
        if(addressFamily == AF_INET) {
            const sockaddr_in *pSocketIn = reinterpret_cast<const sockaddr_in*>(pSocketAddress);
            pIpaddress = static_cast<const void*>(&pSocketIn->sin_addr);
        }
        else {
            const sockaddr_in6 *pSocketIn = reinterpret_cast<const sockaddr_in6*>(pSocketAddress);
            pIpaddress = static_cast<const void*>(&pSocketIn->sin6_addr);
        }

        if(!pIpaddress) {
            throw InternalErrorException(
                    "Failed to retrieve IP address from socket address"
                    );
            return false;
        }

        return inet_ntop(addressFamily, pIpaddress, pAddressBuffer, bufferSize) != nullptr;
    } // NetUtils::socketaddressToString()
} // OmegaL4Scanner::Utilities

/*** end of file NetUtils.hpp ***/
