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
![image](https://user-images.githubusercontent.com/59153788/179463289-4c9d2972-dcef-4970-91e6-683dd76b531e.png)


![image-20220719155235245](C:\Users\cuibin\AppData\Roaming\Typora\typora-user-images\image-20220719155235245.png)
