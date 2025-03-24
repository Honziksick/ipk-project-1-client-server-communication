/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ScanResult.hpp                                                *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    23.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the ScanResult class, which is responsible     *
 *               for storing the results of scans and formating them for       *
 *               final print.                                                  *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ScanResult.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the ScanResult class.
 */

#ifndef SCAN_RESULT_HPP
#define SCAN_RESULT_HPP

#include "Common/OmegaDataTypes.hpp"
#include "Enums/PortStatus.hpp"
#include <string>  // std::string

namespace OmegaL4Scanner::Scanning
{
    /**
     * @class ScanResult
     * @brief Represents the result of scanning a single port.
     */
    class ScanResult final {
    public:
        /**
         * @brief Default constructor for ScanResult.
         */
        ScanResult();

        /**
         * @brief Constructs a ScanResult with specific parameters.
         *
         * @param scannedIpaddress The IP address that was scanned.
         * @param port The port number.
         * @param protocol The protocol used (TCP or UDP).
         * @param status The status of the port.
         */
        ScanResult(const std::string &scannedIpaddress, int port, const Common::Protocol &protocol, Enums::PortStatus status);

        std::string mScannedIpaddress; /**< IP address that was scanned. */
        int mPort;                     /**< Port number.                 */
        Common::Protocol mProtocol;    /**< Protocol used (TCP or UDP).  */
        Enums::PortStatus mStatus;     /**< Status of the port.          */

        /**
         * @brief Returns a formatted string of the scan result.
         *
         * @return std::string A string representation of the scan result.
         */
        std::string scanResultToString() const;
    }; // ScanResult
} // OmegaL4Scanner::Scanning

#endif // SCAN_RESULT_HPP

/*** end of file ScanResult.hpp ***/
