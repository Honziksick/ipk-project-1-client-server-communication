/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaDataTypes.hpp                                            *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      15.03.2025                                                    *
 * Last edit:    15.03.2025                                                    *
 *                                                                             *
 * Description:  Header file for custom data types used in the OMEGA L4        *
 *               Scanner project.                                              *
 *                                                                             *
 ******************************************************************************/
/**
 * @file OmegaDataTypes.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for custom data types used in the OMEGA L4 Scanner project.
 */

#ifndef OMEGA_DATA_TYPES_HPP
#define OMEGA_DATA_TYPES_HPP

#include <variant>  // std::variant<T...>
#include <utility>  // std::pair<T1, T2>

namespace OmegaL4Scanner::Common
{
    /**
     * @brief Type definition for port number or port range.
     *
     * @details This type can represent either a single port (int),
     *          or a range of ports (std::pair<int, int>).
     */
    using PortRange = std::variant<int, std::pair<int, int>>;

} // OmegaL4Scanner::Common

#endif //OMEGA_DATA_TYPES_HPP

/*** end of file OmegaDataTypes.hpp ***/
