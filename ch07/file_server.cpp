#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
using namespace std;
#define BUF_SIZE 30
void error_handling(const char *message);

int main()
{
    FILE *fp;
    fp = fopen("qq.txt", "rb");
    
    int server_fd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_adr;
    bzero(&serv_adr, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(8888); 
    if (bind(server_fd, ( struct sockaddr*)&serv_adr, sizeof(serv_adr))< 0) exit(0);
    // 开始监听客户端连入
    if (listen(server_fd, 1) < 0) exit(0);

    struct sockaddr_in clt_adr;
    socklen_t clnt_addr_size;
    clnt_addr_size = sizeof(clt_adr);
    int clt_fd = accept(server_fd, (struct sockaddr*)&clt_adr, (socklen_t*)&clt_adr);
    cout << "connected" << endl;
    char buf[BUF_SIZE];
    int read_cnt;
    while (1){
        //从文件流中读取数据，buffer为接收数据的地址，size为一个单元的大小，count为单元个数，stream为文件流
        //返回实际读取的单元个数
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE){
            write(clt_fd, buf, read_cnt);
            break;
        }
        write(clt_fd, buf, BUF_SIZE);
    }
    shutdown(clt_fd, 1);
    read(clt_fd, buf, BUF_SIZE);
    cout << "message from client: " << buf << endl;

    fclose(fp);
    close(server_fd);
    close(clt_fd);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
