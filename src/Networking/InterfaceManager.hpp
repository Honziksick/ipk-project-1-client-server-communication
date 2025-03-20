/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         InterfaceManager.hpp                                          *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the InterfaceManager class, which is           *
 *               responsible for managing network interfaces.                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file InterfaceManager.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the InterfaceManager class.
 */

#ifndef INTERFACE_MANAGER_HPP
#define INTERFACE_MANAGER_HPP

#include "Networking/InterfaceInfo.hpp"
#include <string> // std::string
#include <vector> // std::vector

namespace OmegaL4Scanner::Networking
{
    /**
     * @class InterfaceManager
     * @brief Provides functions to retrieve information about network interfaces.
     */
    class InterfaceManager final {
    public:
        /**
         * @brief Returns a list of active network interfaces.
         *
         * @details Active interfaces are those that have the IFF_UP flag set.
         *
         * @return std::vector<NetworkInterfaceInfo> List of active interfaces.
         */
        static std::vector<InterfaceInfo> getActiveInterfaces();

        /**
        * @brief Finds a network interface by name.
         *
         * @param name The name of the interface (e.g., "eth0").
         * @return Information about the interface if it exists, otherwise nullptr.
         */
        static InterfaceInfo getInterfaceByName(const std::string &name);
    }; // OmegaL4Scanner::Networking
} // OmegaL4Scanner::Networking

#endif // INTERFACE_MANAGER_HPP

/*** end of file InterfaceManager.hpp ***/
