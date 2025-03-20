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
 * Last edit:    19.03.2025                                                    *
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
        SUCESS                    = 0,  /**< Success exit code.                     */
        HELP_REQUESTED            = 1,  /**< Help requested exit code.              */
        INTERFACE_PRINT_REQUESTED = 2,  /**< Interface print requested exit code.   */
        INVALID_ARGUMENT_ERROR    = 10,  /**< Invalid argument error code.           */
        INTERFACE_ERROR           = 11,  /**< Error while getting active interfaces. */
        SOCKET_ERROR              = 12,  /**< Socket error code.                     */
        PCAP_ERROR                = 13,  /**< PCAP error code.                       */
        USER_INTERRUPTION_ERROR   = 14,  /**< Interrupted by user error code.        */
        INTERNAL_ERROR            = 99,  /**< Internal error code.                   */
        UNKNOWN_ERROR             = 255  /**< Unknown error code.                    */
    }; // ExitCodes
} // namespace OmegaL4Scanner::Enums

#endif // EXIT_CODES_HPP

/*** end of file ExitCodes.hpp ***/
