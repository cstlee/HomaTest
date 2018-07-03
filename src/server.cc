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

#include <memory>

const char* usage = "server <IP> <PORT>";

int main(int argc, char* argv[])
{
    if (argc != 3) {
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

    printf("%s\n", locator.c_str());

    Homa::Transport::Address addr(locator);
    Homa::Transport transport(addr);

    std::unique_ptr<Homa::ServerRpc> serverRpc;
    Request payload;

    while (1) {
        serverRpc = std::move(transport.receiveServerRpc());
        if (serverRpc) {
            Request* request = serverRpc->request->get(&payload, 0);
            printf("%d: %s\n", request->id, request->message);

            Response* response = serverRpc->response->alloc<Response>();
            response->id = request->id;
            sprintf(response->message, "Recieved %d from you.", response->id);
            transport.replyServerRpc(std::move(serverRpc));
        }
        transport.poll();
    }

    return 0;
}