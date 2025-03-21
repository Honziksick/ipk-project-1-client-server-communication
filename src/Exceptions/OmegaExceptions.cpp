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
 * Last edit:    20.03.2025                                                    *
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
#include "Constants/ExceptionMessages.hpp"
#include "Enums/ExitCodes.hpp"

using namespace OmegaL4Scanner::Constants;
using namespace OmegaL4Scanner::Enums;
using namespace std;

namespace OmegaL4Scanner::Exceptions
{
    HelpRequestedException::HelpRequestedException(string detail)
        : OmegaBaseException{
            ExitCodes::SUCESS,
            helpRequestedMsg,
            move(detail)
        } {}

    InterfacePrintRequestedException::InterfacePrintRequestedException(string detail)
        : OmegaBaseException{
            ExitCodes::SUCESS,
            interfacePrintRequestedMsg,
            move(detail)
        } {}

    InvalidArgumentException::InvalidArgumentException(string detail)
        : OmegaBaseException{
            ExitCodes::INVALID_ARGUMENT_ERROR,
            invalidArgumentErrorMsg,
            move(detail)
        } {}

    InterfaceErrorException::InterfaceErrorException(string detail)
        : OmegaBaseException{
            ExitCodes::INTERFACE_ERROR,
            interfaceErrorMsg,
            move(detail)
        } {}

    SocketErrorException::SocketErrorException(string detail)
        : OmegaBaseException{
            ExitCodes::SOCKET_ERROR,
            socketErrorMsg,
            move(detail)
        } {}

    PcapErrorException::PcapErrorException(string detail)
        : OmegaBaseException{
            ExitCodes::PCAP_ERROR,
            pcapErrorMsg,
            move(detail)
        } {}

    UserInterruptionException::UserInterruptionException(string detail)
        : OmegaBaseException{
            ExitCodes::USER_INTERRUPTION_ERROR,
            userInterruptionMsg,
            move(detail)
        } {}

    InternalErrorException::InternalErrorException(string detail)
        : OmegaBaseException{
            ExitCodes::INTERNAL_ERROR,
            internalErrorMsg,
            move(detail)
        } {}

    UknownErrorException::UknownErrorException(string detail)
        : OmegaBaseException{
            ExitCodes::UNKNOWN_ERROR,
            unknownErrorMsg,
            move(detail)
        } {}
} // OmegaL4Scanner::Exceptions

/*** end of file OmegaExceptions.cpp ***/
