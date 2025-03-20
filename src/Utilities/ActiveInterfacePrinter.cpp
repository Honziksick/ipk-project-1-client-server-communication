/*******************************************************************************
*                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ActiveInterfacePrinter.cpp                                    *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains implementation of ActiveInterfacePrinter   *
 *               class, which provides functionality to print information      *
 *               about active network interfaces.                              *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ActiveInterfacePrinter.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation of the ActiveInterfacePrinter class.
 */

#include "Networking/InterfaceInfo.hpp"
#include "Networking/InterfaceManager.hpp"
#include "Utilities/ActiveInterfacePrinter.hpp"
#include "Constants/ColorEscapeSequences.hpp"
#include <iostream> // std::cout, std::endl
#include <vector>   // std::vector
#include <map>      // std::map

using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Constants;
using namespace std;

namespace OmegaL4Scanner::Utilities
{
    void ActiveInterfacePrinter::printActiveInterfaces() {
        const vector<InterfaceInfo> activeInterfaces = InterfaceManager::getActiveInterfaces();

        // Group interfaces by their name
        map<string, vector<InterfaceInfo>> interfaceMap;
        for(const auto &interface : activeInterfaces) {
            interfaceMap[interface.mName].push_back(interface);
        }

        printHeader();

        size_t groupCounter = 0;
        const size_t totalGroups = interfaceMap.size();
        for(const auto &[name, interfaceGroup] : interfaceMap) {
            printInterfaceGroup(name, interfaceGroup);
            groupCounter++;

            // Print a separator between groups
            if(groupCounter < totalGroups) {
                printSeparator(BETWEEN_INTERFACES_SEPARATOR, SEPARATOR_LENGTH);
            }
        }

        printSeparator(TITLE_SEPARATOR, SEPARATOR_LENGTH);
    } // ActiveInterfacePrinter::printActiveInterfaces()

    void ActiveInterfacePrinter::printHeader() {
        printSeparator(TITLE_SEPARATOR, SEPARATOR_LENGTH);
        cout << "|                          Active Network Interfaces                           |" << endl;
        printSeparator(TITLE_SEPARATOR, SEPARATOR_LENGTH);
    } // ActiveInterfacePrinter::printHeader()

    void ActiveInterfacePrinter::printIpAddressDetails(const int index, const string &ip, const string &netmask,
                                                       const string &broadcast, const string &destination) {
        cout << "    " << index << ") " << ip << endl;
        cout << "       - " << COLOR_YELLOW << "Netmask: " << RESET
                << (netmask.empty() ? "N/A" : netmask) << endl;
        cout << "       - " << COLOR_MAGENTA << "Broadcast Address: " << RESET
                << (broadcast.empty() ? "N/A" : broadcast) << endl;
        cout << "       - " << COLOR_GREEN << "Destination Address: " << RESET
                << (destination.empty() ? "N/A" : destination) << endl;
    } // ActiveInterfacePrinter::printIPAddressDetails()

    void ActiveInterfacePrinter::printInterfaceGroup(const string &interfaceName, const vector<InterfaceInfo> &interfaceGroup) {
        cout << FORMAT_BOLD << "Interface: " << interfaceName << RESET << endl;
        cout << COLOR_RED << "  Flags: " << RESET << interfaceGroup.front().mFlags << endl;
        cout << COLOR_CYAN << "  IP Addresses:" << RESET << endl;

        int counter = 1;
        for(const auto &interface : interfaceGroup) {
            // Print details for each IP address
            for(const auto &ip : interface.mIpAddresses) {
                printIpAddressDetails(counter, ip, interface.mNetmask,
                                      interface.mBroadcastAddress, interface.mDestinationAddress);
                counter++;
            }
        }
    } // ActiveInterfacePrinter::printInterfaceGroup()

    void ActiveInterfacePrinter::printSeparator(const char character, const int count) {
        for(int i = 0; i < count; ++i) {
            cout << character;
        }
        cout << endl;
    } // ActiveInterfacePrinter::printSeparator()
} // OmegaL4Scanner::Utilities

/*** end of file ActiveInterfacePrinter.cpp ***/
