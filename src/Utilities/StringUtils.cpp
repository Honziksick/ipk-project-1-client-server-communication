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
