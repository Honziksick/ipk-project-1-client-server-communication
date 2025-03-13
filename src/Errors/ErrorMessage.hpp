/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ErrorMessage.hpp                                              *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    13.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains constant error messages used in the        *
 *               Omega L4 Scanner project.                                     *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ErrorMessage.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Constant error messages for the Omega L4 Scanner project.
 */

#ifndef ERROR_MESSAGE_HPP
#define ERROR_MESSAGE_HPP

namespace OmegaL4Scanner::Errors
{
    /**
     * @brief Error message for invalid argument.
     */
    inline auto invalidArgumentErrMsg = "Invalid argument provided.";

    /**
     * @brief Error message for invalid interface.
     */
    inline auto invalidInterfaceErrMsg = "Invalid interface provided.";

    /**
     * @brief Error message for invalid port range.
     */
    inline auto invalidPortRangeErrMsg = "Invalid port range provided.";

    /**
     * @brief Error message for invalid hostname.
     */
    inline auto invalidHostnameErrMsg = "Invalid hostname entered.";

    /**
     * @brief Error message for socket error.
     */
    inline auto socketErrMsg = "Socket error occurred.";

    /**
     * @brief Error message for pcap error.
     */
    inline auto pcapErrMsg = "Pcap error occurred.";

    /**
     * @brief Error message for user interruption.
     */
    inline auto userInterruptionErrMsg = "Operation was interrupted.";

    /**
     * @brief Error message for internal error.
     */
    inline auto internalErrMsg = "Internal error occurred.";
} // OmegaL4Scanner::Errors

#endif // ERROR_MESSAGE_HPP

/*** end of file ErrorMessages.hpp ***/
