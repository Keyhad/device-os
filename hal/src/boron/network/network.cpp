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

#include "ifapi.h"
#include "ot_api.h"
#include "openthread/lwip_openthreadif.h"
#include "wiznet/wiznetif.h"
#include "nat64.h"
#include <mutex>
#include <nrf52840.h>
#include "random.h"
#include "border_router_manager.h"
#include <malloc.h>
#include "serial_stream.h"
#include "usart_hal.h"
#include "ncp.h"
#include "pppncpnetif.h"

using namespace particle;
using namespace particle::net;
using namespace particle::net::nat;

namespace {

/* th1 - OpenThread */
BaseNetif* th1 = nullptr;
/* en2 - Ethernet FeatherWing */
BaseNetif* en2 = nullptr;
/* pp3 - Cellular */
BaseNetif* pp3 = nullptr;

bool netifCanForwardIpv4(netif* iface) {
    if (iface && netif_is_up(iface) && netif_is_link_up(iface)) {
        auto addr = netif_ip_addr4(iface);
        auto mask = netif_ip_netmask4(iface);
        auto gw = netif_ip_gw4(iface);
        if (!ip_addr_isany(addr) && !ip_addr_isany(mask) && !ip_addr_isany(gw)) {
            return true;
        }
    }

    return false;
}

} /* anonymous */

particle::services::at::BoronNcpAtClient* boronNcpAtClient() {
    using namespace particle::services::at;
    static SerialStream stream(HAL_USART_SERIAL2, 230400, SERIAL_8N1 | SERIAL_FLOW_CONTROL_RTS_CTS);
    static BoronNcpAtClient atClient(&stream);
    return &atClient;
}

int if_init_platform(void*) {
    /* lo0 (created by LwIP) */

    /* th1 - OpenThread */
    th1 = new OpenThreadNetif(ot_get_instance());

    /* en2 - Ethernet FeatherWing (optional) */
    uint8_t mac[6] = {};
    {
        const uint32_t lsb = __builtin_bswap32(NRF_FICR->DEVICEADDR[0]);
        const uint32_t msb = NRF_FICR->DEVICEADDR[1] & 0xffff;
        memcpy(mac + 2, &lsb, sizeof(lsb));
        mac[0] = msb >> 8;
        mac[1] = msb;
        /* Drop 'multicast' bit */
        mac[0] &= 0b11111110;
        /* Set 'locally administered' bit */
        mac[0] |= 0b10;
    }
    en2 = new WizNetif(HAL_SPI_INTERFACE1, D5, D3, D4, mac);
    uint8_t dummy;
    if (if_get_index(en2->interface(), &dummy)) {
        /* No en2 present */
        delete en2;
        en2 = nullptr;
    }

    /* pp3 - Cellular */
    pp3 = new PppNcpNetif(boronNcpAtClient());

    /* Enable border router by default */
    BorderRouterManager::instance()->start();

    auto m = mallinfo();
    const size_t total = m.uordblks + m.fordblks;
    LOG(TRACE, "Heap: %lu/%lu Kbytes used", m.uordblks / 1000, total / 1000);

    return 0;
}

extern "C" {

struct netif* lwip_hook_ip4_route_src(const ip4_addr_t* src, const ip4_addr_t* dst) {
    if (src == nullptr) {
        if (en2 && netifCanForwardIpv4(en2->interface())) {
            return en2->interface();
        } else if (pp3 && netifCanForwardIpv4(pp3->interface())) {
            return pp3->interface();
        }
    }

    return nullptr;
}

int lwip_hook_ip6_forward_pre_routing(struct pbuf* p, struct ip6_hdr* ip6hdr, struct netif* inp, u32_t* flags) {
    auto nat64 = BorderRouterManager::instance()->getNat64();
    if (nat64) {
        return nat64->ip6Input(p, ip6hdr, inp);
    }

    /* Do not forward */
    return 1;
}

int lwip_hook_ip4_input_pre_upper_layers(struct pbuf* p, const struct ip_hdr* iphdr, struct netif* inp) {
    auto nat64 = BorderRouterManager::instance()->getNat64();
    if (nat64) {
        int r = nat64->ip4Input(p, (ip_hdr*)iphdr, inp);
        if (r) {
            /* Ip4 hooks do not free the packet if it has been handled by the hook */
            pbuf_free(p);
        }

        return r;
    }

    /* Try to handle locally if not consumed by NAT64 */
    return 0;
}

}
