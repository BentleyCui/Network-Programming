

# socket编程

## 协议


**以太网帧协议：**

​		ARP协议：根据 IP 地址获取 mac 地址

​		以太网帧协议：根据 mac 地址，完成数据包传输

**IP 协议：**

​		版本：IPV4  IPV6

​		生存时间 TTL: time to live，下一跳的个数，数据包在路由节点中的跳转上限

​		源IP，目的IP：32位-----4字节

**UDP 协议：**

​		源端口号------16位         2^16 =- 65536，端口号的上限

​		目的端口号-----16位

​		**IP 地址：可以在网络环境中标识唯一一台主机**

​		**端口号：可以在网络的一台主机上，唯一表示一个进程**

​		IP 地址 + 端口号 ：

**TCP 协议：**

​		源端口号-----16位

​		目的端口号

​		序号------32位

​		确认号

​		6个标志位

​		窗口大小-----16位

## 套接字

socket

在通信过程中，套接字一定是成对出现的

**套接字通信原理：**

![image](https://user-images.githubusercontent.com/59153788/171339259-f3b78ae3-b41e-4378-b18a-dea8a42e7518.png)

 一个文件描述符指向一个套接字，该套接字内部由内核借助两个缓

## 网络字节序

​		**小端法**：(pc 本地存储)   高位存高地址，低位存低地址

​		**大端法**：(网络存储) 低位存高地址

需要转换：IP地址和端口号

```c
#include <arpa/inet.h>

uint32_t htonl(uint32_t hostlong); // “192.168.1.1” string  ->  int 转换  
 
string -> atoi()-> int -> htonl -> 网络字节序

uint16_t htons(uint16_t hostshort); // 本地 -> 网络 port

uint32_t ntohl(uint32_t netlong);

uint16_t ntohs(uint16_t netshort);
```

```c
// 现在  IP地址转换函数
#include <arpa/inet.h>
int inet_pton(int af, const char* src, void* dst); 本地字节序 ---> 网络字节序
// af: AF_INET  AF_INET6
// src: 传入参数，IP地址，点分十进制的string
// dst: 传出参数，转换后的 网络字节序的 IP地址
返回值：
    成功：1
    异常：0，说明 src指向的不是一个有效的ip地址
    
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
src: 网络字节序的ip地址
dst: 本地字节序（string ip）
size: dst的大小
返回值：
    成功：dst
    失败：null
```



## sockaddr 数据结构：

```c
struct sockaddr {
	sa_family_t sa_family; 		/* address family, AF_xxx */
	char sa_data[14];			/* 14 bytes of protocol address */
};

struct sockaddr_in {
	sa_family_t    sin_family; /* address family: AF_INET */
	in_port_t      sin_port;   /* port in network byte order */
	struct in_addr sin_addr;   /* internet address */
};

/* Internet address. */
struct in_addr {
	uint32_t       s_addr;     /* address in network byte order */
};
```

```c
// 用法
struct sockaddr_in addr;
addr.sin_family = AF_INET;
addr.sin_port = htons(8888);
addr.sin_addr.s_addr = htonl(INADDR_ANY); //取出系统中有效的任意ip地址，二进制类型
//int dst;
//addr.sin_addr.s_addr = inet_pton(AF_INET, "192.168.10.1", (void*)dst);
bind(fd, (struct sockaddr*)&addr, sizeof(addr));
```

## socket函数

```c
bind(); // 绑定 ip+端口号
listen(); //设置监听上限
accept(); //阻塞监听客户端连接
```

```c
#include <sys/socket.h>
int socket(int domain, int type, int protocol);
// domain: AF_INET  AF_INET6  AF_UNIX
// type: SOCK_STREAM  SOCK_DGRAM，流式协议 报式协议
// protocol: 0，代表协议
功能：创建一个套接字
返回值：
    成功：新套接字对应的文件描述符
    失败： -1，设置 errno
```

```c
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// 给socket绑定一个 地址结构：ip + port
返回值：
    成功：0
    失败：-1 errno
```

```c
int listen(int sockfd, int backlog);
// 设置同时与服务器建立连接的上限数（同时进行三次握手的客户端数量），最大值128
返回值：
    成功：0
    失败： -1 errno
```

```c
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// 阻塞等待客户端建立连接，成功的话返回一个与客户端建立连接的socket文件描述符
// sockfd: 
// addr: 传出参数，成功与服务器建立连接的客户端的地址结构
// addrlen： 传入传出参数，传入addr的大小，传出客户端addr的实际大小
返回值：
    能与服务器建立连接的socket对应的文件描述符
    失败：-1 errno
```

```c
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// 与服务器建立连接
sockfd: 客户端sockfd
addr: 服务器的地址结构;
addrlen: 服务器地址结构的长度 
返回值：
    成功：0
    失败：-1
```

## TCP 通信流程

<img src="https://user-images.githubusercontent.com/59153788/171345518-2b2e0486-0b72-46aa-8cbd-45dc87ceebd3.png" alt="image" style="zoom: 67%;" />

```c
bind()    //绑定 ip + port;
listen()  //设置监听上限，同时跟服务端建立连接的数量;
accept()  //阻塞客户端连接 
```

**共有三个套接字**

 
![image](https://user-images.githubusercontent.com/59153788/179470282-283acc9a-c10a-43a0-964a-cbb45c029d61.png)

