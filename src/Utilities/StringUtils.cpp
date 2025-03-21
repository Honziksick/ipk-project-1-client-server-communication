/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         StringUtils.cpp                                               *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    21.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the implementation of the StringUtils      *
 *               class, which provides utility functions for string            *
 *               operations.                                                   *
 *                                                                             *
 ******************************************************************************/
/**
 * @file StringUtils.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation of the StringUtils class for string operations.
 */

#include "StringUtils.hpp"
#include <string>    // std::string
#include <algorithm> // std::transform
#include <cctype>    // std::tolower

using namespace std;

namespace OmegaL4Scanner::Utilities
{
    // Soruce: https://stackoverflow.com/a/313990
    string StringUtils::toLower(const string &str) {
        string lowerCaseString = str;
        ranges::transform(lowerCaseString, lowerCaseString.begin(),
                          [](const unsigned char character) {
                              return tolower(character);
                          });
        return lowerCaseString;
    } // StringUtils::toLower()
} // OmegaL4Scanner::Utilities

/*** end of file StringUtils.cpp ***/
