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
 * Last edit:    26.03.2025                                                    *
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
         * @brief Checks for a response on a raw socket.
         *
         * @details Waits for a response on the specified raw socket and
         *          checks if the port is closed or open based on the response.
         *
         * @param rawSocket The raw socket file descriptor.
         * @param waitTimeoutMilliseconds The timeout in milliseconds.
         * @param sourcePort The source port.
         * @param destinationPort The destination port.
         * @param ipAddressVersion The IP address version (IPv4 or IPv6).
         * @param protocolType The protocol type (TCP or UDP).
         *
         * @return PortStatus indicating whether the port is open, closed, or filtered.
         */
        Enums::PortStatus checkRawResponse(int rawSocket, long int waitTimeoutMilliseconds,
                                           uint16_t sourcePort, int destinationPort,
                                           Common::IPAddressVersion ipAddressVersion,
                                           const Common::ProtocolType &protocolType);

        /**
         * @brief Determines the status of a port based on the received packet.
         *
         * @param pBuffer Pointer to the buffer containing the received packet data.
         * @param bytesReceived The number of bytes received in the packet.
         * @param destinationPort The destination port number.
         * @param sourcePort The source port number.
         * @param ipAddressVersion The IP address version (IPv4 or IPv6).
         *
         * @return Enums::PortStatus indicating whether the port is open, closed, or filtered.
         */
        virtual Enums::PortStatus determinePortStatus(const uint8_t *pBuffer, ssize_t bytesReceived,
                                                      int destinationPort, uint16_t sourcePort,
                                                      Common::IPAddressVersion ipAddressVersion) = 0;

        /**
         * @brief Converts a timeout in milliseconds to an integer.
         *
         * @details This function ensures that the conversion from
         *          `std::chrono::milliseconds` to `long int` does not overflow
         *          by clamping the value within the range of `long int`.
         *
         * @note The use of static casts to 'int64_t' may be necessary on some
         *       platforms as 'long int' may not be 64-bit on them.
         *
         * @param waitTimeout The duration in milliseconds to be converted.
         * @return long int The converted duration as an integer, clamped to
         *         the range of `long int`.
         */
        static long int millisecondsToLongInt(std::chrono::milliseconds waitTimeout);

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
        static std::string getSourceAddress(const Networking::InterfaceInfo &interfaceInfo,
                                            Common::IPAddressVersion ipAddressType);

        /**
         * @brief Determines the IP address version (IPv4 or IPv6) of the given
         *        IP address string.
         *
         * @param ipAddress The IP address as a string to be analyzed.
         * @return Common::IPAddressVersion The version of the IP address (IPv4 or IPv6).
         */
        static Common::IPAddressVersion getIpAddressVersion(const std::string &ipAddress);

        /**
         * @brief Configures the specified socket to operate in non-blocking mode.
         *
         * @param socket The socket file descriptor.
         */
        static void useNonBlockingMode(int socket);

        /**
         * @brief Initializes the libnet context.
         *
         * @param interfaceInfo Information about the network interface.
         * @param ipAddressVersion Whether the address is IPv6.
         *
         * @return Pointer to the initialized libnet context.
         */
        static libnet_t *initLibnetContext(const Networking::InterfaceInfo &interfaceInfo,
                                           Common::IPAddressVersion ipAddressVersion);

        /**
         * @brief Creates a raw socket.
         *
         * @param ipAddressVersion Whether the address is IPv6.
         * @param protocolType The protocol type (TCP or UDP).
         *
         * @return The file descriptor of the created raw socket.
         */
        static int createRawSocket(Common::IPAddressVersion ipAddressVersion,
                                   const Common::ProtocolType &protocolType);

        /**
         * @brief Builds the IPv4 header using libnet.
         *
         * @param pLibnetContext Pointer to the libnet context.
         * @param sourceIpv4 Source IP address in binary form.
         * @param destinationIpv4 Destination IP address in binary form.
         * @param protocolType The protocol type (TCP or UDP).
         */
        static void createIpv4Header(libnet_t *pLibnetContext, uint32_t sourceIpv4,
                                     uint32_t destinationIpv4, const Common::ProtocolType &protocolType);

        /**
         * @brief Builds the IPv6 header using libnet.
         *
         * @param pLibnetContext Pointer to the libnet context.
         * @param sourceIpv6 Source IPv6 address in binary form.
         * @param destinationIpv6 Destination IPv6 address in binary form.
         * @param protocolType The protocol type (TCP or UDP).
         */
        static void createIpv6Header(libnet_t *pLibnetContext, libnet_in6_addr sourceIpv6,
                                     libnet_in6_addr destinationIpv6, const Common::ProtocolType &protocolType);

        /**
         * @brief Builds the IP header (either IPv4 or IPv6) using libnet.
         * @details Constructs the IP header for the packet, either IPv4 or IPv6,
         *          using the libnet library.
         *
         * @param pLibnetContext Pointer to the libnet context.
         * @param ipAddressVersion Whether the address is IPv6.
         * @param interfaceInfo Information about the network interface.
         * @param ipAddressToScan The target IP address to scan.
         * @param protocolType The protocol type (TCP or UDP).
         */
        static void createIpHeader(libnet_t *pLibnetContext, Common::IPAddressVersion ipAddressVersion,
                                   const Networking::InterfaceInfo &interfaceInfo,
                                   const std::string &ipAddressToScan, const Common::ProtocolType &protocolType);

        /**
         * @brief Initializes the file descriptor set for the select() call.
         *
         * @param readfds Reference to the file descriptor set to be initialized.
         * @param rawSocket The raw socket file descriptor to be added to the set.
         */
        static void initializeFdSet(fd_set &readfds, int rawSocket);

        /**
         * @brief Sets the timeval structure for the select() call.
         *
         * @param tv Reference to the timeval structure to be set.
         * @param waitTimeoutMilliseconds The timeout value in milliseconds.
         */
        static void setTimeval(timeval &tv, int waitTimeoutMilliseconds);

        /**
         * @brief Calls the select() function to monitor the raw socket for readability.
         *
         * @param rawSocket The raw socket file descriptor.
         * @param fileDescriptorSet Reference to the file descriptor set.
         * @param timeout Reference to the timeval structure specifying the timeout.
         * @return int The result of the select() call.
         */
        static int selectSocket(int rawSocket, fd_set &fileDescriptorSet, timeval &timeout);
    }; // PortScanner
} // OmegaL4Scanner::Scanning

#endif // PORT_SCANNER_HPP

/*** end of file PortScanner.hpp ***/
