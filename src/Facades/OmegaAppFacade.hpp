/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         OmegaAppFacade.hpp                                            *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    21.03.2025                                                    *
 *                                                                             *
 * Description: This file contains the declaration of the OmegaAppFacade       *
 *              class, which serves as a facade for the OMEGA L4 Scanner       *
 *              application. The facade pattern is used to provide a           *
 *              simplified interface to a complex subsystem.                   *
 *                                                                             *
 ******************************************************************************/
/**
 * @file OmegaAppFacade.hpp
 * @author Jan Kalina <xkalinj00>
 * @brief Header file for the OmegaAppFacade class.
 */

#ifndef OMEGA_APP_FACADE_HPP
#define OMEGA_APP_FACADE_HPP

#include "Common/CommandLineOptions.hpp"
#include "Networking/InterfaceInfo.hpp"

namespace OmegaL4Scanner::Facades
{
    /**
     * @class OmegaAppFacade
     * @brief Facade class for the Omega L4 Scanner application.
     *
     * @details
     * This class provides a single entry point (runScan) that initializes
     * the application:
     * - Parses command line arguments
     * - Retrieves network interface information (or prints active interfaces
     *   if none is specified)
     * - Simulates the scanning process.
     */
    class OmegaAppFacade final {
    public:
        /**
         * @brief Constructs the OmegaAppFacade.
         */
        OmegaAppFacade();

        /**
         * @brief Initializes options, sets up the network interface, and runs
         *        the L4 scan.
         *
         * @details
         * This method performs all necessary steps:
         * - parsing command line arguments,
         * - printing the list of active interfaces on user request,
         * - initializing network interface information,
         * - invoking the port scanning process via ScannerController.
         *
         * @param argc Number of command line arguments.
         * @param argv Array of argument strings.
         */
        void runScan(int argc, char *argv[]);

    private:
        Common::CommandLineOptions mCommandLineOptions;   /**< Parsed command line options.                      */
        Networking::InterfaceInfo mInterfaceInfo;         /**< Information about the selected network interface. */

        /**
         * @brief Initializes command line options by parsing the input arguments.
         *
         * @param argc Number of command line arguments.
         * @param argv Array of argument strings.
         */
        void getCommandLineOptions(int argc, char *argv[]);

        /**
         * @brief Initializes the network interface.
         *
         * If no interface was specified in the options, the method prints a list
         * of active interfaces and terminates the application. Otherwise, it retrieves
         * the information of the specified interface.
         */
        void getInterfaceInfo();

        /**
        * @brief Scans the L4 layer of the network.
        *
        * @details This method performs the scanning of the L4 layer using
        *          the initialized command line options and network interface
        *          information. The results of the scan are processed and
        *          printed to the standard output.
        */
        void scanL4Layer() const;
    }; // OmegaAppFacade
} // OmegaL4Scanner::Facades

#endif // OMEGA_APP_FACADE_HPP

/*** end of file OmegaAppFacade.hpp ***/
