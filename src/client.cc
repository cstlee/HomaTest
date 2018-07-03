/**
 * Copyright (c) 2018, Stanford University
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "WireFormat.h"

#include "Homa.h"
#include "PerfUtils/Cycles.h"

using PerfUtils::Cycles;

const char* usage = "client <IP> <PORT> <IP> <PORT>";

int main(int argc, char* argv[])
{
    if (argc != 5) {
        printf("Usage:\n");
        printf("    %s\n", usage);
        return 0;
    }

    char* ip_str = argv[1];
    char* port_str = argv[2];

    std::string locator;

    locator.append(ip_str);
    locator.append(":");
    locator.append(port_str);

    printf("source: %s\n", locator.c_str());

    std::string destination;

    destination.append(argv[3]);
    destination.append(":");
    destination.append(argv[4]);

    printf("destination: %s\n", destination.c_str());

    Homa::Transport::Address addr(locator);
    Homa::Transport transport(addr);

    unsigned int id = 0;

    while (1) {
        Homa::Rpc rpc(&transport, Homa::Transport::Address(destination));
        Request* request = rpc.request->alloc<Request>();
        request->id = ++id;
        sprintf(request->message, "Sending you %d", id);
        printf("TX (%10d) : %s", request->id, request->message);

        rpc.send();
        rpc.wait();

        Response* response = rpc.response->get(response, 0);
        printf("TX (%10d) : %s", response->id, response->message);

        Cycles::sleep(1000000);
    }

    return 0;
}