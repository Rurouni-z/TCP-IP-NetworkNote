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
    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    
    FILE *fp = fopen("file_server.cpp", "rb");
    int serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_adr;
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    bind(serv_sd, (struct sockaddr *)&serv_adr, sizeof(serv_adr));
    listen(serv_sd, 5);

    struct sockaddr_in clnt_adr;
    int clnt_sd = accept(serv_sd, (struct sockaddr *)&clnt_adr, (socklen_t*)sizeof(clnt_adr));

    char buf[BUF_SIZE];
    int read_cnt;
    while (1)
    {
        //从文件流中读取数据，buffer为接收数据的地址，size为一个单元的大小，count为单元个数，stream为文件流
        //返回实际读取的单元个数
        read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE)
        {
            write(clnt_sd, buf, read_cnt);
            break;
        }
        write(clnt_sd, buf, BUF_SIZE);
    }

    shutdown(clnt_sd, SHUT_WR);
    read(clnt_sd, buf, BUF_SIZE);
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
