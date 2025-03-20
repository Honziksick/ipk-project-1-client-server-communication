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
    catch(HelpRequestedException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::HELP_REQUESTED));
        EXPECT_STREQ(e.what(), helpRequestedMsg);
        EXPECT_STREQ(e.detail().c_str(), "Help me please, good sir.");
    }
}

TEST(OmegaExceptionsTests, ThrowInterfaceReqestedException) {
    try {
        throw InterfacePrintRequestedException("This is a request for printing interfaces.");
    }
    catch(InterfacePrintRequestedException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INTERFACE_PRINT_REQUESTED));
        EXPECT_STREQ(e.what(), interfacePrintRequestedMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a request for printing interfaces.");
    }
}

TEST(OmegaExceptionsTests, ThrowInvalidArgumentException) {
    try {
        throw InvalidArgumentException("This is an invalid argument detail.");
    }
    catch(InvalidArgumentException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INVALID_ARGUMENT_ERROR));
        EXPECT_STREQ(e.what(), invalidArgumentErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid argument detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInvalidInterfaceException) {
    try {
        throw InvalidInterfaceException("This is an invalid interface detail.");
    }
    catch(InvalidInterfaceException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INVALID_INTERFACE_ERROR));
        EXPECT_STREQ(e.what(), invalidInterfaceErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid interface detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInvalidPortRangeException) {
    try {
        throw InvalidPortRangeException("This is an invalid port range detail.");
    }
    catch(InvalidPortRangeException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INVALID_PORT_RANGE_ERROR));
        EXPECT_STREQ(e.what(), invalidPortRangeErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid port range detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowHostResolutionException) {
    try {
        throw HostnameException("This is an invalid hostname detail.");
    }
    catch(HostnameException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INVALID_HOSTNAME_ERROR));
        EXPECT_STREQ(e.what(), invalidHostnameErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid hostname detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowSocketException) {
    try {
        throw SocketException("This is a socket error detail.");
    }
    catch(SocketException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::SOCKET_ERROR));
        EXPECT_STREQ(e.what(), socketErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a socket error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowPcapException) {
    try {
        throw PcapException("This is a Pcap error detail.");
    }
    catch(PcapException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::PCAP_ERROR));
        EXPECT_STREQ(e.what(), pcapErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a Pcap error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInterruptedException) {
    try {
        throw UserInterruptionException("This is a user interruption detail.");
    }
    catch(UserInterruptionException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::USER_INTERRUPTION_ERROR));
        EXPECT_STREQ(e.what(), userInterruptionErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a user interruption detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInternalErrorException) {
    try {
        throw InternalErrorException("This is an internal error detail.");
    }
    catch(InternalErrorException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INTERNAL_ERROR));
        EXPECT_STREQ(e.what(), internalErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an internal error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowUknownErrorException) {
    try {
        throw bad_alloc();
    }
    catch(std::exception &badAllocException) {
        try {
            throw UknownErrorException(badAllocException.what());
        }
        catch(UknownErrorException &e) {
            EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::UNKNOWN_ERROR));
            EXPECT_STREQ(e.what(), badAllocException.what());
            EXPECT_STREQ(e.detail().c_str(), "");
        }
    }
}

/*** end of file OmegaExceptionsTests.cpp ***/
