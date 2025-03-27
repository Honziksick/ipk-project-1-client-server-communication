/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         TCPScanner.hpp                                                *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    25.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the declaration of the TcpScanner class,   *
 *               which is responsible for scanning TCP ports on specified      *
 *               IP addresses. The TcpScanner class uses raw sockets and       *
 *               the libnet library to send TCP SYN packets and determine      *
 *               the status of the ports based on the responses received.      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file TCPScanner.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the TcpScanner class for scanning TCP ports.
 */

#ifndef TCP_SCANNER_HPP
#define TCP_SCANNER_HPP

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
     * @class TcpScanner
     * @brief Class responsible for scanning TCP network ports.
     */
    class TcpScanner final : public PortScanner {
    public:
        /**
         * @brief Constructs a TcpScanner.
         *
         * @param ipaddressesToScan The target address to scan.
         * @param interfaceInfo Information about the network interface.
         * @param waitTimeout The timeout duration for the scan.
         */
        TcpScanner(const std::vector<std::string> &ipaddressesToScan,
                   const Networking::InterfaceInfo &interfaceInfo,
                   std::chrono::milliseconds waitTimeout);
        /**
         * @brief Scans a specific port.
         *
         * @param ipAddressToScan The IP address to scan.
         * @param portToScan The port number to scan.
         * @return ScanResult The result of the port scan.
         */
        ScanResult scanPort(const std::string &ipAddressToScan, uint16_t portToScan) override;

    private:
        static constexpr int MAX_TRANSMIT_ATTEMPTS = 2;  /**< Maximum number of SYN packet transmit attempts. */

        /**
         * @brief Parses a received TCP response packet.
         *
         * @details This function expects the raw packet to contain an IP header
         *          followed by a TCP header. It verifies that the response’s
         *          destination port equals the ephemeral (source) port used
         *          in the SYN packet and that the source port equals the target
         *          port. Depending on the TCP flags (RST, SYN/ACK), it returns
         *          the corresponding port status.
         *
         * @param pBuffer Pointer to the received data buffer.
         * @param bytesReceived Number of bytes received.
         * @param destinationPort The target port.
         * @param sourcePort The ephemeral source port.
         * @param ipAddressVersion The IP address version (IPv4 or IPv6).
         *
         * @return PortStatus::OPEN if SYN/ACK received, PortStatus::CLOSED if
         *         RST received, or PortStatus::FILTERED if the packet is invalid.
         */
        Enums::PortStatus determinePortStatus(const uint8_t *pBuffer, ssize_t bytesReceived,
                                              uint16_t destinationPort, uint16_t sourcePort,
                                              Common::IPAddressVersion ipAddressVersion) override;

        /**
         * @brief Constructs the TCP header for the packet using the libnet library.
         *
         * @param pLibnetContext The libnet context.
         * @param sourcePort The source port.
         * @param destinationPort The destination port.
         * @param sequenceNumber The sequence number.
         */
        static void createTcpHeader(libnet_t *pLibnetContext, uint16_t sourcePort,
                                    uint16_t destinationPort, uint32_t sequenceNumber);
    }; // TcpScanner
} // OmegaL4Scanner::Scanning

#endif // TCP_SCANNER_HPP

/*** end of file TCPScanner.hpp ***/
