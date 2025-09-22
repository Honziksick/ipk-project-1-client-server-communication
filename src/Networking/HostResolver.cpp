/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         HostResolver.cpp                                              *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    23.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the HostResolver class, which is            *
 *               responsible for resolving hostnames to IP addresses.          *
 *                                                                             *
 ******************************************************************************/
/**
 * @file HostResolver.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation of the HostResolver class.
 */

#include "Networking/HostResolver.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include "Utilities/NetUtils.hpp"
#include <algorithm> // std::sort, std::unique
#include <ranges>    // std::ranges::sort, std::ranges::unique
#include <cstring>   // memset
#include <netdb.h>   // getaddrinfo(), gai_strerror(), freeaddrinfo()

using namespace OmegaL4Scanner::Utilities;
using namespace OmegaL4Scanner::Exceptions;
using namespace std;

namespace OmegaL4Scanner::Networking
{
    vector<string> HostResolver::resolveHost(const string &targetHost) {
        vector<string> ipAddresses;

        // Prepare the 'hints' structure for address resolution
        addrinfo hints{};
        memset(&hints, 0, sizeof(hints));

        hints.ai_family = AF_UNSPEC; // get both IPv4 and IPv6 addresses
        hints.ai_socktype = 0;       // any socket type (TCP/UDP)
        hints.ai_flags = 0;          // get only real IPs

        // Perform the address resolution
        addrinfo *pResult{nullptr};
        const int getaddrinfoError = getaddrinfo(targetHost.c_str(), nullptr, &hints, &pResult);
        if(getaddrinfoError != 0) {
            throw HostnameResolutionErrorException(
                    "getaddrinfo() error: " + string(gai_strerror(getaddrinfoError))
                    );
        }

        // Iterate through the results and convert the IP address to a string
        for(const addrinfo *pAddressInfo = pResult; pAddressInfo != nullptr; pAddressInfo = pAddressInfo->ai_next) {
            char ipBuffer[INET6_ADDRSTRLEN]{};
            if(NetUtils::socketaddressToString(pAddressInfo->ai_addr, pAddressInfo->ai_family, ipBuffer, sizeof(ipBuffer))) {
                ipAddresses.emplace_back(ipBuffer);
            }
        }

        // Free the address info structure
        freeaddrinfo(pResult);

        // Remove duplicates from the vector
        ranges::sort(ipAddresses);
        auto [uniqueBegin, uniqueEnd] = ranges::unique(ipAddresses);
        ipAddresses.erase(uniqueBegin, uniqueEnd);

        return ipAddresses;
    } // HostResolver::resolveHost()
} // OmegaL4Scanner::Networking

/*** end of file HostResolver.cpp ***/
