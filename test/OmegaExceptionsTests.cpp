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
 * Last edit:    24.03.2025                                                    *
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
        // Act
        throw HelpRequestedException("Help me please, good sir.");
    }
    catch(const HelpRequestedException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::SUCCESS));
        EXPECT_STREQ(e.what(), helpRequestedMsg);
        EXPECT_STREQ(e.detail().c_str(), "Help me please, good sir.");
    }
}

TEST(OmegaExceptionsTests, ThrowInterfaceReqestedException) {
    try {
        // Act
        throw InterfacePrintRequestedException("This is a request for printing interfaces.");
    }
    catch(const InterfacePrintRequestedException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::SUCCESS));
        EXPECT_STREQ(e.what(), interfacePrintRequestedMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a request for printing interfaces.");
    }
}

TEST(OmegaExceptionsTests, ThrowInvalidArgumentException) {
    try {
        // Act
        throw InvalidArgumentException("This is an invalid argument detail.");
    }
    catch(const InvalidArgumentException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INVALID_ARGUMENT_ERROR));
        EXPECT_STREQ(e.what(), invalidArgumentErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid argument detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInvalidInterfaceException) {
    try {
        // Act
        throw InterfaceErrorException("This is an invalid interface detail.");
    }
    catch(const InterfaceErrorException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INTERFACE_ERROR));
        EXPECT_STREQ(e.what(), interfaceErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an invalid interface detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowHostnameResolutionException) {
    try {
        // Act
        throw HostnameResolutionErrorException("This is a hostname resolution error detail.");
    }
    catch(const HostnameResolutionErrorException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::HOSTNAME_RESOLUTION_ERROR));
        EXPECT_STREQ(e.what(), hostnameResolutionErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a hostname resolution error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowSocketException) {
    try {
        // Act
        throw SocketErrorException("This is a socket error detail.");
    }
    catch(const SocketErrorException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::SOCKET_ERROR));
        EXPECT_STREQ(e.what(), socketErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a socket error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowPcapException) {
    try {
        // Act
        throw LibnetErrorException("This is a Pcap error detail.");
    }
    catch(const LibnetErrorException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::LIBNET_ERROR));
        EXPECT_STREQ(e.what(), libnetErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a Pcap error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInterruptedException) {
    try {
        // Act
        throw UserInterruptionException("This is a user interruption detail.");
    }
    catch(const UserInterruptionException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::USER_INTERRUPTION_ERROR));
        EXPECT_STREQ(e.what(), userInterruptionMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is a user interruption detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowInternalErrorException) {
    try {
        // Act
        throw InternalErrorException("This is an internal error detail.");
    }
    catch(const InternalErrorException &e) {
        // Assert
        EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::INTERNAL_ERROR));
        EXPECT_STREQ(e.what(), internalErrorMsg);
        EXPECT_STREQ(e.detail().c_str(), "This is an internal error detail.");
    }
}

TEST(OmegaExceptionsTests, ThrowUknownErrorException) {
    try {
        // Act (part 1)
        throw bad_alloc();
    }
    catch(const exception &badAllocException) {
        try {
            // Act (part 2)
            throw UknownErrorException(badAllocException.what());
        }
        catch(const UknownErrorException &e) {
            // Assert
            EXPECT_EQ(e.code(), static_cast<int>(ExitCodes::UNKNOWN_ERROR));
            EXPECT_STREQ(e.what(), unknownErrorMsg);
            EXPECT_STREQ(e.detail().c_str(), badAllocException.what());
        }
    }
}

/*** end of file OmegaExceptionsTests.cpp ***/
