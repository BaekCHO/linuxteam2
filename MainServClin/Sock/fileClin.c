#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
#include <fcntl.h>  
#include <sys/types.h>  
#include <sys/socket.h>  

#define BUFSIZE 30  

void error_handling(char *message);  
int main(int argc, char **argv) {  
          int fd;  
          int sd;  
          char buf[BUFSIZE];
          char cbuf[BUFSIZE];  
          int len;  
          struct sockaddr_in serv_addr;  
          
          if(argc!=3){  
              printf("Usage : %s <IP> <port>\n", argv[0]);  
              exit(1);  
          }  
          
          /* 수신 한 데이터를 저장 할 파일 오픈 */  
          fd = open("receive.dat", O_WRONLY|O_CREAT|O_TRUNC);  
          if(fd == -1)  
              error_handling("File open error");  
          
          /* 서버 접속을 위한 소켓 생성 */  
          sd=socket(PF_INET, SOCK_STREAM, 0);     
          if(sd == -1)  
              error_handling("socket() error");  
          
          memset(&serv_addr, 0, sizeof(serv_addr));  
          serv_addr.sin_family=AF_INET;  
          serv_addr.sin_addr.s_addr=inet_addr(argv[1]);  
          serv_addr.sin_port=htons(atoi(argv[2]));  
          
          if( connect(sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1 )  
              error_handling("connect() error!");  
          
          /* 원하는 데이터를 입력 */
          fgets(cbuf, 10, stdin);
          write(sd, cbuf, 5);     
          
          /* 데이터를 전송 받아서 파일에 저장한다 */  
          while( (len=read(sd, buf, BUFSIZE )) != 0 )  
          {  
              write(fd, buf, len);   
              
          }  
          
          /* 전송해 준것에 대한 감사의 메시지 전달 */  
          write(sd, "\nThank you\n", 10);     
          close(fd);  
          close(sd);  
          
          return 0;  
}  

void error_handling(char *message)  
{  
    fputs(message, stderr);  
    fputc('\n', stderr);  
    
    exit(1);  
}
