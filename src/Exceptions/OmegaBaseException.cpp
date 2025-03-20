/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaBaseException.cpp                                        *
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
 * @file OmegaBaseException.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the OmegaBaseException class.
 */

#include "Exceptions/OmegaBaseException.hpp"
#include <utility>  // std::move

using namespace std;

namespace OmegaL4Scanner::Exceptions
{
    OmegaBaseException::OmegaBaseException(const Enums::ExitCodes code, string message, string detail)
        : mCode{code}, mMessage{move(message)}, mDetail{move(detail)} {}

    const char *OmegaBaseException::what() const noexcept {
        return mMessage.c_str();
    } // OmegaBaseException::what()

    int OmegaBaseException::code() const noexcept {
        return static_cast<int>(mCode);
    } // OmegaBaseException::code()

    string OmegaBaseException::detail() const noexcept {
        return mDetail;
    } // OmegaBaseException::detail()
} // OmegaL4Scanner::Exceptions

/*** end of file OmegaBaseException.cpp ***/
