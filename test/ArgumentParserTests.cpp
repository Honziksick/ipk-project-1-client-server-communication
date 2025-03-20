/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ArgumentParserTests.cpp                                       *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      15.03.2025                                                    *
 * Last edit:    19.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains unit tests for the ArgumentParser          *
 *               class, which is responsible for parsing command line          *
 *               arguments for the OMEGA L4 Scanner project.                   *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ArgumentParserTests.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Unit tests for the ArgumentParser class.
 */

#include "Common/ArgumentParser.hpp"
#include "Common/CommandLineOptions.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include <gtest/gtest.h>
#include <chrono>
#include <vector>
#include <string>
#include <variant>
#include <utility>

using namespace OmegaL4Scanner::Common;
using namespace OmegaL4Scanner::Exceptions;
using namespace testing;
using namespace std;


/*******************************************************************************
 *                                                                             *
 *                          ArgumentParser TEST UTILS                          *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Creates a STDIN *argv[] array from a vector of strings.
 *
 * @param args A vector of C-style strings representing command line arguments.
 * @return A vector of char* representing the argv array.
 */
vector<char*> createArgv(const vector<string> &args) {
    vector<char*> argv;
    argv.push_back(const_cast<char*>("ipk-l4-scan"));

    for(const auto &arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }

    argv.push_back(nullptr);
    return argv;
}

/**
 * @brief Retrieves the port range from a PortRange variant.
 *
 * @param portPair A reference to a PortRange variant containing a port range.
 * @return pair<int, int> A reference to a pair of integers representing the
 *         port range.
 */
inline pair<int, int> &getPortRange(PortRange &portPair) {
    return get<pair<int, int>>(portPair);
}

/**
 * @brief Retrieves a single port from a PortRange variant.
 *
 * @param port A reference to a PortRange variant containing a single port.
 * @return int An integer representing the port.
 */
inline int getPort(const PortRange &port) {
    return get<int>(port);
}


/*******************************************************************************
 *                                                                             *
 *                                 UNIT TESTS                                  *
 *                                                                             *
 ******************************************************************************/

TEST(ArgumentParserTests, HelpShort) {
    // Arrange
    const vector<string> args = {"-h"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), HelpRequestedException);
}

TEST(ArgumentParserTests, HelpLong) {
    // Arrange
    const vector<string> args = {"--help"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), HelpRequestedException);
}

TEST(ArgumentParserTests, HelpSwitchWithOtherSwitches1) {
    // Arrange
    const vector<string> args = {"-h", "-i", "eth0", "-u", "38", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), HelpRequestedException);
}

TEST(ArgumentParserTests, HelpSwitchWithOtherSwitches2) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "example.com", "--help"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), HelpRequestedException);
}

TEST(ArgumentParserTests, HelpSwitchWithOtherSwitches3) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "-h", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), HelpRequestedException);
}

TEST(ArgumentParserTests, PrintInterfaces_NoArgumentsPassed) {
    // Arrange
    constexpr vector<string> args = {};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InterfacePrintRequestedException);
}

TEST(ArgumentParserTests, PrintInterfaces_InterfaceNotSpecified1) {
    // Arrange
    const vector<string> args = {"-i"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InterfacePrintRequestedException);
}

TEST(ArgumentParserTests, PrintInterfaces_InterfaceNotSpecified2) {
    // Arrange
    const vector<string> args = {"--interface"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InterfacePrintRequestedException);
}

TEST(ArgumentParserTests, Ports_OnlyUdpPorts1) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "53", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mTcpPorts.empty());
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 53);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_OnlyUdpPorts2) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "53,45,49825", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mTcpPorts.empty());
    ASSERT_EQ(options.mUdpPorts.size(), 3u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 45);
    EXPECT_EQ(getPort(options.mUdpPorts[1]), 53);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 49825);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_OnlyUdpPorts3) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "50-89", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mTcpPorts.empty());
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPortRange(options.mUdpPorts[0]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[0]).second, 89);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_OnlyUdpPorts4) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38,800,50-89,999,1024", "-w", "42", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mTcpPorts.empty());
    ASSERT_EQ(options.mUdpPorts.size(), 5u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);
    EXPECT_EQ(getPort(options.mUdpPorts[4]), 1024);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(42));
}

TEST(ArgumentParserTests, Ports_OnlyTcpPorts1) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "30587", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mUdpPorts.empty());
    ASSERT_EQ(options.mTcpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 30587);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_OnlyTcpPorts2) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "58,10,1", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mUdpPorts.empty());
    ASSERT_EQ(options.mTcpPorts.size(), 3u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 1);
    EXPECT_EQ(getPort(options.mTcpPorts[1]), 10);
    EXPECT_EQ(getPort(options.mTcpPorts[2]), 58);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_OnlyTcpPorts3) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "658-50245", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mUdpPorts.empty());
    ASSERT_EQ(options.mTcpPorts.size(), 1u);
    EXPECT_EQ(getPortRange(options.mTcpPorts[0]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[0]).second, 50245);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_OnlyTcpPorts4) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "38,800,50-89,999", "-w", "42", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mUdpPorts.empty());
    ASSERT_EQ(options.mTcpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mTcpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mTcpPorts[3]), 999);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(42));
}

TEST(ArgumentParserTests, Ports_BothPortTypes1) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "53", "-t", "30587", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 53);
    ASSERT_EQ(options.mTcpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 30587);
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_BothPortTypes2) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "53,85,35", "-t", "58,10,1", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 3u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 35);
    EXPECT_EQ(getPort(options.mUdpPorts[1]), 53);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 85);

    ASSERT_EQ(options.mTcpPorts.size(), 3u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 1);
    EXPECT_EQ(getPort(options.mTcpPorts[1]), 10);
    EXPECT_EQ(getPort(options.mTcpPorts[2]), 58);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_BothPortTypes3) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "658-50245", "-u", "50-89", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mTcpPorts.size(), 1u);
    EXPECT_EQ(getPortRange(options.mTcpPorts[0]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[0]).second, 50245);

    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPortRange(options.mUdpPorts[0]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[0]).second, 89);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_BothPortTypes4) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "658-50245,38", "-u", "38,800,50-89,999", "-w", "42", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(42));
}

TEST(ArgumentParserTests, Ports_AdvancedPortEntries) {
    // Arrange
    const vector<string> args = {
        "-i",
        "eth0",
        "-t",
        "80,90-95,85,80-80,100,99-101",
        "-u",
        "53,55-57,56,60",
        "-w",
        "3000",
        "merlin.fit.vutbr.cz"
    };
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_EQ(options.mTarget, "merlin.fit.vutbr.cz");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(3000));

    ASSERT_EQ(options.mTcpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 80);
    EXPECT_EQ(getPort(options.mTcpPorts[1]), 85);
    EXPECT_EQ(getPortRange(options.mTcpPorts[2]).first, 90);
    EXPECT_EQ(getPortRange(options.mTcpPorts[2]).second, 95);
    EXPECT_EQ(getPortRange(options.mTcpPorts[3]).first, 99);
    EXPECT_EQ(getPortRange(options.mTcpPorts[3]).second, 101);

    ASSERT_EQ(options.mUdpPorts.size(), 3u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 53);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 55);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 57);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 60);
}

TEST(ArgumentParserTests, Ports_DuplicatePorts) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "80,80,80", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mTcpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 80);
    EXPECT_TRUE(options.mUdpPorts.empty());
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Ports_DuplicateRanges) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "66-7982,58,66-7982", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 58);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 66);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 7982);
    EXPECT_TRUE(options.mUdpPorts.empty());
    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, PassedInAnyOrder1) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38,800,50-89,999", "-t", "658-50245,38", "-w", "100587", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");

    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder2) {
    // Arrange
    const vector<string> args = {"-t", "658-50245,38", "-u", "38,800,50-89,999", "-w", "100587", "example.com", "-i", "eth0"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder3) {
    // Arrange
    const vector<string> args = {"-t", "658-50245,38", "-i", "eth0", "-u", "38,800,50-89,999", "-w", "100587", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder4) {
    // Arrange
    const vector<string> args = {"-t", "658-50245,38", "-u", "38,800,50-89,999", "-i", "eth0", "-w", "100587", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder5) {
    // Arrange
    const vector<string> args = {"-t", "658-50245,38", "-u", "38,800,50-89,999", "-w", "100587", "-i", "eth0", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder6) {
    // Arrange
    const vector<string> args = {"-t", "658-50245,38", "-i", "eth0", "-u", "38,800,50-89,999", "-w", "100587", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder7) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38,800,50-89,999", "-w", "100587", "-t", "658-50245,38", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder8) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38,800,50-89,999", "-w", "100587", "example.com", "-t", "658-50245,38"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder9) {
    // Arrange
    const vector<string> args = {"-w", "100587", "-i", "eth0", "-t", "658-50245,38", "-u", "38,800,50-89,999", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder10) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "658-50245,38", "-u", "38,800,50-89,999", "example.com", "-w", "100587"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder11) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-w", "100587", "example.com", "-t", "38,800,50-89,999"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mUdpPorts.empty());

    ASSERT_EQ(options.mTcpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mTcpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mTcpPorts[3]), 999);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, PassedInAnyOrder12) {
    // Arrange
    const vector<string> args = {"-u", "38,800,50-89,999", "-i", "eth0", "-w", "100587", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    EXPECT_TRUE(options.mTcpPorts.empty());

    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, ArgumentsLongVersion) {
    // Arrange
    const vector<string> args = {
        "--interface",
        "eth0",
        "--pu",
        "38,800,50-89,999",
        "--pt",
        "658-50245,38",
        "--wait",
        "100587",
        "example.com"
    };
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 4u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).first, 50);
    EXPECT_EQ(getPortRange(options.mUdpPorts[1]).second, 89);
    EXPECT_EQ(getPort(options.mUdpPorts[2]), 800);
    EXPECT_EQ(getPort(options.mUdpPorts[3]), 999);

    ASSERT_EQ(options.mTcpPorts.size(), 2u);
    EXPECT_EQ(getPort(options.mTcpPorts[0]), 38);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).first, 658);
    EXPECT_EQ(getPortRange(options.mTcpPorts[1]).second, 50245);

    EXPECT_EQ(options.mTarget, "example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(100587));
}

TEST(ArgumentParserTests, Host_IPv4_1) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "192.168.1.1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "192.168.1.1");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv4_2) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "8.8.8.8"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "8.8.8.8");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv4_3) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "172.16.0.1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "172.16.0.1");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv4_4) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "10.0.0.1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "10.0.0.1");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv4_5) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "255.255.255.255"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "255.255.255.255");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv4_6) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "127.0.0.1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "127.0.0.1");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv4_7) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "169.254.0.1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "169.254.0.1");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv6_1) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "2001:0db8:85a3:0000:0000:8a2e:0370:7334"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "2001:0db8:85a3:0000:0000:8a2e:0370:7334");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv6_2) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "fe80::1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "fe80::1");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv6_3) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "::1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "::1");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv6_4) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "2001:db8::"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "2001:db8::");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_IPv6_5) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "ff02::1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "ff02::1");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_DomainName_1) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "www.google.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "www.google.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_DomainName_2) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "ftp.example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "ftp.example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_DomainName_3) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "localhost"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "localhost");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_DomainName_4) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "xn--bcher-kva.example"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "xn--bcher-kva.example");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_DomainName_5) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "example.com."};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "example.com.");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, Host_DomainName_6) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act
    EXPECT_NO_THROW(options = ArgumentParser::parseArguments(argc, argv.data()));

    // Assert
    EXPECT_EQ(options.mInterfaceName, "eth0");
    ASSERT_EQ(options.mUdpPorts.size(), 1u);
    EXPECT_EQ(getPort(options.mUdpPorts[0]), 38);
    EXPECT_TRUE(options.mTcpPorts.empty());
    EXPECT_EQ(options.mTarget, "a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.q.r.s.t.u.v.w.x.y.z.example.com");
    EXPECT_EQ(options.mWaitTimeout, chrono::milliseconds(5000));
}

TEST(ArgumentParserTests, MissingInterfaceArgument) {
    // Arrange
    const vector<string> args = {"-u", "38", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, MissingInterfaceValue) {
    // Arrange
    const vector<string> args = {"-i", "-u", "38", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, MissingUdpAndTcpPorts) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, OnlyUdpSwitchWithoutPorts) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, OnlyTcpSwitchWithoutPorts) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, TcpAndUdpSwitchesMissingValues) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-t", "-u", "38", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, MissingDomainOrIpAddress) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, DuplicateSwitches) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-i", "eth1", "-u", "38", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, MissingWaitValue) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "-w", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, NegativeWaitValue) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "-w", "-1000", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, PortRangeStartGreaterThanEnd) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "90-80", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, PortRangeStartNegative) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "-10-80", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, PortRangeEndNegative) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "10--80", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, SinglePortNegative) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "-80", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, OnlyInterfaceGiven) {
    // Arrange
    const vector<string> args = {"-i", "eth0"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, MissingHostname) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "-80"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, MissingPorts) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidDomainName_InvalidDomain) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "invalid_domain"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidDomainName_ExampleDoubleDot) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "example..com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidDomainName_HyphenAtStart) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "-example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidDomainName_HyphenAtEnd) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "example.com-"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidDomainName_SlashInDomain) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "example.com/"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, Host_DomainName_DotAtStart) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", ".example.com"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv4Address1) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "256.256.256.256"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv4Address2) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "192.168.1.256"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv4Address3) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "192.168.1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv4Address4) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "192.168.1.1.1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv4Address5) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "192.168.1.-1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv4Address6) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "80", "0.0.0.0"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv6Address_TripleColon) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "2001:db8:::1"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv6Address_DoubleDoubleColon) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "2001:db8::85a3::7334"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv6Address_TooManySegments) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "2001:db8:85a3:0000:0000:8a2e:0370:7334:1234"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv6Address_EndsWithDoubleColon) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "2001:db8:85a3:0000:0000:8a2e:0370:7334::"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

TEST(ArgumentParserTests, InvalidIPv6Address_InvalidCharacters) {
    // Arrange
    const vector<string> args = {"-i", "eth0", "-u", "38", "2001:db8:85a3:0000:0000:8a2e:0370:7334:gggg"};
    auto argv = createArgv(args);
    const int argc = static_cast<int>(argv.size()) - 1;
    CommandLineOptions options;

    // Act & Assert
    EXPECT_THROW(options = ArgumentParser::parseArguments(argc, argv.data()), InvalidArgumentException);
}

/*** end of file ArgumentParserTests.cpp ***/
