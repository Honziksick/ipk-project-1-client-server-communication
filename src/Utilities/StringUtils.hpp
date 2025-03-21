/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         StringUtils.hpp                                               *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    21.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the declaration of the StringUtils class,  *
 *               which provides  utility functions for string operations.      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file StringUtils.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Declaration of the StringUtils class for string operations.
 */

#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string> // std::string

namespace OmegaL4Scanner::Utilities
{
    /**
     * @class StringUtils
     * @brief Utility class for string operations.
     */
    class StringUtils {
    public:
        /**
         * @brief Converts a string to lowercase.
         *
         * @param str The string to be converted.
         * @return The lowercase version of the input string.
         */
        static std::string toLower(const std::string &str);
    }; // StringUtils
} // OmegaL4Scanner::Utilities

#endif //STRING_UTILS_HPP

/*** end of file StringUtils.hpp ***/
