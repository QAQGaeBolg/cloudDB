#include <stdio.h>
#include <unistd.h>
#include <windows.h>
#include <stdlib.h>
#include <ctype.h>
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <sys/types.h>
 
#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666
 
int main(void)
{
    int lfd, cfd; //文件描述符
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    char buf[BUFSIZ];             //BUFSIZ定义在stdio.h，为8192，一般是会用来做数组的长度。
    int n, i;
 
    lfd = socket(AF_INET, SOCK_STREAM, 0);
 
    /* 创建一个IPV4 socket地址 */
    serv_addr.sin_family = AF_INET;                //协议
    serv_addr.sin_port = htons(SERV_PORT);         //16位端口 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //IP地址
    //上一行代码也可被这代替 inet_pton(AF_INET,SERV_IP,&serv_addr.sin_addr);
 
    bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
 
    listen(lfd, 128);
 
    clie_addr_len = sizeof(clie_addr); //因为clie_addr_len是传入传出参数，使用前必须初始化
 
    //clie_addr是传出参数，因此会在函数外定义，在函数中对该变量操作赋值
    cfd = accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
 
    printf("client IP:%s, client port:%d\n",
		inet_ntop(AF_INET,&clie_addr.sin_addr.s_addr,clie_IP,sizeof(clie_IP)),
		ntohs(clie_addr.sin_port));
    
    //连接成功后，循环读取用户的输入
    while (1) {
        n = read(cfd, buf, sizeof(buf));
        for (i = 0; i < n; i++)
            buf[i] = toupper(buf[i]); //将小写字母转换为大写
        write(cfd, buf, n);
    }
 
    close(lfd);
    close(cfd);
 
    return 0;
}