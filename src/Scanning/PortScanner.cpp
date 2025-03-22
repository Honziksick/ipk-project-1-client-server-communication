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
 * Last edit:    22.03.2025                                                    *
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
#include "Networking/InterfaceInfo.hpp"
#include "Constants/ColorEscapeSequences.hpp"
#include <string>     // std::string
#include <vector>     // std::vector
#include <chrono>     // std::chrono::milliseconds
#include <algorithm>  // std::clamp
#include <limits>     // std::numeric_limits
#include <iostream>   // std::cout, std::cerr, std::endl

using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Common;
using namespace OmegaL4Scanner::Constants;
using namespace std;

namespace OmegaL4Scanner::Scanning
{
    PortScanner::PortScanner(const vector<string> &ipaddressesToScan,
                             const InterfaceInfo &interfaceInfo,
                             const chrono::milliseconds waitTimeout)
        : mIpaddressesToScan{ipaddressesToScan}, mInterfaceInfo{interfaceInfo}, mWaitTimeout{waitTimeout} {}

    void PortScanner::scanPorts(const vector<PortRange> &portRanges) {
        for(const auto &portSpec : portRanges) {
            int rangeStart{0};
            int rangeEnd{0};

            if(holds_alternative<int>(portSpec)) {
                rangeStart = rangeEnd = get<int>(portSpec);
            }
            else {
                auto [start, end] = get<pair<int, int>>(portSpec);
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
            cerr << COLOR_MAGENTA << "Warning: Timeout value " << waitTimeout.count() <<
                    " ms is out of 'int' range and was clamped to " <<
                    clampedValue << " ms." << RESET << endl;
        }

        // Return the clamped value as an integer
        return static_cast<int>(clampedValue);
    } // PortScanner::millisecondsToInt()
} // OmegaL4Scanner::Scanning

/*** end of file PortScanner.cpp ***/
