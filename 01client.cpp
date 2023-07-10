#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char computer_name[50];
    int num_drives;
    char drives[10][50];
    char buf[1024] = {0};

    printf("Enter computer name: ");
    scanf("%s", computer_name);

    printf("Enter number of drives: ");
    scanf("%d", &num_drives);

    printf("Enter drive info (drive_letter-size):\n");
    for (int i = 0; i < num_drives; i++) {
        scanf("%s", drives[i]);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    sprintf(buf, "%s;%d", computer_name, num_drives);
    for (int i = 0; i < num_drives; i++) {
        strcat(buf, ";");
        strcat(buf, drives[i]);
    }
    send(sock, buf, strlen(buf), 0);
    printf("Data sent to server: %s\n", buf);

    close(sock);

    return 0;
}