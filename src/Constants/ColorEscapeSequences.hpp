/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ColorEscapeSequences.hpp                                      *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      12.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description: This file contains the definition of color escape sequences    *
 *              used for formatting text output in the OMEGA L4 Scanner        *
 *              project.                                                       *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ColorEscapeSequences.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief This file defines constants for color escape sequences used in
 *        text formatting.
 */

#ifndef COLOR_ESCAPE_SEQUENCES_HPP
#define COLOR_ESCAPE_SEQUENCES_HPP

namespace OmegaL4Scanner::Constants
{
    // General formatting escape sequences
    inline const auto RESET = "\033[0m";    /**< Reset all attributes. */
    inline const auto FORMAT_BOLD = "\033[1m";    /**< Bold text format.     */

    // Foreground colors
    inline const auto COLOR_RED = "\033[31m";   /**< Red text color.      */
    inline const auto COLOR_GREEN = "\033[32m";   /**< Green text color.    */
    inline const auto COLOR_YELLOW = "\033[33m";   /**< Yellow text color.   */
    inline const auto COLOR_MAGENTA = "\033[35m";   /**< Magenta text color.  */
    inline const auto COLOR_CYAN = "\033[36m";   /**< Cyan text color.     */
} // OmegaL4Scanner::Constants

#endif // COLOR_ESCAPE_SEQUENCES_HPP

/*** end of file ColorEscapeSequences.hpp ***/
