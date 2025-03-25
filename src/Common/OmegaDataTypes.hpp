/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaDataTypes.hpp                                            *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      15.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description:  Header file for custom data types used in the OMEGA L4        *
 *               Scanner project.                                              *
 *                                                                             *
 ******************************************************************************/
/**
 * @file OmegaDataTypes.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for custom data types used in the OMEGA L4 Scanner project.
 */

#ifndef OMEGA_DATA_TYPES_HPP
#define OMEGA_DATA_TYPES_HPP

#include <string>   // std::string
#include <variant>  // std::variant<T...>
#include <utility>  // std::pair<T1,T2>

namespace OmegaL4Scanner::Common
{
    /**
     * @brief Type definition for port number or port range.
     *
     * @details This type can represent either a single port (int),
     *          or a range of ports (std::pair<int, int>).
     */
    using PortRange = std::variant<int, std::pair<int, int>>;

    /**
     * @brief Alias for std::string representation of protocol types.
     *
     * @details This alias represents the protocol type as a string.
     *          It can be used to specify protocols such as "tcp" or "udp".
     *
     * @note Values are stored in Constants/ProtocolTypes file under
     *       OmegaL4Scanner::Constants::ProtocolTypes namespace.
     * @note The "tcp" and "udp" protocols are represented by constants TCP and UDP.
     */
    using ProtocolType = std::string;

    /**
     * @brief Alias for boolean representing the type of IP address.
     *
     * @details This alias is used to specify the type of IP address.
     *          - `true` represents an IPv6 address.
     *          - `false` represents an IPv4 address.
     */
    using IPAddressVersion = bool;
} // OmegaL4Scanner::Common

#endif //OMEGA_DATA_TYPES_HPP

/*** end of file OmegaDataTypes.hpp ***/
