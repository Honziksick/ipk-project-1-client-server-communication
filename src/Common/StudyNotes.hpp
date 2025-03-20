/*******************************************************************************
 *                                                                             *
 * Project:      OMEGA L4 Scanner                                              *
 * University:   Faculty of Information Technology, BUT                        *
 * Subject:      IPK: Computer Communications and Networks                     *
 *                                                                             *
 * File:         StudyNotes.cpp                                             *
 * Author:       Jan Kalina <xkalinj00>                                        *
 *                                                                             *
 * Created:      20.03.2025                                                    *
 * Last edit:    20.03.2025                                                    *
 *                                                                             *
 * Description:  This file contains notes and documentation references         *
 *               that help me during my learning process. I apologize for      *
 *               its existence, as I know such files should not normally       *
 *               exist in project repositories.                                *
 *                                                                             *
 ******************************************************************************/
/**
 * @file StudyNotes.hpp
 * @author Jan Kalina \<xkalinj00>
 * @brief Learning notes and documentation references.
 */

#ifndef STUDY_NOTES_HPP
#define STUDY_NOTES_HPP

/**
 * @brief Notes for <arpa/inet.h> (https://man7.org/linux/man-pages/man0/arpa_inet.h.0p.html)
 * - sockaddr -> sa_family_t sa_family (address family: AF_INET, AF_INET6)
 *            -> char sa_data[] (socket address)
 * - INET6_ADDRSTRLEN -> 46 (length of IPv6 address string)
 * - const char *inet_ntop(int af, const void *restrict src, char dst[restrict .size], socklen_t size);
 *     -> convert IPv4 and IPv6 addresses from binary to text form
 */

/**
 * @brief Notes for <ifaddrs.h> (https://man7.org/linux/man-pages/man3/getifaddrs.3.html)
 * - ifaddrs -> ifaddrs *ifa_next (next item in list)
 *           -> char *ifa_name (name of interface)
 *           -> unsigned int ifa_flags (flags from SIOCGIFFLAGS)
 *           -> sockaddr *ifa_addr (address of interface)
 *           -> sockaddr *ifa_netmask (netmask of interface)
 *           -> sockaddr *ifu_broadaddr (broadcast address of interface)
 *           -> sockaddr *ifu_dstaddr (point-to-point destination address)
 * - int getifaddrs(ifaddrs **ifap);
 * - void freeifaddrs(ifaddrs *ifa);
 */

/**
 * @brief Notes for <sys/socket.h> (https://man7.org/linux/man-pages/man0/sys_socket.h.0p.html)
 * - AF_INET -> src points to a struct in_addr (in network byte order)
 *              which is converted to an IPv4 network address in the
 *              dotted-decimal format, "ddd.ddd.ddd.ddd".  The buffer dst
 *              must be at least INET_ADDRSTRLEN bytes long.
 * - AF_INET6 -> src points to a struct in6_addr (in network byte order)
 *               which is converted to a representation of this address in
 *               the most appropriate IPv6 network address format for this
 *               address.  The buffer dst must be at least INET6_ADDRSTRLEN
 *               bytes long.
 */

/**
 * @brief Notes for <net/if.h> (https://man7.org/linux/man-pages/man0/net_if.h.0p.html)
 * - IFF_UP -> Interface is running
 */

#endif // STUDY_NOTES_HPP

/*** end of file StudyNotes.hpp ***/
