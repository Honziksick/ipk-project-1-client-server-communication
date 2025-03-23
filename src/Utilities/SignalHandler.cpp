/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         SignalHandler.cpp                                             *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      22.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the SignalHandler class, which is           *
 *               responsible for handling system signals.                      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file SignalHandler.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the SignalHandler class.
 */

#include "Utilities/SignalHandler.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include <csignal> // signal

using namespace OmegaL4Scanner::Exceptions;

namespace OmegaL4Scanner::Utilities
{
    void SignalHandler::registerHandlers() {
        signal(SIGINT, handleSignal);
    } // SignalHandler::registerHandlers()

    void SignalHandler::handleSignal(const int signal) {
        if(signal == SIGINT) {
            throw UserInterruptionException("SIGINT: User interrupted the program.");
        }
    } // SignalHandler::handleSignal()
} // OmegaL4Scanner::Utilities

/*** end of file SignalHandler.cpp ***/
