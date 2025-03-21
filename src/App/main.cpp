/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         main.cpp                                                      *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    21.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the main function that serves as the entry *
 *               point for the OMEGA L4 Scanner application. It initializes    *
 *               the application and starts the scanning process.              *
 *                                                                             *
 ******************************************************************************/
/**
 * @file main.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Main entry point for the OMEGA L4 Scanner application.
 */

#include "Facades/OmegaAppFacade.hpp"
#include "Utilities/ExceptionHandler.hpp"
#include <exception>  // std::exception

using namespace OmegaL4Scanner;
using namespace std;

int main(const int argc, char *argv[]) {
    try {
        Facades::OmegaAppFacade appFacade;
        appFacade.runScan(argc, argv);
    }
    catch(const exception &e) {
        Utilities::ExceptionHandler::handleError(e);
    }

    return EXIT_SUCCESS;
} // main()

/*** end of file main.cpp ***/
