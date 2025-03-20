/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaBaseException.hpp                                        *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    13.03.2025                                                    *
 *                                                                             *
 * Description:  Header file for the OmegaBaseException class used in the      *
 *               OMEGA L4 Scanner project.                                     *
 *                                                                             *
 ******************************************************************************/
/**
 * @file OmegaBaseException.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the OmegaBaseException class.
 */

#ifndef OMEGA_BASE_EXCEPTION_HPP
#define OMEGA_BASE_EXCEPTION_HPP

#include "Enums/ExitCodes.hpp"
#include <exception> // std::exception
#include <string>    // std::string

namespace OmegaL4Scanner::Exceptions
{
    /**
     * @class OmegaBaseException
     * @brief Exception class for handling errors in the OMEGA L4 Scanner project.
     */
    class OmegaBaseException : public std::exception {
    public:
        /**
         * @brief Constructor for OmegaBaseException.
         * @param code The error code.
         * @param message The error message.
         * @param detail Additional details about the error.
         */
        OmegaBaseException(Enums::ExitCodes code, std::string message, std::string detail = "");

        /**
         * @brief Returns the error message.
         * @return The error message as a C-style string.
         */
        [[nodiscard]]
        const char *what() const noexcept override;

        /**
         * @brief Returns the error code as integer.
         * @return The error code value.
         */
        [[nodiscard]]
        int code() const noexcept;

        /**
         * @brief Returns additional details about the error.
         * @return The error details as a string.
         */
        [[nodiscard]]
        std::string detail() const noexcept;

    private:
        const Enums::ExitCodes mCode; /**< The error code.                     */
        const std::string mMessage;   /**< The error message.                  */
        std::string mDetail;          /**< Additional details about the error. */
    }; // OmegaBaseException
} // OmegaL4Scanner::Exceptions

#endif // OMEGA_BASE_EXCEPTION_HPP

/*** end of file OmegaBaseException.hpp ***/
