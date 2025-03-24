/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ExitCodes.hpp                                                 *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the ExitCodes enum class, which is used to     *
 *               represent error and other exit codes in the OMEGA L4 Scanner. *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ExitCodes.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the ExitCodes enum class.
 */

#ifndef EXIT_CODES_HPP
#define EXIT_CODES_HPP

namespace OmegaL4Scanner::Enums
{
    /**
     * @enum ExitCodes
     * @brief Enum class representing custom exit codes in the OMEGA L4 Scanner.
     *
     * @details This enum class defines various error codes that can be used
     *          to represent different error conditions in the OMEGA L4 Scanner
     *          project.
     */
    enum class ExitCodes {
        SUCCESS                   = 0,   /**< Success exit code (EX_OK).                          */
        INVALID_ARGUMENT_ERROR    = 64,  /**< Invalid argument error code (EX_USAGE).             */
        INTERFACE_ERROR           = 66,  /**< Error while getting active interfaces (EX_NOINPUT). */
        HOSTNAME_RESOLUTION_ERROR = 68,  /**< Hostname resolution error code (EX_NOHOST).         */
        INTERNAL_ERROR            = 70,  /**< Internal error code (EX_SOFTWARE).                  */
        SOCKET_ERROR              = 71,  /**< Socket error code (EX_OSERR).                       */
        LIBNET_ERROR              = 73,  /**< Libnet error code (EX_CANTCREAT).                   */
        UNKNOWN_ERROR             = 78,  /**< Unknown error code (EX_CONFIG).                     */
        USER_INTERRUPTION_ERROR   = 130  /**< Interrupted by user error code (128 + SIGINT).      */
    }; // ExitCodes
} // OmegaL4Scanner::Enums

#endif // EXIT_CODES_HPP

/*** end of file ExitCodes.hpp ***/
