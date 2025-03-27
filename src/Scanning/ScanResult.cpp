/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ScanResult.cpp                                                *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the ScanResult class, which is responsible  *
 *               for storing the results of scans and formating them for       *
 *               final print.                                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ScanResult.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the ScanResult class.
 */

#include "Scanning/ScanResult.hpp"
#include "Common/OmegaDataTypes.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include "Enums/PortStatus.hpp"
#include <string>  // std::string, std::to_string()

using namespace OmegaL4Scanner::Common;
using namespace OmegaL4Scanner::Enums;
using namespace OmegaL4Scanner::Exceptions;
using namespace std;

namespace OmegaL4Scanner::Scanning
{
    ScanResult::ScanResult(): mPort(0), mStatus(PortStatus::UNKNOWN) {}

    ScanResult::ScanResult(const string &scannedIpaddress, const uint16_t port, const ProtocolType &protocol, const PortStatus status)
        : mScannedIpaddress{scannedIpaddress}, mPort{port}, mProtocol{protocol}, mStatus{status} {}

    string ScanResult::scanResultToString() const {
        string result = mScannedIpaddress + " " + to_string(mPort) + " " + mProtocol + " ";
        switch(mStatus) {
            case PortStatus::OPEN:
                result += "open";
                break;
            case PortStatus::CLOSED:
                result += "closed";
                break;
            case PortStatus::FILTERED:
                result += "filtered";
                break;
            default:
                throw InternalErrorException(
                        "Trying to print scan result with unknown port status."
                        );
        }
        return result;
    }
} // OmegaL4Scanner::Scanning

/*** end of file ScanResult.cpp ***/
