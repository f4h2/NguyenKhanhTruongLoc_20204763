#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_DATA_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <filename> <receiver_ip> <receiver_port>\n", argv[0]);
        exit(1);
    }

    // Lấy tên file từ dòng lệnh
    char* filename = argv[1];

    // Lấy địa chỉ IP và cổng của receiver từ dòng lệnh
    char* receiver_ip = argv[2];
    int receiver_port = atoi(argv[3]);

    // Tạo socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Mở file và đọc nội dung
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("fopen");
        exit(1);
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char* data = (char*)malloc(file_size * sizeof(char));
    if (!data) {
        perror("malloc");
        exit(1);
    }
    if (fread(data, sizeof(char), file_size, file) != file_size) {
        perror("fread");
        exit(1);
    }
    fclose(file);

    // Khởi tạo địa chỉ receiver
    struct sockaddr_in receiver_addr;
    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_addr.s_addr =  inet_addr("127.0.0.1");
    receiver_addr.sin_port = htons(receiver_port);

    // Gửi tên file và nội dung đến receiver
    if (sendto(sockfd, filename, strlen(filename) + 1, 0, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr)) < 0) {
        perror("sendto");
        exit(1);
    }
    if (sendto(sockfd, data, file_size, 0, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr)) < 0) {
        perror("sendto");
        exit(1);
    }

    // Đóng socket và giải phóng bộ nhớ
    close(sockfd);
    free(data);

    return 0;
}