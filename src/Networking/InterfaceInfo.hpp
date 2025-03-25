/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         InterfaceInfo.hpp                                             *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      19.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the InterfaceInfo class, which              *
 *               provides information about network interfaces.                *
 *                                                                             *
 ******************************************************************************/
/**
 * @file InterfaceInfo.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the InterfaceInfo class.
 */

#ifndef INTERFACE_INFO_HPP
#define INTERFACE_INFO_HPP

#include "Networking/AddressInfo.hpp"
#include <string> // std::string
#include <vector> // std::vector

namespace OmegaL4Scanner::Networking
{
    /**
     * @class InterfaceInfo
     * @brief Represents a network interface.
     *
     * @details This class provides information about a network interface,
     *          including its name and associated IP addresses.
     */
    class InterfaceInfo final {
    public:
        /**
         * @brief Default constructor for the InterfaceInfo class.
         */
        InterfaceInfo();

        /**
         * @brief Constructs a NetworkInterfaceInfo object.
         *
         * @param interfaceName The name of the network interface.
         * @param flags The flags of the network interface.
         */
        explicit InterfaceInfo(std::string interfaceName, unsigned int flags);

        std::string mName;                      /**< The name of the network interface.                            */
        std::vector<AddressInfo> mIpAddresses;  /**< A list of IP addresses associated with the network interface. */
        unsigned int mFlags;                    /**< The flags of the network interface.                           */
    }; // InterfaceInfo
} // OmegaL4Scanner::Networking

#endif // INTERFACE_INFO_HPP

/*** end of file InterfaceInfo.hpp ***/
