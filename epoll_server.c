#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>
#define EPOLL_SIZE 1000
#define BUF_SIZE 1000
int clnt_socks[1000]={0,};
int clnt_cnt=0;

void write_to_all(char * buf, int len)
{
    int i;
   
    for(i=0;i<clnt_cnt;i+=1)
        write(clnt_socks[i], buf, len);
    
}


int main( int argc, char * argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in clnt_adr;
    struct sockaddr_in serv_adr;
    struct epoll_event event;
    struct epoll_event * ep_events;
    socklen_t clnt_adr_sz;
    int i;
    int epfd;
    int event_cnt=0;
    char buf[BUF_SIZE]={0,};
    int readcnt=0;
    int fd_max;
    int recv_sum=0;
    int str_len=0;
    int option=1;
    socklen_t optlen;
    int a=0;

    if(argc!=2)
    {
        printf("usage: %s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock==-1)
        printf("socket() error!");
    optlen=sizeof(option);
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
     serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
        printf("bind() error!");

    if(listen(serv_sock, 5)==-1)    
        printf("listen() erroR!");
    //fd_max=serv_sock;
    epfd=epoll_create(EPOLL_SIZE);
    ep_events=(struct epoll_event*)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);
    
    event.events=EPOLLIN;
    event.data.fd=serv_sock;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);
    clnt_adr_sz=sizeof(clnt_adr);
    while(1)
    {
        event_cnt=epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        
        if(event_cnt==-1)
            printf("epoll_wait() error!");
        
        for(i=0;i<event_cnt;i+=1)
        {
            if(ep_events[i].data.fd==serv_sock)
            {
                
                clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
                if(clnt_sock==-1)
                        printf("accept() error! || No client required");
                   
                clnt_socks[clnt_cnt++]=clnt_sock;


                option=1;
                optlen=sizeof(option);
                setsockopt(clnt_sock, SOL_SOCKET, SO_REUSEADDR, (void*)option, optlen);


                event.events=EPOLLIN;
                event.data.fd=clnt_sock;    
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
                printf("connected client: %d\n",clnt_sock);
                
            }
            else
            {    
                a=read(ep_events[i].data.fd, &readcnt, 4);
                if(a==0)
                {
                    close(ep_events[i].data.fd);
                    printf("closed client: %d\n", (int)ep_events[i].data.fd);
                    for(i=0;i<clnt_cnt;i+=1)
                    {
                        if(clnt_socks[i]==ep_events[i].data.fd)
                        {
                            while(i<clnt_cnt-1)
                            {
                                clnt_socks[i]=clnt_socks[i+1];
                                i+=1;
                            }
                        }
                    }
                    clnt_cnt--;
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    
                    continue;
                }
                recv_sum=0;
                //printf("readcnt: %d\n",readcnt);
                while(recv_sum<readcnt)
                {
                    str_len=read(ep_events[i].data.fd, &buf[recv_sum], BUF_SIZE);
                    if(str_len==-1)
                    {
                        printf("read() erroR!");
                        exit(1);
                    }
                    recv_sum+=str_len;
                }
                
            
                write_to_all( buf, recv_sum);
                memset(buf, 0, BUF_SIZE);
                
            }
        }
    }
    close(serv_sock);
    close(epfd);
    return 0;
}