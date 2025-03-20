/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaExceptions.cpp                                           *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    19.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation file for the OmegaBaseException class used in  *
 *               the OMEGA L4 Scanner project.                                 *
 *                                                                             *
 ******************************************************************************/
/**
 * @file OmegaExceptions.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the OmegaExceptions classes.
 */

#include "Exceptions/OmegaExceptions.hpp"
#include "Exceptions/ExceptionMessages.hpp"
#include "Enums/ExitCodes.hpp"

using namespace OmegaL4Scanner::Enums;
using namespace std;

namespace OmegaL4Scanner::Exceptions
{
    HelpRequestedException::HelpRequestedException(string detail) :
        OmegaBaseException{
            ExitCodes::HELP_REQUESTED,
            helpRequestedMsg,
            move(detail)
        } {}

    InterfacePrintRequestedException::InterfacePrintRequestedException(string detail) :
        OmegaBaseException{
            ExitCodes::INTERFACE_PRINT_REQUESTED,
            interfacePrintRequestedMsg,
            move(detail)
        } {}

    InvalidArgumentException::InvalidArgumentException(string detail) :
        OmegaBaseException{
            ExitCodes::INVALID_ARGUMENT_ERROR,
            invalidArgumentErrMsg,
            move(detail)
        } {}

    InvalidInterfaceException::InvalidInterfaceException(string detail) :
        OmegaBaseException{
            ExitCodes::INVALID_INTERFACE_ERROR,
            invalidInterfaceErrMsg,
            move(detail)
        } {}

    InvalidPortRangeException::InvalidPortRangeException(string detail) :
        OmegaBaseException{
            ExitCodes::INVALID_PORT_RANGE_ERROR,
            invalidPortRangeErrMsg,
            move(detail)
        } {}

    HostnameException::HostnameException(string detail) :
        OmegaBaseException{
            ExitCodes::INVALID_HOSTNAME_ERROR,
            invalidHostnameErrMsg,
            move(detail)
        } {}

    SocketException::SocketException(string detail) :
        OmegaBaseException{
            ExitCodes::SOCKET_ERROR,
            socketErrMsg,
            move(detail)
        } {}

    PcapException::PcapException(string detail) :
        OmegaBaseException{
            ExitCodes::PCAP_ERROR,
            pcapErrMsg,
            move(detail)
        } {}

    UserInterruptionException::UserInterruptionException(string detail) :
        OmegaBaseException{
            ExitCodes::USER_INTERRUPTION_ERROR,
            userInterruptionErrMsg,
            move(detail)
        } {}

    InternalErrorException::InternalErrorException(string detail) :
        OmegaBaseException{
            ExitCodes::INTERNAL_ERROR,
            internalErrMsg,
            move(detail)
        } {}

    UknownErrorException::UknownErrorException(const char *message, string detail) :
        OmegaBaseException{
            ExitCodes::UNKNOWN_ERROR,
            message,
            move(detail)
        } {}
} // OmegaL4Scanner::Exceptions

/*** end of file OmegaExceptions.cpp ***/
