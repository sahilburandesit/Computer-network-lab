#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 10082
#define BUF_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    char buffer[BUF_SIZE];
    int addrlen = sizeof(address);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Prepare server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    // Listen
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    printf("TCP Echo Server listening on port %d...\n", PORT);

    // Accept client
    new_socket = accept(
        server_fd,
        (struct sockaddr *)&address,
        (socklen_t *)&addrlen
    );

    if (new_socket < 0) {
        perror("Accept failed");
        close(server_fd);
        return 1;
    }

    // Receive and echo messages
    while (1) {
        memset(buffer, 0, BUF_SIZE);

        int bytes = read(new_socket, buffer, BUF_SIZE - 1);

        if (bytes <= 0)
            break;

        buffer[bytes] = '\0';

        printf("Received: %s", buffer);

        // Echo message back to client
        send(new_socket, buffer, bytes, 0);
    }

    close(new_socket);
    close(server_fd);

    return 0;
}
