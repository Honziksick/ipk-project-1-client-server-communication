/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ExceptionHandler.hpp                                          *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      13.03.2025                                                    *
 * Last edit:    13.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the ExceptionHandler class, which is           *
 *               responsible for printing error messages.                      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ExceptionHandler.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the ExceptionHandler class.
 */

#ifndef EXCEPTION_HANDLER_HPP
#define EXCEPTION_HANDLER_HPP

#include "Exceptions/OmegaBaseException.hpp"
#include <exception> // std::exception

namespace OmegaL4Scanner::Utilities
{
    /**
     * @class ExceptionHandler
     * @brief Class responsible for handling and printing error messages.
     */
    class ExceptionHandler {
    public:
        /**
         * @brief Handles the given exception by printing the error message
         *        and terminating the program with error code.
         * @param exception The exception to handle.
         */
        static void handleError(const std::exception &exception);

    private:
        /**
         * @brief Prints the given error message.
         * @param exception The exception containing the error message to print.
         */
        static void printError(const Exceptions::OmegaBaseException &exception);

        /**
         * @brief Terminates the program with the given error code.
         * @param errorCode The error code to terminate the program with.
         */
        static void terminateProgram(int errorCode);

        /**
         * @brief Retrieves the OmegaBaseException from an exception.
         *
         * @details This method attempts to cast a standard exception to an
         *          OmegaBaseException. If the cast is successful, it returns
         *          a pointer to the OmegaBaseException. Otherwise, it returns
         *          nullptr.
         *
         * @param exception The standard exception to cast.
         * @return Pointer to the OmegaBaseException if cast is successful,
         *         nullptr otherwise.
         */
        static const Exceptions::OmegaBaseException *getOmegaException(const std::exception &exception);
    }; // ExceptionHandler
} // OmegaL4Scanner::Utilities

#endif // EXCEPTION_HANDLER_HPP

/*** end of file ExceptionHandler.hpp ***/
