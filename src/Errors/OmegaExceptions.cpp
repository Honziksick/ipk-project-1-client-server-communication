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
 * Last edit:    13.03.2025                                                    *
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

#include "Errors/OmegaExceptions.hpp"
#include "Errors/ErrorMessage.hpp"
#include "Enums/ErrorCode.hpp"

using namespace OmegaL4Scanner::Enums;
using namespace std;

namespace OmegaL4Scanner::Errors
{
    InvalidArgumentException::InvalidArgumentException(string detail) :
        OmegaBaseException{
            ErrorCode::INVALID_ARGUMENT_ERROR,
            invalidArgumentErrMsg,
            move(detail)
        } {}

    InvalidInterfaceException::InvalidInterfaceException(string detail) :
        OmegaBaseException{
            ErrorCode::INVALID_INTERFACE_ERROR,
            invalidInterfaceErrMsg,
            move(detail)
        } {}

    InvalidPortRangeException::InvalidPortRangeException(string detail) :
        OmegaBaseException{
            ErrorCode::INVALID_PORT_RANGE_ERROR,
            invalidPortRangeErrMsg,
            move(detail)
        } {}

    HostnameException::HostnameException(string detail) :
        OmegaBaseException{
            ErrorCode::INVALID_HOSTNAME_ERROR,
            invalidHostnameErrMsg,
            move(detail)
        } {}

    SocketException::SocketException(string detail) :
        OmegaBaseException{
            ErrorCode::SOCKET_ERROR,
            socketErrMsg,
            move(detail)
        } {}

    PcapException::PcapException(string detail) :
        OmegaBaseException{
            ErrorCode::PCAP_ERROR,
            pcapErrMsg,
            move(detail)
        } {}

    UserInterruptionException::UserInterruptionException(string detail) :
        OmegaBaseException{
            ErrorCode::USER_INTERRUPTION_ERROR,
            userInterruptionErrMsg,
            move(detail)
        } {}

    InternalErrorException::InternalErrorException(string detail) :
        OmegaBaseException{
            ErrorCode::INTERNAL_ERROR,
            internalErrMsg,
            move(detail)
        } {}

    UknownErrorException::UknownErrorException(const char *message, string detail) :
    OmegaBaseException{
        ErrorCode::UNKNOWN_ERROR,
        message,
        move(detail)
    } {}
} // OmegaL4Scanner::Errors

/*** end of file OmegaExceptions.cpp ***/
