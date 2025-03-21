/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaExceptionsTests.cpp                                      *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      13.03.2025                                                    *
 * Last edit:    13.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains unit tests for the OmegaExceptions classes *
 *               using the Google Test framework.                              *
 *                                                                             *
 ******************************************************************************/
/**
 * @file OmegaExceptionsTests.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Unit tests for the OmegaExceptions classes.
 */

#include "Exceptions/OmegaExceptions.hpp"
#include "Utilities/ExceptionHandler.hpp"
#include "Constants/ExceptionMessages.hpp"
#include "Enums/ExitCodes.hpp"
#include <gtest/gtest.h>

using namespace OmegaL4Scanner::Exceptions;
using namespace OmegaL4Scanner::Constants;
using namespace OmegaL4Scanner::Enums;
using namespace testing;
using namespace std;

TEST(OmegaExceptionsTests, ThrowHelpRequestedException) {
    try {
        throw HelpRequestedException("Help me please, good sir.");
    }
    catch(const HelpRequestedException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::SUCESS));
        EXPECT_STREQ(e.what(), helpRequestedMsg);
        EXPECT_STREQ(e.detail().c_str(), "Help me please, good sir.");
    }
}

TEST(OmegaExceptionsTests, ThrowInterfaceReqestedException) {
    try {
        throw InterfacePrintRequestedException("This is a request for printing interfaces.");
    }
    catch(const InterfacePrintRequestedException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::SUCESS));
        EXPECT_STREQ(e.what(), interfacePrintRequestedMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a request for printing interfaces.");
    }
}

TEST(OmegaExceptionsTests, ThrowInvalidArgumentException) {
    try {
        throw InvalidArgumentException("This is an invalid argument detail.");
    }
    catch(const InvalidArgumentException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INVALID_ARGUMENT_ERROR));
        EXPECT_STREQ(e.what(), invalidArgumentErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid argument detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInvalidInterfaceException) {
    try {
        throw InterfaceErrorException("This is an invalid interface detail.");
    }
    catch(const InterfaceErrorException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INTERFACE_ERROR));
        EXPECT_STREQ(e.what(), interfaceErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid interface detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowSocketException) {
    try {
        throw SocketErrorException("This is a socket error detail.");
    }
    catch(const SocketErrorException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::SOCKET_ERROR));
        EXPECT_STREQ(e.what(), socketErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a socket error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowPcapException) {
    try {
        throw PcapErrorException("This is a Pcap error detail.");
    }
    catch(const PcapErrorException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::PCAP_ERROR));
        EXPECT_STREQ(e.what(), pcapErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a Pcap error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInterruptedException) {
    try {
        throw UserInterruptionException("This is a user interruption detail.");
    }
    catch(const UserInterruptionException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::USER_INTERRUPTION_ERROR));
        EXPECT_STREQ(e.what(), userInterruptionMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a user interruption detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInternalErrorException) {
    try {
        throw InternalErrorException("This is an internal error detail.");
    }
    catch(const InternalErrorException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INTERNAL_ERROR));
        EXPECT_STREQ(e.what(), internalErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an internal error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowUknownErrorException) {
    try {
        throw bad_alloc();
    }
    catch(const exception &badAllocException) {
        try {
            throw UknownErrorException(badAllocException.what());
        }
        catch(const UknownErrorException &e) {
            EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::UNKNOWN_ERROR));
            EXPECT_STREQ(e.what(), unknownErrorMsg);
            EXPECT_STREQ(e.detail().c_str(), badAllocException.what());
        }
    }
}

/*** end of file OmegaExceptionsTests.cpp ***/
