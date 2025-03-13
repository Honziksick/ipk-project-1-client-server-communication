/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaExceptionsTest.cpp                                       *
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
 * @file OmegaExceptionsTest.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Unit tests for the OmegaExceptions classes.
 */

#include "Errors/OmegaExceptions.hpp"
#include "Errors/ErrorHandler.hpp"
#include "Errors/ErrorMessage.hpp"
#include "Enums/ErrorCode.hpp"
#include <gtest/gtest.h>

using namespace OmegaL4Scanner::Errors;
using namespace OmegaL4Scanner::Enums;
using namespace testing;
using namespace std;

TEST(OmegaExceptionsTest, ThrowInvalidArgumentException) {
    try {
        throw InvalidArgumentException("This is an invalid argument detail.");
    }
    catch(InvalidArgumentException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::INVALID_ARGUMENT_ERROR));
        EXPECT_STREQ(e.what(), invalidArgumentErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid argument detail.");
    }
}

TEST(OmegaExceptionsTest, ThrowInvalidInterfaceException) {
    try {
        throw InvalidInterfaceException("This is an invalid interface detail.");
    }
    catch(InvalidInterfaceException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::INVALID_INTERFACE_ERROR));
        EXPECT_STREQ(e.what(), invalidInterfaceErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid interface detail.");
    }
}

TEST(OmegaExceptionsTest, ThrowInvalidPortRangeException) {
    try {
        throw InvalidPortRangeException("This is an invalid port range detail.");
    }
    catch(InvalidPortRangeException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::INVALID_PORT_RANGE_ERROR));
        EXPECT_STREQ(e.what(), invalidPortRangeErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid port range detail.");
    }
}

TEST(OmegaExceptionsTest, ThrowHostResolutionException) {
    try {
        throw HostnameException("This is an invalid hostname detail.");
    }
    catch(HostnameException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::INVALID_HOSTNAME_ERROR));
        EXPECT_STREQ(e.what(), invalidHostnameErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid hostname detail.");
    }
}

TEST(OmegaExceptionsTest, ThrowSocketException) {
    try {
        throw SocketException("This is a socket error detail.");
    }
    catch(SocketException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::SOCKET_ERROR));
        EXPECT_STREQ(e.what(), socketErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a socket error detail.");
    }
}

TEST(OmegaExceptionsTest, ThrowPcapException) {
    try {
        throw PcapException("This is a Pcap error detail.");
    }
    catch(PcapException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::PCAP_ERROR));
        EXPECT_STREQ(e.what(), pcapErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a Pcap error detail.");
    }
}

TEST(OmegaExceptionsTest, ThrowInterruptedException) {
    try {
        throw UserInterruptionException("This is a user interruption detail.");
    }
    catch(UserInterruptionException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::USER_INTERRUPTION_ERROR));
        EXPECT_STREQ(e.what(), userInterruptionErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a user interruption detail.");
    }
}

TEST(OmegaExceptionsTest, ThrowInternalErrorException) {
    try {
        throw InternalErrorException("This is an internal error detail.");
    }
    catch(InternalErrorException &e) {
        EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::INTERNAL_ERROR));
        EXPECT_STREQ(e.what(), internalErrMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an internal error detail.");
    }
}

TEST(OmegaExceptionsTest, ThrowUknownErrorException) {
    try {
        throw bad_alloc();
    }
    catch(std::exception &badAllocException) {
        try {
            UknownErrorException unknownException{badAllocException.what()};
            throw unknownException;
        }
        catch(UknownErrorException &e) {
            EXPECT_EQ(e.code(), static_cast<int>(ErrorCode::UNKNOWN_ERROR));
            EXPECT_STREQ(e.what(), badAllocException.what());
            EXPECT_STREQ(e.detail().c_str(), "");
        }
    }
}

/*** end of file OmegaExceptionsTest.cpp ***/
