/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         UDPScanner.hpp                                                *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the UDPScanner class, which is                 *
 *               responsible for scanning UDP network ports.                   *
 *                                                                             *
 ******************************************************************************/
/**
 * @file UDPScanner.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the UDPScanner class.
 */

#ifndef UDP_SCANNER_HPP
#define UDP_SCANNER_HPP

#include "Scanning/PortScanner.hpp"
#include "Scanning/ScanResult.hpp"
#include "Networking/InterfaceInfo.hpp"
#include <string>  // std::string
#include <chrono>  // std::chrono::milliseconds

namespace OmegaL4Scanner::Scanning
{
    /**
     * @class UdpScanner
     * @brief Class responsible for scanning UDP network ports.
     */
    class UdpScanner final : public PortScanner {
    public:
        /**
         * @brief Constructs a UdpScanner.
         *
         * @param ipaddressesToScan The target address to scan.
         * @param interfaceInfo Information about the network interface.
         * @param waitTimeout The timeout duration for the scan.
         */
        UdpScanner(const std::vector<std::string> &ipaddressesToScan,
                   const Networking::InterfaceInfo &interfaceInfo,
                   std::chrono::milliseconds waitTimeout);

        /**
         * @brief Scans a specific port.
         *
         * @param ipAddressToScan The target IP address to scan.
         * @param portToScan The port number to scan.
         * @return ScanResult The result of the port scan.
         */
        ScanResult scanPort(const std::string &ipAddressToScan, int portToScan) override;
    }; // UdpScanner
} // OmegaL4Scanner::Scanning

#endif // UDP_SCANNER_HPP

/*** end of file UDPScanner.hpp ***/
