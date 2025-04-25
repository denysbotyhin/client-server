#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <exception>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>

#define SERVER_CAPACITY 10
#define MAX_PORT_LENGTH 6

void print_addr_info(const sockaddr_storage *sock_addr);
void worker(int sock_client, sockaddr_storage *client_addr);
addrinfo *get_addr_list(const char *host, const char *port);
int create_socket(const char *host, const char *port);
void start_server(const char *host, const char *port);

void worker(int sock_client, sockaddr_storage *client_addr)
{
    if (sock_client == -1)
    {
        printf("failed to accept the incoming connection\n");
        return;
    }

    print_addr_info(client_addr);

    char buf[2000];
    ssize_t msg_len;

    if ((msg_len = recv(sock_client, buf, 2000, 0)) == 0)
        printf("failed to read the message\n");
    else
        printf("message: %s\n", buf);

    delete client_addr;
    close(sock_client);
}

void print_addr_info(const sockaddr_storage *sock_addr)
{
    char addr[INET6_ADDRSTRLEN];
    char port[MAX_PORT_LENGTH];
    getnameinfo(reinterpret_cast<const sockaddr *>(sock_addr),
                sock_addr->ss_len,
                addr, INET6_ADDRSTRLEN,
                port, MAX_PORT_LENGTH,
                NI_NUMERICHOST | NI_NUMERICSERV);

    printf("Client: %s:%s\n", addr, port);
}

addrinfo *get_addr_list(const char *host, const char *port)
{
    addrinfo hints{};
    addrinfo *addr_list{};

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(host, port, &hints, &(addr_list)) == -1)
        throw std::runtime_error(gai_strerror(errno));

    return addr_list;
}

int create_socket(const char *host, const char *port)
{
    addrinfo *addr_list = get_addr_list(host, port);
    int s{-1};

    for (addrinfo *p = addr_list; p != nullptr; p = p->ai_next)
    {
        if ((s = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) != -1)
        {
            if (bind(s, p->ai_addr, p->ai_addr->sa_len) != -1)
                break;
            else
                close(s);
        }
    }

    freeaddrinfo(addr_list);

    if (s == -1)
        throw std::runtime_error("Failed to create the socket");

    return s;
}

void start_server(const char *host, const char *port)
{
    int s = create_socket(host, port);

    if (listen(s, SERVER_CAPACITY) == -1)
        throw std::runtime_error("Failed to start the server");

    printf("Server listning on port %s\n", port);

    socklen_t clien_len = sizeof(sockaddr_storage);

    while (true)
    {
        sockaddr_storage *client_addr = new sockaddr_storage{};

        int sock_client = accept(s,
                                 reinterpret_cast<sockaddr *>(client_addr),
                                 &clien_len);
        std::thread w(worker, sock_client, client_addr);
        w.detach();
    }
}

int main(int argc, const char **argv)
{
    if (argc < 3)
    {
        printf("Usage:\n%s  {hostname} {port number}\n", argv[0]);
        return 1;
    }

    try
    {
        start_server(argv[1], argv[2]);
    }
    catch (std::runtime_error e)
    {
        printf("Exception occured: %s", e.what());
    }

    return 0;
}