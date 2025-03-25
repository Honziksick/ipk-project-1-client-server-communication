/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         AddressInfo.hpp                                               *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      19.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the AddressInfo class, which  provides      *
 *               information about IP addresse of given interface.             *
 *                                                                             *
 ******************************************************************************/
/**
 * @file AddressInfo.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the AddressInfo class.
 */

#ifndef ADDRESS_INFO_HPP
#define ADDRESS_INFO_HPP

#include <string> // std::string

namespace OmegaL4Scanner::Networking
{
    /**
     * @class AddressInfo
     * @brief Provides information about IP addresses of a given interface.
     */
    class AddressInfo final {
    public:
        std::string mIpAddress;           /**< IP address. */
        std::string mNetmask;             /**< Netmask.    */
        std::string mBroadcastAddress;    /**< Broadcast address for IPv4; "N/A" for IPv6.                  */
        std::string mDestinationAddress;  /**< Destination address for point-to-point IPv4; "N/A" for IPv6. */

        static constexpr std::string N_A = "N/A";  /**< Not applicable string. */
    }; // AddressInfo
} // OmegaL4Scanner::Networking

#endif // ADDRESS_INFO_HPP

/*** end of file AddressInfo.hpp ***/
