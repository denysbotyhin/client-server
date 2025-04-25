#include <arpa/inet.h>
#include <exception>
#include <iostream>
#include <netdb.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <tuple>
#include <unistd.h>

#define SERVER_CAPACITY 10
#define MAX_PORT_LENGTH 6

void print_addr_info(const sockaddr_storage *sock_addr);
void worker(sockaddr_storage *client_addr, const char *msg);
addrinfo *get_addr_list(const char *host, const char *port);
void start_server(const char *host, const char *port);
std::tuple<int, sockaddr_storage, socklen_t> create_socket(const char *host,
                                                           const char *port);

addrinfo *get_addr_list(const char *host, const char *port)
{
    addrinfo hints{};
    addrinfo *addr_list{};

    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_UNSPEC;

    if (getaddrinfo(host, port, &hints, &(addr_list)) == -1)
        throw std::runtime_error(gai_strerror(errno));

    return addr_list;
}

std::tuple<int, sockaddr_storage, socklen_t> create_socket(const char *host,
                                                           const char *port)
{
    addrinfo *addr_list = get_addr_list(host, port);
    sockaddr_storage server{};
    socklen_t servlen{};
    int s{-1};

    for (addrinfo *p = addr_list; p != nullptr; p = p->ai_next)
    {
        if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1)
        {
            memcpy(&server, p->ai_addr, p->ai_addrlen);
            servlen = p->ai_addrlen;
        }
    }

    freeaddrinfo(addr_list);

    if (s == -1)
        throw std::runtime_error("Failed to create the socket");

    return {s, server, servlen};
}

void send_message(const char *host, const char *port, const char *msg)
{
    auto [s, servaddr, addrlen] = create_socket(host, port);

    for (int i = 0; i < 100; i++)
    {

        if (sendto(s,
                   msg,
                   strlen(msg),
                   0,
                   reinterpret_cast<sockaddr *>(&servaddr),
                   addrlen) < 0)
            throw std::runtime_error(strerror(errno));
    }

    close(s);
}

int main(int argc, const char **argv)
{
    if (argc < 3)
    {
        printf("Usage:\n%s  {hostname} {port number}\n", argv[0]);
        return 0;
    }

    const char *msg = argc == 3 ? "ping" : argv[3];

    try
    {
        send_message(argv[1], argv[2], msg);
    }
    catch (std::runtime_error e)
    {
        printf("%s", e.what());
    }

    return 0;
}
