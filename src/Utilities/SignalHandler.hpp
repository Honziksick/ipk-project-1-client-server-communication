/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         SignalHandler.hpp                                             *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      22.03.2025                                                    *
 * Last edit:    22.03.2025                                                    *
 *                                                                             *
 * Description:  Declaration of the SignalHandler class, which is              *
 *               responsible for handling system signals.                      *
 *                                                                             *
 ******************************************************************************/
/**
 * @file SignalHandler.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Header file for the SignalHandler class.
 */

#ifndef SIGNAL_HANDLER_HPP
#define SIGNAL_HANDLER_HPP

namespace OmegaL4Scanner::Utilities
{
    /**
     * @class SignalHandler
     * @brief Provides functionality for handling system signals (e.g. SIGINT).
     */
    class SignalHandler final {
    public:
        /**
         * @brief Registers the necessary signal handlers.
         */
        static void registerHandlers();

    private:
        /**
         * @brief Signal handling function.
         *
         * @param signal The received signal.
         */
        static void handleSignal(int signal);
    }; // SignalHandler
} // OmegaL4Scanner::Utilities

#endif // SIGNAL_HANDLER_HPP

/*** end of file SignalHandler.hpp ***/
