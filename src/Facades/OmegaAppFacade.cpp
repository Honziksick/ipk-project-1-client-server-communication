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
 * Last edit:    22.03.2025                                                    *
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
#include "Facades/ScannerController.hpp"
#include "Common/ArgumentParser.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include "Networking/InterfaceManager.hpp"
#include "Utilities/ActiveInterfacePrinter.hpp"
#include "Utilities/ExceptionHandler.hpp"
#include "Utilities/SignalHandler.hpp"
#include <exception>  // std::exception
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
            SignalHandler::registerHandlers(); // handles SIGINT signal

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

            const ScannerController scannerController(mCommandLineOptions, mInterfaceInfo);
            scannerController.scanL4Layer();
        }
        catch(const exception &e) {
            ExceptionHandler::handleError(e);
        }
    } // OmegaAppFacade::runScan()

    void OmegaAppFacade::getCommandLineOptions(const int argc, char *argv[]) {
        mCommandLineOptions = ArgumentParser::parseArguments(argc, argv);
    } // OmegaAppFacade::getCommandLineOptions()

    void OmegaAppFacade::getInterfaceInfo() {
        mInterfaceInfo = InterfaceManager::getInterfaceByName(mCommandLineOptions.mInterfaceName);
    } // OmegaAppFacade::getInterfaceInfo()
} // OmegaL4Scanner::Facades

/*** end of file OmegaAppFacade.cpp ***/
