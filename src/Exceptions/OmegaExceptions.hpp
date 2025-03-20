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
 * Last edit:    19.03.2025                                                    *
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
         */
        explicit HelpRequestedException(std::string detail = "");
    }; // HelpRequestedException

    /**
     * @class InerfacePrintRequestedException
     * @brief Thrown when interfaces are requested to be printed.
     */
    class InerfacePrintRequestedException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for InerfacePrintRequestedException.
         */
        explicit InerfacePrintRequestedException(std::string detail = "");
    }; // InerfacePrintRequestedException

    /**
     * @class InvalidArgumentException
     * @brief Exception class for invalid arguments.
     */
    class InvalidArgumentException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for InvalidArgumentException.
         */
        explicit InvalidArgumentException(std::string detail = "");
    }; // InvalidArgumentException

    /**
     * @class InvalidInterfaceException
     * @brief Exception class for invalid interfaces.
     */
    class InvalidInterfaceException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for InvalidInterfaceException.
         */
        explicit InvalidInterfaceException(std::string detail = "");
    }; // InvalidInterfaceException

    /**
     * @class InvalidPortRangeException
     * @brief Exception class for invalid port ranges.
     */
    class InvalidPortRangeException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for InvalidPortRangeException.
         */
        explicit InvalidPortRangeException(std::string detail = "");
    }; // InvalidPortRangeException

    /**
     * @class HostnameException
     * @brief Exception class for invalid hostnames.
     */
    class HostnameException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for HostnameException.
         */
        explicit HostnameException(std::string detail = "");
    }; // HostnameException

    /**
     * @class SocketException
     * @brief Exception class for socket errors.
     */
    class SocketException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for SocketException.
         */
        explicit SocketException(std::string detail = "");
    }; // SocketException

    /**
     * @class PcapException
     * @brief Exception class for pcap errors.
     */
    class PcapException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for PcapException.
         */
        explicit PcapException(std::string detail = "");
    }; // PcapException

    /**
     * @class UserInterruptionException
     * @brief Exception class for user interruptions.
     */
    class UserInterruptionException final : public OmegaBaseException {
    public:
        /**
         * @brief Constructor for UserInterruptionException.
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
         * @param message Error message of the original exception.
         */
        explicit UknownErrorException(const char *message, std::string detail = "");
    }; // UknownErrorException
} // OmegaL4Scanner::Exceptions

#endif // OMEGA_EXCEPTIONS_HPP

/*** end of file OmegaExceptions.hpp ***/
