/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ProtocolTypes.hpp                                             *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      21.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description:  Header file defining protocol types constants for the         *
 *               OMEGA L4 Scanner project.                                     *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ProtocolTypes.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for protocol types constants.
 */

#ifndef PROTOCOL_CONSTANTS_HPP
#define PROTOCOL_CONSTANTS_HPP

#include "Common/OmegaDataTypes.hpp"

namespace OmegaL4Scanner::Constants
{
    inline const Common::Protocol TCP = "tcp";    /**< Constant for TCP protocol type. */
    inline const Common::Protocol UDP = "udp";    /**< Constant for UDP protocol type. */
} // OmegaL4Scanner::Constants

#endif // PROTOCOL_CONSTANTS_HPP

/*** end of file ProtocolTypes.hpp ***/
