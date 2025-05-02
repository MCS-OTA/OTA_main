// secure_ota_socket_client.cpp
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <iostream>
#include <string>

int main() {
    const char* socket_path = "/tmp/ota_secure.sock";
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, socket_path);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        return 1;
    }

    std::string payload = "token:secret123|OTA_UPDATE_AVAILABLE";
    send(sock, payload.c_str(), payload.size(), 0);

    char buffer[1024] = {};
    recv(sock, buffer, sizeof(buffer), 0);
    std::cout << "[C++] Response from Python: " << buffer;

    close(sock);
    return 0;
}
