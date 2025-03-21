/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaAppFacade.cpp                                            *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    21.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the implementation of the OmegaAppFacade   *
 *               class, which serves as a facade for the OMEGA L4 Scanner      *
 *               application. The facade pattern is used to provide a          *
 *               simplified interface to a complex subsystem.                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file OmegaAppFacade.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the OmegaAppFacade class.
 */

#include "Facades/OmegaAppFacade.hpp"
#include "Common/ArgumentParser.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include "Networking/InterfaceManager.hpp"
#include "Utilities/ActiveInterfacePrinter.hpp"
#include "Utilities/ExceptionHandler.hpp"
#include <exception>  // std::exception
#include <iostream>
#include <cstdlib>    // std::exit

using namespace OmegaL4Scanner::Common;
using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Utilities;
using namespace std;

namespace OmegaL4Scanner::Facades
{
    OmegaAppFacade::OmegaAppFacade() = default;

    void OmegaAppFacade::runScan(const int argc, char *argv[]) {
        try {
            try {
                getCommandLineOptions(argc, argv);
            }
            catch(const HelpRequestedException &) {
                exit(EXIT_SUCCESS);
            }
            catch(const InterfacePrintRequestedException &) {
                ActiveInterfacePrinter::printActiveInterfaces();
                exit(EXIT_SUCCESS);
            }

            getInterfaceInfo();

            scanL4Layer();
        }
        catch(const exception &e) {
            ExceptionHandler::handleError(e);
        }
    } // OmegaAppFacade::runScan

    void OmegaAppFacade::getCommandLineOptions(const int argc, char *argv[]) {
        mCommandLineOptions = ArgumentParser::parseArguments(argc, argv);
    } // OmegaAppFacade::getCommandLineOptions

    void OmegaAppFacade::getInterfaceInfo() {
        mInterfaceInfo = InterfaceManager::getInterfaceByName(mCommandLineOptions.mInterfaceName);
    } // OmegaAppFacade::getInterfaceInfo

    void OmegaAppFacade::scanL4Layer() const {
        // TODO: Replace this simulation with actual scanning logic.
        cout << "Simulated scan began..." << endl;
        cout << endl;
        cout << "Starting scan for target: " << mCommandLineOptions.mTarget << " on interface: " << mInterfaceInfo.mName << endl;
        cout << "TCP Ports to scan: ";
        for (const auto &port : mCommandLineOptions.mTcpPorts) {
            if (std::holds_alternative<int>(port)) {
                cout << std::get<int>(port) << " ";
            } else {
                auto [start, end] = std::get<std::pair<int, int>>(port);
                cout << start << "-" << end << " ";
            }
        }
        cout << endl;
        cout << "UDP Ports to scan: ";
        for (const auto &port : mCommandLineOptions.mUdpPorts) {
            if (std::holds_alternative<int>(port)) {
                cout << std::get<int>(port) << " ";
            } else {
                auto [start, end] = std::get<std::pair<int, int>>(port);
                cout << start << "-" << end << " ";
            }
        }
        cout << endl;
        cout << "Wait timeout: " << mCommandLineOptions.mWaitTimeout.count() << " milliseconds" << endl;
        cout << endl;
        cout << "Simulated scan completed!" << endl;
    } // OmegaAppFacade::scanL4Layer
} // OmegaL4Scanner::Facades


/*** end of file OmegaAppFacade.cpp ***/
