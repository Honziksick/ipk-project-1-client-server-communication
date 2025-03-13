/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ErrorHandler.cpp                                              *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      13.03.2025                                                    *
 * Last edit:    13.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the ErrorHandler class, which is            *
 *               responsible for printing error messages.                      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ErrorHandler.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the ErrorHandler class.
 */

#include "ErrorHandler.hpp"
#include "OmegaBaseException.hpp"
#include <exception> // exception
#include <iostream>  // cerr
#include <string>    // string

#include "OmegaExceptions.hpp"

using namespace std;

const string COLOR_RESET  = "\033[0m";    /**< ANSI escape code for resetting the color. */
const string COLOR_YELLOW = "\033[0;33m"; /**< ANSI escape code for yellow color.        */
const string COLOR_RED    = "\033[0;31m"; /**< ANSI escape code for red color.           */

namespace OmegaL4Scanner::Errors
{
    void ErrorHandler::handleError(const exception &exception) {
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

    void ErrorHandler::printError(const OmegaBaseException &exception) {
        cerr << COLOR_RED << "Error " << exception.code() << ": " << exception.what() << COLOR_RESET << endl;
        if (!exception.detail().empty()){
            cerr << COLOR_YELLOW << "Detail: " << exception.detail() << COLOR_RESET << endl;
        }
    } // printError()

    void ErrorHandler::terminateProgram(const int errorCode) {
        exit(errorCode);
    } // terminateProgram()

    const OmegaBaseException *ErrorHandler::getOmegaException(const exception &exception) {
        return dynamic_cast<const OmegaBaseException*>(&exception);
    } // getOmegaException()
} // OmegaL4Scanner::Errors

/*** end of file ErrorHandler.cpp ***/
