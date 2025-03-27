/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         IPAddressVersion.hpp                                          *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      24.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description:  Header file defining IP address types constants for the       *
 *               OMEGA L4 Scanner project.                                     *
 *                                                                             *
 ******************************************************************************/
/**
 * @file IPAddressVersion.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for IP address types constants.
 */

#ifndef IP_ADDRESS_VERSION_HPP
#define IP_ADDRESS_VERSION_HPP

#include "Common/OmegaDataTypes.hpp"

namespace OmegaL4Scanner::Constants
{
    inline constexpr Common::IpAddressVersion IPv4 = true;     /**< Constant for IPv4 address type. */
    inline constexpr Common::IpAddressVersion IPv6 = false;    /**< Constant for IPv6 address type. */
} // OmegaL4Scanner::Constants

#endif // IP_ADDRESS_VERSION_HPP

/*** end of file IPAddressVersion.hpp ***/
