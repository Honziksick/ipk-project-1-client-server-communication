/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ActiveInterfacePrinter.hpp                                    *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains declaration of the ActiveInterfacePrinter  *
 *               class, which provides functionality to print information      *
 *               about active network interfaces.                              *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ActiveInterfacePrinter.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Declaration of the ActiveInterfacePrinter class.
 */

#ifndef ACTIVE_INTERFACE_PRINTER_HPP
#define ACTIVE_INTERFACE_PRINTER_HPP

#include "Networking/InterfaceInfo.hpp"
#include <string> // std::string
#include <vector> // std::vector

namespace OmegaL4Scanner::Utilities
{
    /**
     * @class ActiveInterfacePrinter
     * @brief A utility class for printing information about active network interfaces.
     */
    class ActiveInterfacePrinter {
    public:
        /**
         * @brief Prints information about all active network interfaces.
         */
        static void printActiveInterfaces();

    private:
        /**
         * @brief Prints detailed information about a single network interface.
         *
         * @param interface An InterfaceInfo object representing a single
         *                  network interface.
         */
        static void printSingleInterfaceInfo(const Networking::InterfaceInfo &interface);

        /**
         * @brief Prints the header for the active network interfaces section.
         */
        static void printHeader();

        /**
         * @brief Prints detailed information about an IP address.
         *
         * @param index The index of the IP address.
         * @param ip The IP address.
         * @param netmask The netmask of the IP address.
         * @param broadcast The broadcast address of the IP address.
         * @param destination The destination address of the IP address.
         */
        static void printIpAddressDetails(int index, const std::string &ip, const std::string &netmask,
                                          const std::string &broadcast, const std::string &destination);

        /**
         * @brief Prints information about a group of network interfaces with
         *        the same name.
         *
         * @param interfaceName The name of the network interface group.
         * @param interfaceGroup A vector of InterfaceInfo objects representing
         *                   the network interface group.
         */
        static void printInterfaceGroup(const std::string &interfaceName,
                                        const std::vector<Networking::InterfaceInfo> &interfaceGroup);

        /**
         * @brief Prints a separator line consisting of a repeated character.
         *
         * @param character The character to be repeated.
         * @param count The number of times the character should be repeated.
         */
        static void printSeparator(char character, int count);

        // Print seprators
        static constexpr char BETWEEN_INTERFACES_SEPARATOR = '-'; /**< Separator between interfaces. */
        static constexpr char TITLE_SEPARATOR = '=';              /**< Separator for title.          */
        static constexpr int SEPARATOR_LENGTH = 80;               /**< Length of the separator line. */
    }; // ActiveInterfacePrinter
} // OmegaL4Scanner::Utilities

#endif // ACTIVE_INTERFACE_PRINTER_HPP

/*** end of file ActiveInterfacePrinter.hpp **/
