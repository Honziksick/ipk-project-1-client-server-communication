/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         PortScanner.cpp                                               *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    23.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the PortScanner class, which is             *
 *               responsible for scanning network ports.                       *
 *                                                                             *
 ******************************************************************************/
/**
 * @file PortScanner.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the PortScanner class.
 */

#include "Scanning/PortScanner.hpp"
#include "Scanning/ScanResult.hpp"
#include "Common/OmegaDataTypes.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include "Networking/InterfaceInfo.hpp"
#include "Constants/IPAddressVersion.hpp"
#include "Constants/ColorEscapeSequences.hpp"
#include <string>     // std::string
#include <vector>     // std::vector
#include <chrono>     // std::chrono::milliseconds
#include <algorithm>  // std::clamp
#include <limits>     // std::numeric_limits
#include <iostream>   // std::cout, std::cerr, std::endl

using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Common;
using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Constants;
using namespace std;

namespace OmegaL4Scanner::Scanning
{
    PortScanner::PortScanner(const vector<string> &ipaddressesToScan,
                             const InterfaceInfo &interfaceInfo,
                             const chrono::milliseconds waitTimeout)
        : mIpaddressesToScan{ipaddressesToScan}, mInterfaceInfo{interfaceInfo}, mWaitTimeout{waitTimeout} {}

    void PortScanner::scanPorts(const vector<PortRange> &portRanges) {
        for(const auto &portVariant : portRanges) {
            int rangeStart{0};
            int rangeEnd{0};

            if(holds_alternative<int>(portVariant)) {
                rangeStart = rangeEnd = get<int>(portVariant);
            }
            else {
                auto [start, end] = get<pair<int, int>>(portVariant);
                rangeStart = start;
                rangeEnd = end;
            }

            for(const auto &ipAddress : mIpaddressesToScan) {
                for(int iPort = rangeStart; iPort <= rangeEnd; iPort++) {
                    cout << scanPort(ipAddress, iPort).scanResultToString() << endl;
                }
            }
        }
    } // PortScanner::scanPorts()

    int PortScanner::millisecondsToInt(const chrono::milliseconds waitTimeout) {
        // Clamps long long waitTimeout to integer (truncate overflow)
        const auto clampedValue = clamp(
                waitTimeout.count(),
                static_cast<int64_t>(numeric_limits<int>::min()),
                static_cast<int64_t>(numeric_limits<int>::max())
                );

        // Check if overflow occurred
        if(clampedValue != waitTimeout.count()) {
            cerr << COLOR_MAGENTA << "Warning: Timeout " << waitTimeout.count() <<
                    " ms is out of 'int' range and was clamped to " <<
                    clampedValue << " ms." << RESET << endl;
        }

        // Return the clamped value as an integer
        return static_cast<int>(clampedValue);
    } // PortScanner::millisecondsToInt()

    string PortScanner::getSourceAddress(const InterfaceInfo &interfaceInfo, const IPAddressVersion ipAddressType) {
        // Select the first IPv4/IPv6 address from the interface
        for(const auto &ipAddress : interfaceInfo.mIpAddresses) {
            if(ipAddressType == IPv4) {
                if(ipAddress.find(':') == string::npos) {
                    return ipAddress;
                }
            }
            else {
                if(ipAddress.find(':') != string::npos) {
                    return ipAddress;
                }
            }
        }
        // Throw an exception if no matching IP address is found
        const string ipVersionStr = (ipAddressType == IPv4) ? "IPv4" : "IPv6";
        throw InternalErrorException("Interface does not contain any " + ipVersionStr + " address.");
    } // PortScanner::getSourceAddress()

    IPAddressVersion PortScanner::getIpAddressVersion(const string &ipAddress) {
        return (ipAddress.find(':') != string::npos) ? IPv6 : IPv4;
    } // PortScanner::getIpAddressType()
} // OmegaL4Scanner::Scanning

/*** end of file PortScanner.cpp ***/
