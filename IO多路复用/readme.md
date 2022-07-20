## I/O多路复用

### select

```c++
int select(int nfds, fd_set *readfds, fd_set *writefds, 
           fd_set *exceptfds, struct timeval *timeout);
参数:
	sizeof(fd_set) = 128
	-nfds: 委托内核检测的最大文件描述符的值 +1;
	-readfds:要检测的文件描述符的读的集合，委托内核检测哪些文件描述符的读属性
        	-一般检测读操作
        	-对应的是对方发过来的数据，检测读缓冲区
        	-是一个传入传出参数
	-writefds:写的集合
        	-委托内核检测写缓冲区是不是还可以写数据（不满就可以写）
	-exceptfds:检测发生异常的文件描述符集合
	-timeout:设置的超时时间
        	-NULL：永久阻塞，直到检测到了文件描述符有变化
        	-tv_sec = 0,  tv_usec = 0：不阻塞    
返回值：
    -1：失败
    >0：检测的集合中有n个文件描述符发生了变化
        
        
// 将参数文件描述符fd对应的标志位置为0
void FD_CLR(int fd， fd_set *set);

// 判断fd对应的标志位是0还是1
void FD_ISSET(int fd， fd_set *set);

//将fd对应的标志位设置为1
void FD_SET();

//清空set，1024 bit全部初始化为0
void FD_ZERO(fd_set *set);
```

![](https://user-images.githubusercontent.com/59153788/179463289-4c9d2972-dcef-4970-91e6-683dd76b531e.png)

![image](https://user-images.githubusercontent.com/59153788/179697590-4ae1c406-28cb-44de-a466-a456813d2b8b.png)





****

### poll

```c++
#include<poll.h>
struct pollfd{
    int		fd;		//委托内核检测的文件描述符
    short	events;	//委托内核检测文件描述符的什么事件
    short	revents;//文件描述符实际发生的事件
};
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

![image](https://user-images.githubusercontent.com/59153788/179697737-64f9f96d-1f8b-4623-904d-eef24e1c404e.png)



****

### epoll

![image](https://user-images.githubusercontent.com/59153788/179998898-cc7b2431-0162-4b8a-9fa6-880a1ae23b56.png)


```c++
#include <sys/epoll.h> 
// 创建一个新的epoll实例。在内核中创建了一个数据，
// 这个数据中有两个比较重要的数据，一个是需要检 测的文件描述符的信息（红黑树），
// 还有一个是就绪列表，存放检测到数据发送改变的文件描述符信息（双向 链表）。 
int epoll_create(int size); 
	- 参数：size : 目前没有意义了。随便写一个数，必须大于0 
    - 返回值： 
        -1 : 失败 
        > 0 : 文件描述符，操作epoll实例的
            
typedef union epoll_data { 
        void *ptr; 
        int fd; 
        uint32_t u32; 
        uint64_t u64; 
} epoll_data_t;

struct epoll_event { 
            uint32_t events; /* Epoll events */ 
            epoll_data_t data; /* User data variable */ 
};
常见的Epoll检测事件： 
    - EPOLLIN 
    - EPOLLOUT 
    - EPOLLERR 
    
// 对epoll实例进行管理：添加文件描述符信息，删除信息，修改信息 
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); 
	- 参数： 
        - epfd : epoll实例对应的文件描述符 
        - op : 要进行什么操作 
            EPOLL_CTL_ADD: 添加 
            EPOLL_CTL_MOD: 修改 
            EPOLL_CTL_DEL: 删除 
        - fd : 要检测的文件描述符 
        - event : 检测文件描述符什么事情 
            
// 检测函数             
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout); 
	- 参数：
        - epfd : epoll实例对应的文件描述符 
		- events : 传出参数，保存了发送了变化的文件描述符的信息 
        - maxevents : 第二个参数结构体数组的大小 
        - timeout : 阻塞时间 
            - 0 : 不阻塞 
            - -1 : 阻塞，直到检测到fd数据发生变化，解除阻塞 
            - > 0 : 阻塞的时长（毫秒） 
        - 返回值： 
            - 成功，返回发送变化的文件描述符的个数 > 0 
            - 失败 -1
```

