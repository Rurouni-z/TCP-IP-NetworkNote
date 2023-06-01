#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(const char *message);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    
    FILE *fp;
    int sd;
    fp = fopen("receive.txt", "wb");
    sd = socket(PF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
//     serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
//     serv_adr.sin_port = htons(atoi(argv[2]));
    serv_adr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_adr.sin_port = htons(8888);

    connect(sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr));

    char buf[BUF_SIZE];
    int read_cnt;
    while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0){
        sleep(1);  // 防止接收到垃圾数据，而爆内存，建议输出一下buf
        fwrite((void *)buf, 1, read_cnt, fp);
    }
    puts("Received file data");
    write(sd, "Thank you", 10);
    fclose(fp);
    close(sd);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
