/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         CommandLineOptions.cpp                                        *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      14.03.2025                                                    *
 * Last edit:    15.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the CommandLineOptions class, which is      *
 *               responsible for parsing and storing command line options for  *
 *               the OMEGA L4 Scanner application.                             *
 *                                                                             *
 ******************************************************************************/
/**
 * @file CommandLineOptions.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation of the CommandLineOptions class.
 */

#include "Common/CommandLineOptions.hpp"

namespace OmegaL4Scanner::Common
{
    CommandLineOptions::CommandLineOptions() : mWaitTimeout{5000} {}
} // OmegaL4Scanner::Common

/*** end of file CommandLineOptions.cpp ***/
