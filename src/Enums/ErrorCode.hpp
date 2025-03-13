/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ErrorCode.hpp                                                 *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    12.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the ErrorCode enum class, which is used to     *
 *               represent error codes in the OMEGA L4 Scanner.                *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ErrorCode.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the ErrorCodes enum class.
 */

#ifndef ERROR_CODES_HPP
#define ERROR_CODES_HPP

namespace OmegaL4Scanner::Enums
{
    /**
     * @enum ErrorCode
     * @brief Enum class representing error codes in the OMEGA L4 Scanner.
     *
     * @details This enum class defines various error codes that can be used
     *          to represent different error conditions in the OMEGA L4 Scanner
     *          project.
     */
    enum class ErrorCode {
        INVALID_ARGUMENT_ERROR   = 1,   /**< Invalid argument error code.    */
        INVALID_INTERFACE_ERROR  = 2,   /**< Invalid interface error code.   */
        INVALID_PORT_RANGE_ERROR = 3,   /**< Invalid port range error code.  */
        INVALID_HOSTNAME_ERROR   = 4,   /**< Invalid hostname error code.    */
        SOCKET_ERROR             = 5,   /**< Socket error code.              */
        PCAP_ERROR               = 6,   /**< PCAP error code.                */
        USER_INTERRUPTION_ERROR  = 7,   /**< Interrupted by user error code. */
        INTERNAL_ERROR           = 99,  /**< Internal error code.            */
        UNKNOWN_ERROR            = 666  /**< Unknown error code.             */
    }; // ErrorCode
} // namespace OmegaL4Scanner::Enums

#endif // ERROR_CODES_HPP

/*** end of file ErrorCode.hpp ***/
