/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ExceptionMessages.hpp                                         *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains constant exception messages used in the    *
 *               Omega L4 Scanner project.                                     *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ExceptionMessages.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Constant exception messages for the Omega L4 Scanner project.
 */

#ifndef EXCEPTION_MESSAGES_HPP
#define EXCEPTION_MESSAGES_HPP

namespace OmegaL4Scanner::Constants
{
    /**
     * @brief Message indicating that the user requested help.
     */
    inline auto helpRequestedMsg = "User requested help.";

    /**
     * @brief Message indicating that the user requested to print interfaces.
     */
    inline auto interfacePrintRequestedMsg = "User requested the print of interfaces.";

    /**
     * @brief Error message for invalid argument.
     */
    inline auto invalidArgumentErrorMsg = "Invalid argument provided.";

    /**
     * @brief Error message for invalid interface.
     */
    inline auto interfaceErrorMsg = "Invalid interface provided.";

    /**
     * @brief Error message for hostname resolution error.
     */
    inline auto hostnameResolutionErrorMsg = "Unable to resolve hostname.";

    /**
     * @brief Error message for socket error.
     */
    inline auto socketErrorMsg = "Socket error occurred.";

    /**
     * @brief Error message for client communication error.
     */
    inline auto communicationErrorMsg = "Error occurred during client communication.";

    /**
     * @brief Error message for pcap error.
     */
    inline auto pcapErrorMsg = "Pcap error occurred.";

    /**
     * @brief Error message for user interruption.
     */
    inline auto userInterruptionMsg = "Operation was interrupted.";

    /**
     * @brief Error message for internal error.
     */
    inline auto internalErrorMsg = "Internal error occurred.";

    /**
     * @brief Error message for unknown error.
     */
    inline auto unknownErrorMsg = "An unexpected unknown error occurred. Please report this issue to the developers.";
} // OmegaL4Scanner::Constants

#endif // EXCEPTION_MESSAGES_HPP

/*** end of file ExceptionMessages.hpp ***/
