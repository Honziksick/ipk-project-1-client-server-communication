/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ScannerController.cpp                                         *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    23.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the ScannerController class, which is       *
 *               responsible for controlling the scanning process.             *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ScannerController.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the ScannerController class.
 */

#include "Facades/ScannerController.hpp"
#include "Common/CommandLineOptions.hpp"
#include "Common/OmegaDataTypes.hpp"
#include "Networking/HostResolver.hpp"
#include "Scanning/TCPScanner.hpp"
#include "Scanning/UDPScanner.hpp"
#include <string>  // std::string
#include <utility> // std::move()
#include <vector>  // std::vector

using namespace OmegaL4Scanner::Scanning;
using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Common;
using namespace std;

namespace OmegaL4Scanner::Facades
{
    ScannerController::ScannerController(CommandLineOptions options, InterfaceInfo interfaceInfo)
        : mCommandLineOptions{move(options)}, mInterfaceInfo{move(interfaceInfo)} {}

    void ScannerController::scanL4Layer() const {
        const vector<string> ipAddresses = HostResolver::resolveHost(mCommandLineOptions.mTarget);

        scanTcpPorts(ipAddresses);
        scanUdpPorts(ipAddresses);
    } // ScannerController::scanL4Layer()

    // TODO: Uncomment when TCP scanning implementation is implemented
    void ScannerController::scanTcpPorts(const vector<string> &ipAddresses) const {
        if(!mCommandLineOptions.mTcpPorts.empty()) {
            TcpScanner tcpScanner(ipAddresses, mInterfaceInfo, mCommandLineOptions.mWaitTimeout);
            tcpScanner.scanPorts(mCommandLineOptions.mTcpPorts);
        }
    } // ScannerController::scanTcpPorts()

    void ScannerController::scanUdpPorts(const vector<string> &ipAddresses) const {
        if(!mCommandLineOptions.mUdpPorts.empty()) {
            UdpScanner udpScanner(ipAddresses, mInterfaceInfo, mCommandLineOptions.mWaitTimeout);
            udpScanner.scanPorts(mCommandLineOptions.mUdpPorts);
        }
    } // ScannerController::scanUdpPorts()
} // OmegaL4Scanner::Facades

/*** end of file ScannerController.cpp ***/
