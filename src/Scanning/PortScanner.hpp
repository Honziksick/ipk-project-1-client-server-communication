/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         PortScanner.hpp                                               *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    23.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the PortScanner class, which is                *
 *               responsible for scanning network ports.                       *
 *                                                                             *
 ******************************************************************************/
/**
 * @file PortScanner.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the PortScanner class.
 */

#ifndef PORT_SCANNER_HPP
#define PORT_SCANNER_HPP

#include "Scanning/ScanResult.hpp"
#include "Common/OmegaDataTypes.hpp"
#include "Networking/InterfaceInfo.hpp"
#include <string>  // std::string
#include <vector>  // std::vector
#include <chrono>  // std::chrono::milliseconds

namespace OmegaL4Scanner::Scanning
{
    /**
     * @class PortScanner
     * @brief Abstract base class for scanning network ports.
     *
     * @details This class provides the interface for scanning network ports.
     *          It must be inherited by concrete scanner implementations that
     *          define the actual scanning logic.
     */
    class PortScanner {
    public:
        /**
         * @brief Constructs a PortScanner object.
         *
         * @param ipaddressesToScan The target IP address to scan.
         * @param interfaceInfo The network interface information.
         * @param waitTimeout The timeout in milliseconds to wait for responses.
         */
        PortScanner(const std::vector<std::string> &ipaddressesToScan,
                    const Networking::InterfaceInfo &interfaceInfo,
                    std::chrono::milliseconds waitTimeout);

        /**
         * @brief Virtual destructor for PortScanner.
         */
        virtual ~PortScanner() = default;

        /**
         * @brief Scans multiple ports given as a vector of port ranges.
         *
         * @details Expands the port ranges into individual ports and calls
         *          scanPort() on each.
         *
         * @param portRanges Vector of port ranges (each can be int or pair<int,int>).
         */
        void scanPorts(const std::vector<Common::PortRange> &portRanges);

    protected:
        std::vector<std::string> mIpaddressesToScan; /**< The target IP addresses to scan.                   */
        Networking::InterfaceInfo mInterfaceInfo;    /**< The network interface information.                 */
        std::chrono::milliseconds mWaitTimeout;      /**< The timeout in milliseconds to wait for responses. */

        /**
         * @brief Scans a specific port.
         *
         * @param ipAddressToScan The target IP address to scan.
         * @param portToScan The port number to scan.
         * @return ScanResult The result of the port scan.
         */
        virtual ScanResult scanPort(const std::string &ipAddressToScan, int portToScan) = 0;

        /**
         * @brief Converts a timeout in milliseconds to an integer.
         *
         * @details This function ensures that the conversion from
         *          `std::chrono::milliseconds` to `int` does not overflow
         *          by clamping the value within the range of `int`.
         *
         * @param waitTimeout The duration in milliseconds to be converted.
         * @return int The converted duration as an integer, clamped to the range of `int`.
         */
        static int millisecondsToInt(std::chrono::milliseconds waitTimeout);

        /**
         * @brief Retrieves the source address from the network interface
         *        information.
         *
         * @details This function iterates through the IP addresses associated
         *          with the provided network interface and returns the first
         *          address that matches the specified IP type (IPv4 or IPv6).
         *
         * @param interfaceInfo The network interface information containing IP addresses.
         * @param ipAddressType The type of IP address to retrieve (IPv4 or IPv6).
         * @return std::string The source address as a string.
         */
        static std::string getSourceAddress(const Networking::InterfaceInfo &interfaceInfo, Common::IPAddressVersion ipAddressType);

        /**
         * @brief Determines the IP address version (IPv4 or IPv6) of the given
         *        IP address string.
         *
         * @param ipAddress The IP address as a string to be analyzed.
         * @return Common::IPAddressVersion The version of the IP address (IPv4 or IPv6).
         */
        static Common::IPAddressVersion getIpAddressVersion(const std::string &ipAddress);
    }; // PortScanner
} // OmegaL4Scanner::Scanning

#endif // PORT_SCANNER_HPP

/*** end of file PortScanner.hpp ***/
