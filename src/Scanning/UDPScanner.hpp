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
 * Last edit:    25.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the UdpScanner class, which is responsible for *
 *               scanning UDP ports on specified IP addresses. The UdpScanner  *
 *               class uses the libnet library to construct and send UDP       *
 *               packets. If an ICMP response is received (IPv4: type 3,       *
 *               code 3; IPv6: type 1, code 4), the port is considered closed; *
 *               otherwise, it is considered open.                             *
 *                                                                             *
 ******************************************************************************/
/**
 * @file UDPScanner.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the UdpScanner class for scanning UDP ports.
 */

#ifndef UDP_SCANNER_HPP
#define UDP_SCANNER_HPP

#include "Scanning/PortScanner.hpp"
#include "Scanning/ScanResult.hpp"
#include "Networking/InterfaceInfo.hpp"
#include "Common/OmegaDataTypes.hpp"
#include "Enums/PortStatus.hpp"
#include <string>    // std::string
#include <vector>    // std::vector
#include <chrono>    // std::chrono::milliseconds
#include <libnet.h>  // libnet_t, libnet_in6_addr

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
         * @param ipAddressesToScan The target addresses to scan.
         * @param interfaceInfo Information about the network interface.
         * @param waitTimeout The timeout duration for the scan.
         */
        UdpScanner(const std::vector<std::string> &ipAddressesToScan,
                   const Networking::InterfaceInfo &interfaceInfo,
                   std::chrono::milliseconds waitTimeout);

        /**
         * @brief Scans a specific port.
         *
         * @details Performs a scan on the specified port of the given IP
         *          address and returns the scan result.
         *
         * @param ipAddressToScan The target IP address to scan.
         * @param portToScan The port number to scan.
         *
         * @return ScanResult The result of the port scan.
         */
        ScanResult scanPort(const std::string &ipAddressToScan, int portToScan) override;

    private:
        /**
         * @brief Checks if the port is unreachable based on the received ICMP packet.
         *
         * @param pBuffer Pointer to the buffer containing the received packet.
         * @param bytesReceived The number of bytes received in the packet.
         * @param destinationPort The destination port to check.
         * @param sourcePort The source port to check.
         * @param ipAddressVersion The IP address version (IPv4 or IPv6).
         *
         * @return PortStatus::CLOSED if an ICMP response is received,
         *         PortStatus::OPEN otherwise.
         */
        Enums::PortStatus determinePortStatus(const uint8_t *pBuffer, ssize_t bytesReceived,
                                              int destinationPort, uint16_t sourcePort,
                                              Common::IPAddressVersion ipAddressVersion) override;

        /**
         * @brief Constructs the UDP header for the packet using the libnet library.
         *
         * @param pLibnetContext Pointer to the libnet context.
         * @param ipAddressVersion Whether the address is IPv6.
         * @param sourcePort The source port.
         * @param destinationPort The destination port.
         *
         * @return The protocol tag value of the UDP header.
         */
        static void createUdpHeader(libnet_t *pLibnetContext, Common::IPAddressVersion ipAddressVersion,
                                    uint16_t sourcePort, int destinationPort);
    }; // UdpScanner
} // OmegaL4Scanner::Scanning

#endif // UDP_SCANNER_HPP;

/*** end of file UDPScanner.hpp ***/
