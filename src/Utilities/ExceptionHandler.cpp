/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ExceptionHandler.cpp                                          *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      13.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the ExceptionHandler class, which is        *
 *               responsible for printing error messages.                      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ExceptionHandler.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the ExceptionHandler class.
 */

#include "Exceptions/OmegaExceptions.hpp"
#include "Utilities/ExceptionHandler.hpp"
#include "Constants/ColorEscapeSequences.hpp"
#include <exception> // std::exception
#include <iostream>  // std::cerr
#include <string>    // std::string

using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Constants;
using namespace std;

namespace OmegaL4Scanner::Utilities
{
    void ExceptionHandler::handleError(const exception &exception) {
        // Attempt to cast the original exception to OmegaBaseException
        const OmegaBaseException *pOmegaException = getOmegaException(exception);

        // Create an UnknownErrorException (only used if dynamic_cast above failed)
        const UknownErrorException unknownException{exception.what()};

        // If dynamic_cast above failed, use the address of unknownException
        if(!pOmegaException) {
            pOmegaException = &unknownException;
        }

        // Now we are 100% sure that pOmegaException points to OmegaBaseException
        printError(*pOmegaException);
        terminateProgram(pOmegaException->code());
    } // handleError()

    void ExceptionHandler::printError(const OmegaBaseException &exception) {
        cerr << COLOR_RED << "Error " << exception.code() << ": " << exception.what() << RESET << endl;
        if(!exception.detail().empty()) {
            cerr << COLOR_YELLOW << "Detail: " << exception.detail() << RESET << endl;
        }
    } // printError()

    void ExceptionHandler::terminateProgram(const int errorCode) {
        exit(errorCode);
    } // terminateProgram()

    const OmegaBaseException *ExceptionHandler::getOmegaException(const exception &exception) {
        return dynamic_cast<const OmegaBaseException*>(&exception);
    } // getOmegaException()
} // OmegaL4Scanner::Exceptions

/*** end of file ExceptionHandler.cpp ***/
