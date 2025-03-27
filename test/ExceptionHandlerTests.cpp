/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ExceptionHandlerTests.cpp                                      *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      13.03.2025                                                    *
 * Last edit:    27.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains unit tests for the ExceptionHandler class. *
 *               The tests verify that the ExceptionHandler correctly handles  *
 *               various exceptions by checking the error messages and exit    *
 *               codes.                                                        *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ExceptionHandlerTests.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Unit tests for the ExceptionHandler class.
 */

#include "Exceptions/OmegaExceptions.hpp"
#include "Utilities/ExceptionHandler.hpp"
#include "Enums/ExitCodes.hpp"
#include <gtest/gtest.h>
#include <exception> // std::exception
#include <string>    // std::string

using namespace OmegaL4Scanner::Utilities;
using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Enums;
using namespace testing;
using namespace std;

TEST(ErrorHandlerTests, HandleHelpRequestedException) {
    try {
        // Act
        throw HelpRequestedException("Help me please, good sir.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::SUCCESS)), "");
    }
}

TEST(ErrorHandlerTests, HandleInterfacePrintRequestedException) {
    try {
        // Act
        throw InterfacePrintRequestedException("This is a request for printing interfaces.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::SUCCESS)), "");
    }
}

TEST(ErrorHandlerTests, HandleInvalidArgumentException) {
    try {
        // Act
        throw InvalidArgumentException("This is an invalid argument detail.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::INVALID_ARGUMENT_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleInvalidInterfaceException) {
    try {
        // Act
        throw InterfaceErrorException("This is an invalid interface detail.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::INTERFACE_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleHostnameResolutionException) {
    try {
        // Act
        throw HostnameResolutionErrorException("This is a hostname resolution error detail.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::HOSTNAME_RESOLUTION_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleSocketException) {
    try {
        // Act
        throw SocketErrorException("This is a socket error detail.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::SOCKET_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleLibnetException) {
    try {
        // Act
        throw LibnetErrorException("This is a Libnet error detail.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::LIBNET_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleUserInterruptionException) {
    try {
        // Act
        throw UserInterruptionException("This is a user interruption detail.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::USER_INTERRUPTION_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleInternalErrorException) {
    try {
        // Act
        throw InternalErrorException("This is an internal error detail.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::INTERNAL_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleUnknownException) {
    try {
        // Act
        throw runtime_error("This was a runtime error.");
    }
    catch(const exception &e) {
        // Assert
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::UNKNOWN_ERROR)), "");
    }
}

/*** end of file ExceptionHandlerTests.cpp ***/
