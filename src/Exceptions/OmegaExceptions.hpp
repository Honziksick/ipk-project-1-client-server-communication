/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaExceptions.hpp                                           *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description:  Header file for the OmegaExceptions classes used in the       *
 *               OMEGA L4 Scanner project.                                     *
 *                                                                             *
 ******************************************************************************/
/**
 * @file OmegaExceptions.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the OmegaExceptions classes.
 */

#ifndef OMEGA_EXCEPTIONS_HPP
#define OMEGA_EXCEPTIONS_HPP

#include "Exceptions/OmegaBaseException.hpp"
#include <string> // std::string

namespace OmegaL4Scanner::Exceptions
{
    /**
     * @class HelpRequestedException
     * @brief Exception class for user requesting help.
     */
    class HelpRequestedException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for HelpRequestedException.
         * @param detail Additional information if needed.
         */
        explicit HelpRequestedException(std::string detail = "");
    }; // HelpRequestedException

    /**
     * @class InterfacePrintRequestedException
     * @brief Thrown when interfaces are requested to be printed.
     */
    class InterfacePrintRequestedException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for InterfacePrintRequestedException.
         * @param detail Additional information if needed.
         */
        explicit InterfacePrintRequestedException(std::string detail = "");
    }; // InterfacePrintRequestedException

    /**
     * @class InvalidArgumentException
     * @brief Exception class for invalid arguments.
     */
    class InvalidArgumentException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for InvalidArgumentException.
         * @param detail Additional information about the error.
         */
        explicit InvalidArgumentException(std::string detail = "");
    }; // InvalidArgumentException

    /**
     * @class InterfaceErrorException
     * @brief Exception class for invalid interfaces.
     */
    class InterfaceErrorException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for InterfaceErrorException.
         * @param detail Additional information about the error.
         */
        explicit InterfaceErrorException(std::string detail = "");
    }; // InterfaceErrorException

    /**
     * @class HostnameResolutionErrorException
     * @brief Exception class for hostname resolution error.
     */
    class HostnameResolutionErrorException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for HostnameResolutionErrorException.
         * @param detail Additional information about the error.
         */
        explicit HostnameResolutionErrorException(std::string detail = "");
    }; // HostnameResolutionErrorException

    /**
     * @class SocketErrorException
     * @brief Exception class for socket errors.
     */
    class SocketErrorException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for SocketErrorException.
         * @param detail Additional information about the error.
         */
        explicit SocketErrorException(std::string detail = "");
    }; // SocketErrorException

    /**
     * @class LibnetErrorException
     * @brief Exception class for libnet errors.
     */
    class LibnetErrorException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for LibnetErrorException.
         * @param detail Additional information about the error.
         */
        explicit LibnetErrorException(std::string detail = "");
    }; // LibnetErrorException

    /**
     * @class UserInterruptionException
     * @brief Exception class for user interruptions.
     */
    class UserInterruptionException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for UserInterruptionException.
         * @param detail Additional information about the error.
         */
        explicit UserInterruptionException(std::string detail = "");
    }; // UserInterruptionException

    /**
     * @class InternalErrorException
     * @brief Exception class for internal errors.
     */
    class InternalErrorException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for InternalErrorException.
         * @param detail Additional information about the error.
         */
        explicit InternalErrorException(std::string detail = "");
    }; // InternalErrorException

    /**
     * @class UknownErrorException
     * @brief Exception class for unknown errors.
     */
    class UknownErrorException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for UknownErrorException.
         * @param detail Additional information about the error.
         */
        explicit UknownErrorException(std::string detail = "");
    }; // UknownErrorException
} // OmegaL4Scanner::Exceptions

#endif // OMEGA_EXCEPTIONS_HPP

/*** end of file OmegaExceptions.hpp ***/
