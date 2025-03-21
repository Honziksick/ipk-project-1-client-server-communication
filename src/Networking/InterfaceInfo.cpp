/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         InterfaceInfo.cpp                                             *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      19.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the InterfaceInfo class, which              *
 *               provides information about network interfaces.                *
 *                                                                             *
 ******************************************************************************/
/**
 * @file InterfaceInfo.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation file for the InterfaceInfo class.
 */

#include "Networking/InterfaceInfo.hpp"
#include <string>  // std::string
#include <utility> // std::move

using namespace std;

namespace OmegaL4Scanner::Networking
{
    InterfaceInfo::InterfaceInfo() : mFlags{0} {}

    InterfaceInfo::InterfaceInfo(string interfaceName)
        : mName{move(interfaceName)}, mFlags{0} {}
} // OmegaL4Scanner::Networking

/*** end of file InterfaceInfo.cpp ***/
