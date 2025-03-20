/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ErrorHandlerTests.cpp                                         *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      13.03.2025                                                    *
 * Last edit:    13.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains unit tests for the ExceptionHandler class. *
 *               The tests verify that the ExceptionHandler correctly handles  *
 *               various exceptions by checking the error messages and exit    *
 *               codes.                                                        *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ErrorHandlerTests.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Unit tests for the ErrorHandler class.
 */

#include "Exceptions/OmegaExceptions.hpp"
#include "Utilities/ExceptionHandler.hpp"
#include "Enums/ExitCodes.hpp"
#include <gtest/gtest.h>
#include <exception> // std::exception
#include <string>    // std::string

using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Enums;
using namespace testing;
using namespace std;

TEST(ErrorHandlerTests, HandleInvalidArgumentException) {
    try {
        throw InvalidArgumentException("This is an invalid argument detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::INVALID_ARGUMENT_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleInvalidInterfaceException) {
    try {
        throw InvalidInterfaceException("This is an invalid interface detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::INVALID_INTERFACE_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleInvalidPortRangeException) {
    try {
        throw InvalidPortRangeException("This is an invalid port range detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::INVALID_PORT_RANGE_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleHostnameException) {
    try {
        throw HostnameException("This is an invalid hostname detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::INVALID_HOSTNAME_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleSocketException) {
    try {
        throw SocketException("This is a socket error detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::SOCKET_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandlePcapException) {
    try {
        throw PcapException("This is a Pcap error detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::PCAP_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleUserInterruptionException) {
    try {
        throw UserInterruptionException("This is a user interruption detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::USER_INTERRUPTION_ERROR)), "");
    }
}

TEST(ErrorHandlerTests, HandleInternalErrorException) {
    try {
        throw InternalErrorException("This is an internal error detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ExceptionHandler::handleError(e), ExitedWithCode(static_cast<int>(ExitCodes::INTERNAL_ERROR)), "");
    }
}

/*** end of file ErrorHandlerTests.cpp ***/
