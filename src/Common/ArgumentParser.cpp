/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         ArgumentParser.cpp                                            *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      14.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Implementation of the ArgumentParser class, which is          *
 *               responsible for parsing command line arguments and options.   *
 *                                                                             *
 ******************************************************************************/
/**
 * @file ArgumentParser.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation of the ArgumentParser class.
 */

#include "Common/ArgumentParser.hpp"
#include "Common/OmegaDataTypes.hpp"
#include "Exceptions/OmegaExceptions.hpp"
#include <ranges>   // std::ranges::sort, std::ranges::unique
#include <variant>  // std::holds_alternative
#include <utility>  // std::pair, std::make_pair
#include <tuple>    // std::get
#include <regex>    // std::regex

using namespace OmegaL4Scanner::Exceptions;
using namespace std;

constexpr int PORT_MIN{1};     /**< Minimum valid port number. */
constexpr int PORT_MAX{65535}; /**< Maximum valid port number. */

namespace OmegaL4Scanner::Common
{
    CommandLineOptions ArgumentParser::parseArguments(const int argc, char *argv[]) {
        CommandLineOptions options;  // Instance of CommandLineOptions to store parsed arguments

        // Create an instance of the CLI11 application
        CLI::App app{"OMEGA L4 Scanner"};

        // Local variables to store unprocessed inputs (ports, timeout)
        string tcpPortsStr;
        string udpPortsStr;
        int timeout{-1};  // -1 indicates no explicit timeout set

        // Set up the CLI11 application
        setupCliApp(app, options, tcpPortsStr, udpPortsStr, timeout);

        // Attempt to parse and validate the arguments
        try {
            app.parse(argc, argv);
        }
        catch(const CLI::CallForHelp &e) {
            if(const auto exitCode{app.exit(e)}; exitCode == EXIT_SUCCESS) {
                throw HelpRequestedException("Help successfully printed.");
            }
            else {
                throw InternalErrorException(
                        "CLI11 library returned error while parsing "
                        "command line arguments: " + string(e.what())
                        );
            }
        }
        catch(const CLI::Error &e) {
            throw InvalidArgumentException(string(e.what()));
        }

        // If object state after parsing is same as initial state, user wants to print interfaces
        if(options == CommandLineOptions{}) {
            throw InterfacePrintRequestedException("No arguments or undefined '-i' were given.");
        }

        // If the user provided an interface but nothing else, throw an error
        if(options.mTcpPorts.empty() && options.mUdpPorts.empty() &&
            options.mTarget.empty() && !options.mInterfaceName.empty()) {
            throw InvalidArgumentException("No target (hostname or IP address) specified.");
        }

        // Parse the port ranges from the input strings
        options.mTcpPorts = parsePortRange(tcpPortsStr);
        options.mUdpPorts = parsePortRange(udpPortsStr);

        // Convert the timeout value to milliseconds if it's set
        if(timeout >= 0) {
            options.mWaitTimeout = convertIntToMilliseconds(timeout);
        }

        validateOptions(options); // Validate the parsed options
        return options;
    } // ArgumentParser::parseArguments()

    void ArgumentParser::setupCliApp(CLI::App &app, CommandLineOptions &options,
                                     string &tcpPorts, string &udpPorts,
                                     int &timeout) {
        app.set_help_flag("-h,--help", "Display help message");

        const auto interfaceOpt =
                app.add_option("-i,--interface", options.mInterfaceName,
                               "Network interface to scan through")
                   ->expected(0, 1)
                   ->required(false);

        const auto targetOpt =
                app.add_option("target", options.mTarget,
                               "Target hostname or IP address")
                   ->needs(interfaceOpt)
                   ->required(false);

        app.add_option("-t,--pt", tcpPorts,
                       "Comma-separated TCP ports or port ranges")
           ->needs(interfaceOpt)
           ->needs(targetOpt)
           ->required(false);

        app.add_option("-u,--pu", udpPorts,
                       "Comma-separated UDP ports or port ranges")
           ->needs(interfaceOpt)
           ->needs(targetOpt)
           ->required(false);

        app.add_option("-w,--wait", timeout,
                       "Timeout in milliseconds to wait for a response for a single port scan")
           ->needs(interfaceOpt)
           ->needs(targetOpt)
           ->check(CLI::PositiveNumber)
           ->required(false);
    } // ArgumentParser::setupCliApp()

    void ArgumentParser::validateOptions(const CommandLineOptions &options) {
        // If the target is not specified, assume the list of active interfaces
        // should be displayed (done elsewere).
        if(options.mTarget.empty()) {
            return;
        }

        // If the target is specified (i.e., scanning should be performed),
        // the interface must also be specified.
        if(!validateInterfaceName(options.mInterfaceName)) {
            throw InvalidArgumentException(
                    "Invalid network interface specified: " + options.mInterfaceName
                    );
        }

        // If the target is specified, at least one of the ports (TCP/UDP) must be provided.
        if(options.mTcpPorts.empty() && options.mUdpPorts.empty()) {
            throw InvalidArgumentException(
                    "Target provided but no port ranges specified for scanning."
                    );
        }

        // Check if the given hostname or IP address is valid
        if(!validateHostOrIpAddress(options.mTarget)) {
            throw InvalidArgumentException("Invalid hostname or IP address: " + options.mTarget);
        }
    } // ArgumentParser::validateOptions()

    vector<PortRange> ArgumentParser::parsePortRange(const string &rawPortRange) {
        const vector<string> substrings = splitString(rawPortRange, ',');
        vector<PortRange> ports;

        for(const auto &substring : substrings) {
            // Check if the port value is in range 1-65535
            if(!validatePortRange(substring)) {
                throw InvalidArgumentException("Invalid port or port range: " + substring);
            }
            // If the substring represents a port range
            if(substring.find('-') != string::npos) {
                vector<string> range = splitString(substring, '-');
                int beginPort, endPort;
                try {
                    beginPort = stoi(range[0]);
                    endPort = stoi(range[1]);
                }
                catch(const invalid_argument &e) {
                    throw InvalidArgumentException("Invalid port number: " + string(e.what()));
                } catch(const out_of_range &e) {
                    throw InvalidArgumentException("Port number out of range: " + string(e.what()));
                }

                ports.emplace_back(make_pair(beginPort, endPort));
            }
            // If the substring represents a single port
            else {
                int singlePort;
                try {
                    singlePort = stoi(substring);
                }
                catch(const invalid_argument &e) {
                    throw InvalidArgumentException("Invalid port number: " + string(e.what()));
                } catch(const out_of_range &e) {
                    throw InvalidArgumentException("Port number out of range: " + string(e.what()));
                }
                ports.emplace_back(singlePort);
            }
        }
        return mergeAndSortPortRanges(ports);
    } // ArgumentParser::parsePortRange()

    // Implementation inspired by https://www.geeksforgeeks.org/how-to-split-string-by-delimiter-in-cpp/
    vector<string> ArgumentParser::splitString(const string &originalString, const char delimiter) {
        vector<string> substrings;
        stringstream stringStream{originalString};
        string token;

        while(getline(stringStream, token, delimiter)) {
            substrings.emplace_back(token);
        }
        return substrings;
    } // ArgumentParser::splitString()

    bool ArgumentParser::validatePortRange(const string &portRange) {
        // Regular expression to check for a port or port range
        const regex portRegex(R"(^(\d+)(-(\d+))?$)");

        // Check if portRange matches the regular expression and is valid
        if(smatch match; regex_match(portRange, match, portRegex)) {
            const int beginPort = stoi(match[1].str()); // get the starting port

            // If a port range is specified
            if(match[3].matched) {
                const int endPort = stoi(match[3].str()); // get the ending port

                // Check if the ports are within the valid range, and the
                // begining port is not greater than the ending port
                return (beginPort >= PORT_MIN) && (endPort <= PORT_MAX) && (beginPort <= endPort);
            }

            // Check if the single port is within the valid range
            return (beginPort >= PORT_MIN) && (beginPort <= PORT_MAX);
        }
        return false;
    } // ArgumentParser::validatePortRange()

    bool ArgumentParser::validateHostOrIpAddress(const string &hostOrIpAddress) {
        // Regular expression for validating hostnames
        const regex
                hostnameRegex(R"(^([A-Za-z0-9](?:[A-Za-z0-9-]{0,61}[A-Za-z0-9])?\.)+[A-Za-z]{2,63}\.?$)");

        // Regular expression for validating IPv4 addresses
        // Source: https://ihateregex.io/expr/ip/
        const regex
                ipv4Regex(R"(^(?!0\.0\.0\.0$)(\b25[0-5]|\b2[0-4][0-9]|\b[01]?[0-9][0-9]?)(\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)){3}$)");

        // Regular expression for validating IPv6 addresses
        // Source: https://ihateregex.io/expr/ipv6/
        const regex
                ipv6Regex(
                        R"(^(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))$)");

        // Regular expression for validating localhost
        const regex
                localhostRegex(R"(^(localhost)(?::(?:[1-9]\d{0,3}|[1-5]\d{4}|6[0-4]\d{3}|65[0-4]\d{2}|655[0-2]\d|6553[0-5]))?$)");

        return regex_match(hostOrIpAddress, hostnameRegex) ||
                regex_match(hostOrIpAddress, ipv4Regex) ||
                regex_match(hostOrIpAddress, ipv6Regex) ||
                regex_match(hostOrIpAddress, localhostRegex);
    } // ArgumentParser::validateHostOrIpAddress()

    bool ArgumentParser::validateInterfaceName(const string &interfaceName) {
        // Regular expression for validating interface names according to IFNAMSIZ
        const regex interfaceNameRegex("^[A-Za-z0-9_.:-]{1,15}$");

        // Check if the interface name matches the regular expression
        return regex_match(interfaceName, interfaceNameRegex);
    } // ArgumentParser::validateInterfaceName()

    vector<PortRange> ArgumentParser::mergeAndSortPortRanges(const vector<PortRange> &portRanges) {
        vector<pair<int, int>> intervals;       // Vector for intervals (pair<int, int>)
        intervals.reserve(portRanges.size()); // Memory reservation for better performance
        vector<pair<int, int>> merged;          // Vector for merged intervals
        vector<PortRange> result;               // vector for intervals converted back to PortRange

        // First we need to convert all items to intervals (pair<int, int>)
        for(const auto &range : portRanges) {
            // If the range is an int, convert it to an interval (port, port)
            if(holds_alternative<int>(range)) {
                int port = get<int>(range);
                intervals.emplace_back(port, port);
            }
            // Else the range is a pair<int, int>, so we can add it directly
            else {
                intervals.emplace_back(get<pair<int, int>>(range));
            }
        }

        // Now we can sort intervals by start and end values
        ranges::sort(intervals, [](const auto &a, const auto &b) {
            // If they have the same start value, sort by end value
            return (a.first == b.first) ? (a.second < b.second) : (a.first < b.first);
        });

        // Merge adjacent or overlapping intervals
        for(const auto &interval : intervals) {
            // If 'merged' is empty or the interval is not adjacent, add a new interval
            if(merged.empty() || (interval.first > merged.back().second + 1)) {
                merged.emplace_back(interval);
            }
            // Else the interval is adjacent or overlapping, so we update the end value
            else {
                merged.back().second = max(merged.back().second, interval.second);
            }
        }

        // Memory reservation for better performance
        result.reserve(merged.size());

        // Conversion back to PortRange
        for(const auto &[first, second] : merged) {
            if(first == second) {
                result.emplace_back(first);
            }
            else {
                result.emplace_back(make_pair(first, second));
            }
        }
        return result;
    } // ArgumentParser::mergeAndSortPortRanges()

    chrono::milliseconds ArgumentParser::convertIntToMilliseconds(const int duration) {
        return chrono::milliseconds(duration);
    } // ArgumentParser::convertIntToMilliseconds()
} // OmegaL4Scanner::Common

/*** end of file ArgumentParser.cpp ***/
