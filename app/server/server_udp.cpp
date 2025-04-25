#include <arpa/inet.h>
#include <exception>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#define SERVER_CAPACITY 10
#define MAX_PORT_LENGTH 6

void print_addr_info(const sockaddr_storage *sock_addr);
void worker(sockaddr_storage *client_addr, const char *msg);
addrinfo *get_addr_list(const char *host, const char *port);
int create_socket(const char *host, const char *port);
void start_server(const char *host, const char *port);

void worker(sockaddr_storage *client_addr, const char *msg)
{
    print_addr_info(client_addr);
    printf("message: %s\n", msg);

    delete[] msg;
    delete client_addr;
}

void print_addr_info(const sockaddr_storage *sock_addr)
{
    char addr[INET6_ADDRSTRLEN];
    char port[MAX_PORT_LENGTH];
    getnameinfo(reinterpret_cast<const sockaddr *>(sock_addr),
                sock_addr->ss_len,
                addr,
                INET6_ADDRSTRLEN,
                port,
                MAX_PORT_LENGTH,
                NI_NUMERICHOST | NI_NUMERICSERV);

    printf("Client: %s:%s\n", addr, port);
}

addrinfo *get_addr_list(const char *host, const char *port)
{
    addrinfo hints{};
    addrinfo *addr_list{};

    hints.ai_socktype = SOCK_DGRAM;
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

    printf("Server listning on port %s\n", port);

    socklen_t clien_len = sizeof(sockaddr_storage);

    while (true)
    {
        sockaddr_storage *client_addr = new sockaddr_storage{};
        char *msg = new char[1024];

        recvfrom(s,
                 msg,
                 1024,
                 0,
                 reinterpret_cast<sockaddr *>(client_addr),
                 &clien_len);
        std::thread w(worker, client_addr, msg);
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
