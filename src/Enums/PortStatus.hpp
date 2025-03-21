/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         PortStatus.hpp                                                *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    12.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the PortStatus enum, which is used to          *
 *               represent the status of a port in the OMEGA L4 Scanner.       *
 *                                                                             *
 ******************************************************************************/
/**
 * @file PortStatus.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the PortStatus enum.
 */

#ifndef PORT_STATUS_HPP
#define PORT_STATUS_HPP

namespace OmegaL4Scanner::Enums
{
    /**
     * @enum PortStatus
     * @brief Enum class representing the status of a port in the OMEGA L4 Scanner.
     *
     * @details This enum class defines the possible statuses of a port, which
     *          can be open, closed, or filtered.
     */
    enum class PortStatus {
        OPEN     = 0,   /**< The port is open.     */
        CLOSED   = 1,   /**< The port is closed.   */
        FILTERED = 2,   /**< The port is filtered. */
    }; // PortStatus
} // OmegaL4Scanner::Enums

#endif // PORT_STATUS_HPP

/*** end of file PortStatus.hpp ***/
