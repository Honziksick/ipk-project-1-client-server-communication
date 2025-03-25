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
 * Last edit:    25.03.2025                                                    *
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

using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Constants;
using namespace std;

namespace OmegaL4Scanner::Utilities
{
    void ActiveInterfacePrinter::printActiveInterfaces() {
        const vector<InterfaceInfo> activeInterfaces = InterfaceManager::getActiveInterfaces();

        printHeader();

        size_t interfaceCounter = 0;
        const size_t interfacesTotal = activeInterfaces.size();
        for(const auto &interface : activeInterfaces) {
            printInterfaceInfo(interface);
            interfaceCounter++;

            // Print a separator between groups
            if(interfaceCounter < interfacesTotal) {
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
        cout << "       - " << COLOR_YELLOW  << "Netmask: "             << RESET << netmask << endl;
        cout << "       - " << COLOR_MAGENTA << "Broadcast Address: "   << RESET << broadcast << endl;
        cout << "       - " << COLOR_GREEN   << "Destination Address: " << RESET << destination << endl;
    } // ActiveInterfacePrinter::printIPAddressDetails()

    void ActiveInterfacePrinter::printInterfaceInfo(const InterfaceInfo &interfaceInfo) {
        cout << FORMAT_BOLD << "Interface: "     << interfaceInfo.mName << RESET << endl;
        cout << COLOR_RED   << "  Flags: "       << RESET << interfaceInfo.mFlags << endl;
        cout << COLOR_CYAN  << "  IP Addresses:" << RESET << endl;

        int ipCounter{1};
        for(const auto &ipAddress : interfaceInfo.mIpAddresses) {
            printIpAddressDetails(ipCounter, ipAddress.mIpAddress, ipAddress.mNetmask,
                                  ipAddress.mBroadcastAddress, ipAddress.mDestinationAddress);
            ipCounter++;
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
