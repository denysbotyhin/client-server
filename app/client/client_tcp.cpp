#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <exception>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>

#define SERVER_CAPACITY 10

addrinfo *get_addr_list(const char *host, const char *port);
int create_socket(const char *host, const char *port);
void send_message(const char *host, const char *port, const char *msg);

addrinfo *get_addr_list(const char *host, const char *port)
{
    addrinfo hints{};
    addrinfo *addr_list;

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;

    if (getaddrinfo(host, port, &hints, &(addr_list)) == -1)
        throw std::runtime_error(gai_strerror(errno));

    return addr_list;
}

int create_socket(const char *host, const char *port)
{
    addrinfo *addr_list = get_addr_list(host, port);
    int s;

    for (addrinfo *p = addr_list; p != nullptr; p = p->ai_next)
    {
        if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1)
        {
            if (connect(s, p->ai_addr, p->ai_addrlen) != -1)
                return s;
            else
                close(s);
        }
    }

    freeaddrinfo(addr_list);

    throw std::runtime_error("Failed to create the socket");
}

void send_message(const char *host, const char *port, const char *msg)
{
    for (int i = 0; i < 100; i++)
    {
        int s = create_socket(host, port);

        if (send(s, msg, strlen(msg), 0) == 0)
        {
            throw std::runtime_error("Failed to send the message");
        }

        close(s);
    }
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