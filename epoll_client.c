#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<errno.h>

#define BUF_SIZE 1000
#define NAME_SIZE 30
char name[NAME_SIZE];
void write_routine(int sock, char * buf)
{
    int writecnt=0;
    char namemsg[BUF_SIZE+NAME_SIZE]={0,};
    memset(buf, 0, BUF_SIZE);
    while(1)
    {
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "exit\n"))
        {
            shutdown(sock, SHUT_WR);
            return;
        }
        sprintf(namemsg, "%s %s",name, buf);
        writecnt=strlen(namemsg);
        write(sock, &writecnt, 4);

        write(sock, namemsg, strlen(namemsg));
        memset(buf, 0, BUF_SIZE);
    }
    return;
}


void read_routine(int sock, char * buf)
{
    int read_cnt=0;
    
    while((read_cnt=read(sock, buf, BUF_SIZE))!=0)
    {
        fputs(buf, stdout);
        memset(buf, 0, BUF_SIZE);
    }
    return;
}


int main( int argc, char * argv[])
{
    int sock;
    struct sockaddr_in adr;
    int str_len=0;
    int i;
    char buf1[BUF_SIZE]={0,};
    char buf2[BUF_SIZE]={0,};
    pid_t pid;
    int option=1;
    socklen_t optlen=sizeof(option);

    if(argc!=4)
    {
        printf("usage: %s <ip> <port> <username>\n",argv[0]);
        exit(1);
    }
    sprintf(name, "[%s]", argv[3]);
    
    sock=socket(PF_INET, SOCK_STREAM, 0);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen); 
    if(sock==-1)
        printf("socket() error!");
    
    memset(&adr, 0, sizeof(adr));
    adr.sin_family=AF_INET;
    adr.sin_addr.s_addr=inet_addr(argv[1]);
    adr.sin_port=htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr*)&adr, sizeof(adr))==-1)
    {
        printf("connect() error!");
        printf("errno : %d\n",errno);
    }
    pid=fork();
    if(pid==0)
    {
        write_routine(sock, buf1);
    }
    else
        read_routine(sock, buf2);

    close(sock);
    return 0;
}