/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         NetUtils.hpp                                                  *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the declaration of the NetUtils class,     *
 *               which provides utility functions for network operations.      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file NetUtils.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Declaration of the NetUtils class for network operations.
 */

#ifndef NET_UTILS_HPP
#define NET_UTILS_HPP

#include <sys/socket.h> // sockaddr

namespace OmegaL4Scanner::Utilities
{
    /**
     * @class NetUtils
     * @brief Provides utility functions for network operations.
     *
     * @details The NetUtils class offers static methods.
     */
    class NetUtils final {
    public:
        /**
         * @brief Converts a sockaddr structure to a string representation.
         *
         * @details This static method takes a pointer to a sockaddr structure,
         *          its address family, and a buffer to store the string
         *          representation of the address.
         *
         * @param pSocketAddress Pointer to the sockaddr structure.
         * @param addressFamily Address family (e.g., AF_INET, AF_INET6).
         * @param pAddressBuffer Buffer to store the string representation of the address.
         * @param bufferSize Size of the buffer.
         * @return bool True if the conversion was successful, false otherwise.
         */
        static bool socketaddressToString(const sockaddr *pSocketAddress, int addressFamily,
                                          char *pAddressBuffer, size_t bufferSize);
    }; // NetUtils
} // OmegaL4Scanner::Utilities

#endif // NET_UTILS_HPP

/*** end of file NetUtils.hpp ***/
