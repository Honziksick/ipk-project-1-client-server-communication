/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         RandomNumberGenerator.hpp                                     *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      24.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the declaration of the                     *
 *               RandomNumberGenerator class, which provides utility           *
 *               functions for random number generation.                       *
 *                                                                             *
 ******************************************************************************/
/**
 * @file RandomNumberGenerator.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Declaration of the RandomNumberGenerator class for random number generation.
 */

#ifndef RANDOM_NUMBER_GENERATOR_HPP
#define RANDOM_NUMBER_GENERATOR_HPP

#include <random>   // std::random_device, std::mt19937, std::uniform_int_distribution
#include <cstdint>  // uint16_t

namespace OmegaL4Scanner::Utilities
{
    /**
     * @class RandomNumberGenerator
     * @brief Utility class for generating random numbers.
     *
     * @details This class provides static methods and members for generating
     *          random numbers, specifically for generating ephemeral ports.
     */
    class RandomNumberGenerator final {
    public:
        /**
         * @brief Generates a random 16-bit ephemeral port number.
         *
         * @details This function returns a random port number in the range
         *          of 49152 to 65535 (2^16 - 1).
         *
         * @return uint16_t A random ephemeral port number.
         */
        static uint16_t getEphemeralPort16();

        /**
         * @brief Generates a random 32-bit sequence number.
         *
         * @details This function returns a random sequence number in the range
         *          of 0 to 4294967295 (2^32 - 1).
         *
         * @return uint32_t A random 32-bit sequence number.
         */
        static uint32_t getSequenceNumber32();

    private:
        static std::random_device mRandomDevice;  /**< Random device used to seed the random number generator. */
        static std::mt19937 mRandomGenerator;     /**< Mersenne Twister random number generator.               */
        static std::uniform_int_distribution<uint16_t> mPortDistribution16;  /**< Distribution for generating 16-bit port numbers. */
        static std::uniform_int_distribution<uint32_t> mPortDistribution32;  /**< Distribution for generating 32-bit port numbers. */
    }; // RandomNumberGenerator
} // namespace OmegaL4Scanner::Utilities

#endif // RANDOM_NUMBER_GENERATOR_HPP

/*** end of file RandomNumberGenerator.hpp ***/
