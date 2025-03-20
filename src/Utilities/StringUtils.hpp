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
