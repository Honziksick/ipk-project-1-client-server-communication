/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ErrorHandlerTest.cpp                                          *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      13.03.2025                                                    *
 * Last edit:    13.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains unit tests for the ErrorHandler class.     *
 *               The tests verify that the ErrorHandler correctly handles      *
 *               various exceptions by checking the error messages and exit    *
 *               codes.                                                        *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ErrorHandlerTest.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Unit tests for the ErrorHandler class.
 */

#include "Errors/OmegaExceptions.hpp"
#include "Errors/ErrorHandler.hpp"
#include "Enums/ErrorCode.hpp"
#include <gtest/gtest.h>
#include <exception> // std::exception
#include <string>    // std::string

using namespace OmegaL4Scanner::Errors;
using namespace OmegaL4Scanner::Enums;
using namespace testing;
using namespace std;

TEST(ErrorHandlerTest, HandleInvalidArgumentException) {
    try {
        throw InvalidArgumentException("This is an invalid argument detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ErrorHandler::handleError(e), ExitedWithCode(static_cast<int>(ErrorCode::INVALID_ARGUMENT_ERROR)), "");
    }
}

TEST(ErrorHandlerTest, HandleInvalidInterfaceException) {
    try {
        throw InvalidInterfaceException("This is an invalid interface detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ErrorHandler::handleError(e), ExitedWithCode(static_cast<int>(ErrorCode::INVALID_INTERFACE_ERROR)), "");
    }
}

TEST(ErrorHandlerTest, HandleInvalidPortRangeException) {
    try {
        throw InvalidPortRangeException("This is an invalid port range detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ErrorHandler::handleError(e), ExitedWithCode(static_cast<int>(ErrorCode::INVALID_PORT_RANGE_ERROR)), "");
    }
}

TEST(ErrorHandlerTest, HandleHostnameException) {
    try {
        throw HostnameException("This is an invalid hostname detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ErrorHandler::handleError(e), ExitedWithCode(static_cast<int>(ErrorCode::INVALID_HOSTNAME_ERROR)), "");
    }
}

TEST(ErrorHandlerTest, HandleSocketException) {
    try {
        throw SocketException("This is a socket error detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ErrorHandler::handleError(e), ExitedWithCode(static_cast<int>(ErrorCode::SOCKET_ERROR)), "");
    }
}

TEST(ErrorHandlerTest, HandlePcapException) {
    try {
        throw PcapException("This is a Pcap error detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ErrorHandler::handleError(e), ExitedWithCode(static_cast<int>(ErrorCode::PCAP_ERROR)), "");
    }
}

TEST(ErrorHandlerTest, HandleUserInterruptionException) {
    try {
        throw UserInterruptionException("This is a user interruption detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ErrorHandler::handleError(e), ExitedWithCode(static_cast<int>(ErrorCode::USER_INTERRUPTION_ERROR)), "");
    }
}

TEST(ErrorHandlerTest, HandleInternalErrorException) {
    try {
        throw InternalErrorException("This is an internal error detail.");
    }
    catch(const exception &e) {
        EXPECT_EXIT(ErrorHandler::handleError(e), ExitedWithCode(static_cast<int>(ErrorCode::INTERNAL_ERROR)), "");
    }
}

/*** end of file ErrorHandlerTest.cpp ***/
