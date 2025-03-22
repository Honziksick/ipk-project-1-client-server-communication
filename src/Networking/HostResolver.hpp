/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         HostResolver.hpp                                              *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the HostResolver class, which is responsible   *
 *               for resolving hostnames to IP addresses.                      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file HostResolver.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the HostResolver class.
 */

#ifndef HOST_RESOLVER_HPP
#define HOST_RESOLVER_HPP

#include <vector> // std::vector
#include <string> // std::string

namespace OmegaL4Scanner::Networking
{
    /**
     * @class HostResolver
     * @brief A class responsible for resolving hostnames to IP addresses.
     *
     * @details Uses getaddrinfo to obtain both IPv4 and IPv6 addresses.
     */
    class HostResolver final {
    public:
        /**
         * @brief Resolves the given target (hostname or IP) to a list of
         *        IP addresses.
         *
         * @param targetHost The hostname to resolve.
         * @return A vector of strings containing the resolved IP addresses.
         */
        static std::vector<std::string> resolveHost(const std::string &targetHost);
    }; // HostResolver
} // OmegaL4Scanner::Networking

#endif // HOST_RESOLVER_HPP

/*** end of file HostResolver.hpp ***/
