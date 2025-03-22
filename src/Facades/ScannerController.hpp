/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ScannerController.hpp                                         *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the ScannerController class, which is          *
 *               responsible for controlling the scanning process.             *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ScannerController.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the ScannerController class.
 */

#ifndef SCANNER_CONTROLLER_HPP
#define SCANNER_CONTROLLER_HPP

#include "Common/CommandLineOptions.hpp"
#include "Networking/InterfaceInfo.hpp"
#include <string>  // std::string
#include <vector>  // std::vector

namespace OmegaL4Scanner::Facades
{
    /**
     * @class ScannerController
     * @brief Controller that coordinates the scanning process.
     *
     * @details This class takes the parsed command line options and network
     *          interface information and performs the scanning by invoking
     *          TCP and UDP scanners.
     */
    class ScannerController final {
    public:
        /**
         * @brief Constructor for the ScannerController class.
         *
         * @param options Parsed command line options.
         * @param interfaceInfo Selected network interface information.
         */
        ScannerController(Common::CommandLineOptions options, Networking::InterfaceInfo interfaceInfo);

        /**
         * @brief Executes the L4 scanning process.
         *
         * @details This function orchestrates the steps required for scanning
         *          by analyzing the provided host or interface data and
         *          collecting results.
         */
        void scanL4Layer() const;

    private:
        Common::CommandLineOptions mCommandLineOptions;  /**< Parsed command line options.   */
        Networking::InterfaceInfo mInterfaceInfo;        /**< Network interface information. */

        /**
         * @brief Scans TCP ports.
         *
         * @details This method initiates the process of scanning TCP ports by
         *          sending probe packets and analyzing responses to determine
         *          port states.
         *
         * @param ipAddresses Vector of IP addresses to scan.
         */
        void scanTcpPorts(const std::vector<std::string> &ipAddresses) const;

        /**
         * @brief Scans UDP ports.
         *
         * @details This method initiates the process of scanning UDP ports by
         *          sending probe packets and analyzing responses to determine
         *          port states.
         *
         * @param ipAddresses Vector of IP addresses to scan.
         */
        void scanUdpPorts(const std::vector<std::string> &ipAddresses) const;
    }; // ScannerController
} // OmegaL4Scanner::Facades

#endif // SCANNER_CONTROLLER_HPP

/*** end of file ScannerController.hpp ***/
