#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 10082
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[BUF_SIZE];

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Prepare server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    // Send messages continuously
    while (1) {
        printf("Enter message: ");

        fgets(buffer, BUF_SIZE, stdin);

        // Send message to server
        send(sock, buffer, strlen(buffer), 0);

        // Clear buffer
        memset(buffer, 0, BUF_SIZE);

        // Receive echo from server
        int bytes = read(sock, buffer, BUF_SIZE - 1);

        if (bytes <= 0)
            break;

        buffer[bytes] = '\0';

        printf("Echo from server: %s", buffer);
    }

    close(sock);

    return 0;
}
