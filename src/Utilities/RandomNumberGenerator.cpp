/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         RandomNumberGenerator.cpp                                     *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      24.03.2025                                                    *
 * Last edit:    24.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains the implementation of the                  *
 *               RandomNumberGenerator class, which provides utility           *
 *               functions for random number generation.                       *
 *                                                                             *
 ******************************************************************************/
/**
 * @file RandomNumberGenerator.cpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Implementation of the RandomNumberGenerator class for random number generation.
 */

#include "RandomNumberGenerator.hpp"

using namespace std;

namespace OmegaL4Scanner::Utilities
{
    random_device RandomNumberGenerator::mRandomDevice;
    mt19937 RandomNumberGenerator::mRandomGenerator(mRandomDevice());
    uniform_int_distribution<uint16_t> RandomNumberGenerator::mPortDistribution16(49152, 65535); // recommended ephemeral port range
    uniform_int_distribution<uint32_t> RandomNumberGenerator::mPortDistribution32(0, 0xFFFFFFFF);

    uint16_t RandomNumberGenerator::getEphemeralPort16() {
        return mPortDistribution16(mRandomGenerator);
    } // RandomNumberGenerator::getEphemeralPort16()

    uint32_t RandomNumberGenerator::getSequenceNumber32() {
        return mPortDistribution32(mRandomGenerator);
    } // RandomNumberGenerator::getSequenceNumber32()
} // OmegaL4Scanner::Utilities

/*** end of file RandomNumberGenerator.hpp ***/
