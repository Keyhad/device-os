/*
 * Copyright (c) 2018 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HAL_NETWORK_LWIP_IP_ADDR_UTIL_H
#define HAL_NETWORK_LWIP_IP_ADDR_UTIL_H

namespace particle { namespace net {

template <size_t SIZE>
struct IpAddrNtoaHelper {
    char str[SIZE] = {};
};

#define IPADDR_NTOA(addr) \
    ({ \
        IpAddrNtoaHelper<IP6ADDR_STRLEN_MAX> tmp; \
        ipaddr_ntoa_r(addr, tmp.str, sizeof(tmp.str)); \
        tmp; \
    })

#define IP6ADDR_NTOA(addr) \
    ({ \
        IpAddrNtoaHelper<IP6ADDR_STRLEN_MAX> tmp; \
        ip6addr_ntoa_r(addr, tmp.str, sizeof(tmp.str)); \
        tmp; \
    })

#define IP4ADDR_NTOA(addr) \
    ({ \
        IpAddrNtoaHelper<IP4ADDR_STRLEN_MAX> tmp; \
        ip4addr_ntoa_r(addr, tmp.str, sizeof(tmp.str)); \
        tmp; \
    })

} } /* namespace particle::net */

#endif /* HAL_NETWORK_LWIP_IP_ADDR_UTIL_H */