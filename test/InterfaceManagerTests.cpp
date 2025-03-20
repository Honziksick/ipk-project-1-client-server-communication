/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         InterfaceManagerTests.cpp                                     *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains unit tests for the InterfaceManager        *
 *               class. It shows how to properly set up the mock environment   *
 *               for getifaddrs/freeifaddrs and tests all edge cases.          *
 *                                                                             *
 ******************************************************************************/
/**
 * @file InterfaceManagerTests.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Unit tests for the InterfaceManager class.
 */

#include "Networking/InterfaceManager.hpp"
#include "Networking/InterfaceInfo.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include <gtest/gtest.h>
#include <string>       // std::string
#include <vector>       // std:vector
#include <arpa/inet.h>  // sockaddr_in, sockaddr_in6
#include <ifaddrs.h>    // ifaddrs, getifaddrs(), freeifaddrs()
#include <sys/socket.h> // AF_INET, AF_INET6, AF_UNSPEC
#include <net/if.h>     // IFF_UP

using namespace OmegaL4Scanner::Networking;
using namespace OmegaL4Scanner::Exceptions;
using namespace testing;
using namespace std;


/*******************************************************************************
 *                                                                             *
 *         InterfaceManager TEST UTILS - GLOBAL VARIABLES & STRUCTURES         *
 *                                                                             *
 ******************************************************************************/

/**
 * @struct MockInterfaceDef
 * @brief Helper structure for defining a single "mock" interface.
 */
struct MockInterfaceDef {
    std::string mName;          /**< Interface name, e.g. 'eth0'                           */
    unsigned int mFlags = 0;    /**< Interface flags, e.g. IFF_UP                          */
    int mFamily = AF_INET;      /**< Address family, either AF_INET or AF_INET6            */
    std::string mIp;            /**< IP address, e.g. '192.168.0.1' or 'fe80::1234:abcd'   */
    std::string mNetmask;       /**< Netmask, e.g. '255.255.255.0'                         */
    std::string mBroadcast;     /**< Broadcast address (only for AF_INET)                  */
    std::string mDestination;   /**< Point-to-point destination address (only for AF_INET) */
};

/**
 * @brief Collection of mock interface definitions to be returned in a single
 *        getifaddrs() call.
 */
static vector<MockInterfaceDef> gMockInterfaces;

/**
 * @brief Determines whether getifaddrs() should pretend to fail (-1) or succeed (0).
 */
bool gMockGetIfAddrsShouldFail = false;


/*******************************************************************************
 *                                                                             *
 *                   InterfaceManager TEST UTILS - WRAPPERS                    *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Mock implementation of getifaddrs().
 *
 * @details This function simulates the behavior of getifaddrs() for testing
 *          purposes. It either returns a list of mock network interfaces
 *          or simulates a failure.
 *
 * @note This wrapper was created with assistance from GitHub Copilot.
 *
 * @param ifap Pointer to a linked list of network interfaces.
 * @return 0 on success, -1 on failure.
 */
extern "C" int __wrap_getifaddrs(ifaddrs **ifap) {
    // Simulate getifaddrs() failure
    if(gMockGetIfAddrsShouldFail) {
        *ifap = nullptr;
        return -1;
    }

    // Create a static array of ifaddrs to survive until freeifaddrs is called.
    static ifaddrs staticEntries[16] = {};
    static sockaddr_in staticSockAddrIn[16] = {};
    static sockaddr_in6 staticSockAddrIn6[16] = {};
    static sockaddr_in staticNetmask4[16] = {};
    static sockaddr_in6 staticNetmask6[16] = {};
    static sockaddr_in staticBroadcast4[16] = {};
    static sockaddr_in staticDestination4[16] = {};

    // Initialize the static entries
    size_t i = 0;
    for(; i < gMockInterfaces.size() && i < 16; i++) {
        const auto &[mName, mFlags, mFamily, mIp, mNetmask,
            mBroadcast, mDestination] = gMockInterfaces[i];
        auto &entry = staticEntries[i];

        // Set 'ifa_name'
        entry.ifa_name = const_cast<char*>(mName.c_str());

        // Set 'flags'
        entry.ifa_flags = mFlags;

        // Set 'ifa_addr' based on family
        if(mFamily == AF_INET) {
            staticSockAddrIn[i].sin_family = AF_INET;
            inet_pton(AF_INET, mIp.c_str(), &staticSockAddrIn[i].sin_addr);
            entry.ifa_addr = reinterpret_cast<sockaddr*>(&staticSockAddrIn[i]);
        }
        else if(mFamily == AF_INET6) {
            staticSockAddrIn6[i].sin6_family = AF_INET6;
            inet_pton(AF_INET6, mIp.c_str(), &staticSockAddrIn6[i].sin6_addr);
            entry.ifa_addr = reinterpret_cast<sockaddr*>(&staticSockAddrIn6[i]);
        }
        else {
            entry.ifa_addr = nullptr; // unknown family => interface will be skipped
        }

        // Set 'ifa_netmask'
        if(!mNetmask.empty()) {
            if(mFamily == AF_INET) {
                staticNetmask4[i].sin_family = AF_INET;
                inet_pton(AF_INET, mNetmask.c_str(), &staticNetmask4[i].sin_addr);
                entry.ifa_netmask = reinterpret_cast<sockaddr*>(&staticNetmask4[i]);
            }
            else if(mFamily == AF_INET6) {
                staticNetmask6[i].sin6_family = AF_INET6;
                inet_pton(AF_INET6, mNetmask.c_str(), &staticNetmask6[i].sin6_addr);
                entry.ifa_netmask = reinterpret_cast<sockaddr*>(&staticNetmask6[i]);
            }
        }

        // Set broadcast 'ifu_broadaddr' (only for AF_INET)
        if(!mBroadcast.empty() && mFamily == AF_INET) {
            staticBroadcast4[i].sin_family = AF_INET;
            inet_pton(AF_INET, mBroadcast.c_str(), &staticBroadcast4[i].sin_addr);
            entry.ifa_ifu.ifu_broadaddr = reinterpret_cast<sockaddr*>(&staticBroadcast4[i]);
        }

        // Set destination 'ifu_dstaddr' (only for AF_INET)
        if(!mDestination.empty() && mFamily == AF_INET) {
            staticDestination4[i].sin_family = AF_INET;
            inet_pton(AF_INET, mDestination.c_str(), &staticDestination4[i].sin_addr);
            entry.ifa_ifu.ifu_dstaddr = reinterpret_cast<sockaddr*>(&staticDestination4[i]);
        }

        // Link static entries together
        if(i < 15 && i < (gMockInterfaces.size() - 1)) {
            entry.ifa_next = &staticEntries[i + 1];
        }
        else {
            entry.ifa_next = nullptr;
        }
    } // for

    // Set 'ifap' to the first element
    if(i > 0) {
        *ifap = &staticEntries[0];
    }
    else {
        *ifap = nullptr; // If empty, return nullptr
    }

    return 0; // simulate getifaddrs() success
} // __wrap_getifaddrs()

/**
 * @brief Mock implementation of freeifaddrs().
 *
 * @details This function simulates the behavior of freeifaddrs() for testing
 *          purposes. In this static mock implementation, no memory is actually
 *          freed. In a real implementation, free(ifa) would be called if the
 *          memory was dynamically allocated.
 *
 * @param ifa Pointer to the linked list of network interfaces to be freed.
 */
extern "C" void __wrap_freeifaddrs(const ifaddrs *ifa [[maybe_unused]]) {
    // Because we use static array of ifaddrs, we don't need to free anything
} // __wrap_freeifaddrs()


/*******************************************************************************
 *                                                                             *
 *                   InterfaceManager TEST UTILS - FIXTURES                    *
 *                                                                             *
 ******************************************************************************/

/**
 * @brief Test fixture for InterfaceManager tests.
 */
class InterfaceManagerTest : public Test {
protected:
    /**
     * @brief Instance of InterfaceManager used in tests.
     */
    InterfaceManager mInterfaceManager;

    /**
     * @brief Set up the test environment.
     */
    void SetUp() override {
        gMockGetIfAddrsShouldFail = false;
        gMockInterfaces.clear();
    }

    /**
     * @brief Tear down the test environment.
     */
    void TearDown() override {
        gMockGetIfAddrsShouldFail = false;
        gMockInterfaces.clear();
    }
};


/*******************************************************************************
 *                                                                             *
 *                                 UNIT TESTS                                  *
 *                                                                             *
 ******************************************************************************/

TEST_F(InterfaceManagerTest, GetActiveInterfaces_SingleIPv4_Success) {
    // Arrange
    MockInterfaceDef def;
    def.mName = "eth0";
    def.mFlags = IFF_UP;
    def.mFamily = AF_INET;
    def.mIp = "192.168.1.10";
    gMockInterfaces.push_back(def);

    // Act
    const auto interfaces = InterfaceManager::getActiveInterfaces();

    // Assert
    ASSERT_EQ(interfaces.size(), 1u);
    EXPECT_EQ(interfaces[0].mName, "eth0");
    EXPECT_FALSE(interfaces[0].mIpAddresses.empty());
    EXPECT_EQ(interfaces[0].mIpAddresses[0], "192.168.1.10");
    EXPECT_TRUE(interfaces[0].mNetmask.empty());
    EXPECT_TRUE(interfaces[0].mBroadcastAddress.empty());
    EXPECT_TRUE(interfaces[0].mDestinationAddress.empty());
}

TEST_F(InterfaceManagerTest, GetActiveInterfaces_FailGetIfAddrs) {
    // Arrange
    gMockGetIfAddrsShouldFail = true;

    // Act & Assert
    EXPECT_THROW(mInterfaceManager.getActiveInterfaces(), InternalErrorException);
}

TEST_F(InterfaceManagerTest, SkipInterfaceIfNotUp) {
    // Arrange
    // Interface 1: eth0 UP
    MockInterfaceDef d1;
    d1.mName = "eth0";
    d1.mFlags = IFF_UP;
    d1.mFamily = AF_INET;
    d1.mIp = "10.0.0.1";
    gMockInterfaces.push_back(d1);

    // Interface 2: wlan0 DOWN
    MockInterfaceDef d2;
    d2.mName = "wlan0";
    d2.mFlags = 0;
    d2.mFamily = AF_INET;
    d2.mIp = "192.168.2.5";
    gMockInterfaces.push_back(d2);

    // Act
    const auto interfaces = InterfaceManager::getActiveInterfaces();

    // Assert
    ASSERT_EQ(interfaces.size(), 1u);
    EXPECT_EQ(interfaces[0].mName, "eth0");
}

TEST_F(InterfaceManagerTest, SkipInterfaceIfAddrNull) {
    // Arrange
    // Interface 1: ghost0 with no address (undefined)
    MockInterfaceDef d1;
    d1.mName = "ghost0";
    d1.mFlags = IFF_UP;
    d1.mFamily = AF_UNSPEC;
    gMockInterfaces.push_back(d1);

    // Interface 2: eth0 with valid IPv4
    MockInterfaceDef d2;
    d2.mName = "eth0";
    d2.mFlags = IFF_UP;
    d2.mFamily = AF_INET;
    d2.mIp = "192.168.99.1";
    gMockInterfaces.push_back(d2);

    // Act
    const auto interfaces = InterfaceManager::getActiveInterfaces();

    // Assert
    ASSERT_EQ(interfaces.size(), 1u);
    EXPECT_EQ(interfaces[0].mName, "eth0");
}

TEST_F(InterfaceManagerTest, NetmaskSet) {
    // Arrange
    MockInterfaceDef d;
    d.mName = "eth0";
    d.mFlags = IFF_UP;
    d.mFamily = AF_INET;
    d.mIp = "192.168.0.42";
    d.mNetmask = "255.255.255.0";
    gMockInterfaces.push_back(d);

    // Act
    const auto interfaces = InterfaceManager::getActiveInterfaces();

    // Assert
    ASSERT_EQ(interfaces.size(), 1u);
    EXPECT_EQ(interfaces[0].mNetmask, "255.255.255.0");
}

TEST_F(InterfaceManagerTest, BroadcastSet_OnlyForIPv4) {
    // Arrange
    // AF_INET => broadcast should be set
    MockInterfaceDef d1;
    d1.mName = "eth0";
    d1.mFlags = IFF_UP;
    d1.mFamily = AF_INET;
    d1.mIp = "10.0.0.5";
    d1.mBroadcast = "10.0.0.255";
    gMockInterfaces.push_back(d1);

    // AF_INET6 => broadcast ignored
    MockInterfaceDef d2;
    d2.mName = "eth1";
    d2.mFlags = IFF_UP;
    d2.mFamily = AF_INET6;
    d2.mIp = "fe80::abcd";
    d2.mBroadcast = "fe80::ffff"; // this is ignored in InterfaceManager
    gMockInterfaces.push_back(d2);

    // Act
    const auto interfaces = InterfaceManager::getActiveInterfaces();

    // Assert
    ASSERT_EQ(interfaces.size(), 2u);

    EXPECT_EQ(interfaces[0].mName, "eth0");
    EXPECT_EQ(interfaces[0].mBroadcastAddress, "10.0.0.255");

    EXPECT_EQ(interfaces[1].mName, "eth1");
    EXPECT_TRUE(interfaces[1].mBroadcastAddress.empty()); // ignoring IPv6 broadcast
}

TEST_F(InterfaceManagerTest, DestinationAddressForIPv4) {
    // Arrange
    // tun0 with p2p destination 10.8.0.1
    MockInterfaceDef d;
    d.mName = "tun0";
    d.mFlags = IFF_UP;
    d.mFamily = AF_INET;
    d.mIp = "10.8.0.2";
    d.mDestination = "10.8.0.1";
    gMockInterfaces.push_back(d);

    // Act
    const auto interfaces = InterfaceManager::getActiveInterfaces();

    // Assert
    ASSERT_EQ(interfaces.size(), 1u);
    EXPECT_EQ(interfaces[0].mDestinationAddress, "10.8.0.1");
}

TEST_F(InterfaceManagerTest, GetInterfaceByName_Simple) {
    // Arrange
    MockInterfaceDef d1;
    d1.mName = "eth0";
    d1.mFlags = IFF_UP;
    d1.mFamily = AF_INET;
    d1.mIp = "1.2.3.4";
    gMockInterfaces.push_back(d1);

    MockInterfaceDef d2;
    d2.mName = "wlan0";
    d2.mFlags = IFF_UP;
    d2.mFamily = AF_INET;
    d2.mIp = "192.168.5.5";
    gMockInterfaces.push_back(d2);

    // Act
    auto interface = InterfaceManager::getInterfaceByName("eth0");

    // Assert
    EXPECT_EQ(interface.mName, "eth0");
    EXPECT_EQ(interface.mIpAddresses.size(), 1u);
    EXPECT_EQ(interface.mIpAddresses[0], "1.2.3.4");
}

TEST_F(InterfaceManagerTest, GetInterfaceByName_NotFound) {
    // Arrange
    MockInterfaceDef d;
    d.mName = "eth0";
    d.mFlags = IFF_UP;
    d.mFamily = AF_INET;
    d.mIp = "192.168.0.10";
    gMockInterfaces.push_back(d);

    // Act & Assert
    EXPECT_THROW(mInterfaceManager.getInterfaceByName("wlan0"), InterfaceErrorException);
}

TEST_F(InterfaceManagerTest, GetInterfaceByName_CaseInsensitive) {
    // Arrange
    MockInterfaceDef d;
    d.mName = "eth0";
    d.mFlags = IFF_UP;
    d.mFamily = AF_INET;
    d.mIp = "10.0.0.2";
    gMockInterfaces.push_back(d);

    // Act
    const auto interface = InterfaceManager::getInterfaceByName("ETH0");

    // Assert
    EXPECT_EQ(interface.mName, "eth0");
}

TEST_F(InterfaceManagerTest, GetInterfaceByName_PartialName) {
    // Arrange
    MockInterfaceDef d;
    d.mName = "eth0";
    d.mFlags = IFF_UP;
    d.mFamily = AF_INET;
    d.mIp = "10.0.0.2";
    gMockInterfaces.push_back(d);

    // Act & Assert
    EXPECT_THROW(mInterfaceManager.getInterfaceByName("eth"), InterfaceErrorException);
}

TEST_F(InterfaceManagerTest, NoActiveInterfaces_Empty) {
    // Arrange
    // Empty interface list

    // Act
    const auto interfaces = mInterfaceManager.getActiveInterfaces();

    // Assert
    EXPECT_TRUE(interfaces.empty());
    EXPECT_THROW(mInterfaceManager.getInterfaceByName("eth0"), InterfaceErrorException);
}

/*** end of file InterfaceManagerTests.cpp ***/
