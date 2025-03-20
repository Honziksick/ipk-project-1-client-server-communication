/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ArgumentParser.hpp                                         *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      14.03.2025                                                    *
 * Last edit:    15.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the ArgumentParser class, which is          *
 *               responsible for parsing command line arguments and options.   *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ArgumentParser.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the ArgumentParser class.
 */

#ifndef COMMAND_LINE_PARSER_HPP
#define COMMAND_LINE_PARSER_HPP

#include "Common/CommandLineOptions.hpp"
#include "Common/OmegaDataTypes.hpp"
#include <string>     // std::string
#include <vector>     // std::vector<T>
#include <chrono>     // std::chrono::milliseconds
#include "CLI11.hpp"  /* CLI11 je header-only library for command-line parsing
                         Source: https://github.com/CLIUtils/CLI11 */

namespace OmegaL4Scanner::Common
{
    /**
     * @class ArgumentParser
     * @brief Class responsible for parsing command line arguments and options.
     */
    class ArgumentParser final {
    public:
        /**
         * @brief Parses command line arguments and returns a populated
         *        CommandLineOptions object.
         *
         * @param argc Number of arguments.
         * @param argv Array of argument strings.
         * @return CommandLineOptions instance initialized with parsed values.
         */
        static CommandLineOptions parseArguments(int argc, char *argv[]);

    private:
        /**
         * @brief Sets up the CLI11 application with the necessary options.
         *
         * @details This function configures the CLI11 application by adding
         *          the required command line options and arguments.
         *
         * @param app Reference to the CLI11 application.
         * @param options Reference to CommandLineOptions to store parsed values.
         * @param tcpPorts Reference to a string to store TCP ports.
         * @param udpPorts Reference to a string to store UDP ports.
         * @param timeout Reference to an integer to store the timeout value.
         */
        static void setupCliApp(CLI::App &app, CommandLineOptions &options,
                                std::string &tcpPorts, std::string &udpPorts,
                                int &timeout);

        /**
         * @brief Validates the parsed command line options.
         *
         * @details This function checks the validity of the provided command
         *          line options. It ensures that the options meet the required
         *          criteria and constraints.
         *
         * @param options The CommandLineOptions object containing the parsed
         *                command line options.
         */
        static void validateOptions(const CommandLineOptions &options);

        /**
         * @brief Parses a string containing port numbers and ranges into a
         *        vector of individual port numbers.
         *
         * @param rawPortRange The input string containing port numbers and ranges.
         * @return std::vector<PortRange> A vector of port numbers or port ranges.
         */
        static std::vector<PortRange> parsePortRange(const std::string &rawPortRange);

        /**
         * @brief Splits a string into substrings based on a delimiter character.
         *
         * @param originalString The input string to be split.
         * @param delimiter The character used to split the string.
         * @return std::vector<std::string> A vector containing the resulting substrings.
         */
        static std::vector<std::string> splitString(const std::string &originalString, char delimiter);

        /**
         * @brief Validates if the given string is a valid port or port range.
         *
         * @param portRange The string representing a port or port range.
         * @return bool True if the port range is valid, false otherwise.
         */
        static bool validatePortRange(const std::string &portRange);

        /**
         * @brief Validates if the given string is a valid hostname or IPv4/IPv6 adress.
         *
         * @details This function checks if the provided hostname string meets the
         *          criteria for a valid host. It ensures that the hostname or
         *          IP adress conforms to the required format and constraints.
         *
         * @param hostOrIpAddress The string representing the hostname/IP adress
         *                        to be validated.
         * @return bool True if the hostname is valid, false otherwise.
         */
        static bool validateHostOrIpAddress(const std::string &hostOrIpAddress);

        /**
         * @brief Validates if the given string is a valid network interface name.
         *
         * @details This function checks if the provided interface name string
         *          meets the criteria for a valid network interface name. It
         *          ensures that the interface name conforms to the required
         *          format and constraints.
         *
         * @note The maximum length of a network interface name is defined by
         *       IFNAMSIZ, which is typically 16 bytes (15 characters + terminator)
         *       and allows for alphanumeric characters, colons, underscores
         *       and hyphens.
         *
         * @param interfaceName The string representing the network interface name
         *                      to be validated.
         * @return bool True if the interface name is valid, false otherwise.
         */
        static bool validateInterfaceName(const std::string &interfaceName);

        /**
         * @brief Merges and sorts a vector of port ranges.
         *
         * @details This function takes a vector of port ranges, where each port
         *          range can be either a single port (int) or a range of ports
         *          (std::pair<int, int>). It converts all items to intervals,
         *          sorts them, merges adjacent or overlapping intervals, and
         *          converts them back to the original format.
         *
         * @param portRanges A vector of PortRange, where each PortRange is
         *                   either an int representing a single port or a
         *                   std::pair<int, int> representing a range of ports.
         * @return std::vector<PortRange> A vector of merged and sorted PortRange.
         */
        static std::vector<PortRange> mergeAndSortPortRanges(const std::vector<PortRange> &portRanges);

        /**
         * @brief Converts an integer value to std::chrono::milliseconds.
         * @details This function is used to convert the timeout value obtained
         *          from CLI arguments.
         *
         * @param duration Integer representing the number of milliseconds.
         * @return std::chrono::milliseconds timeout value in milliseconds.
         */
        static std::chrono::milliseconds convertIntToMilliseconds(int duration);
    }; // ArgumentParser
} // OmegaL4Scanner::Common

#endif // COMMAND_LINE_PARSER_HPP

/*** end of file ArgumentParser.hpp ***/
