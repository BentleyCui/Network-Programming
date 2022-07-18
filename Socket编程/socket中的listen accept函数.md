![image](https://user-images.githubusercontent.com/59153788/179470282-283acc9a-c10a-43a0-964a-cbb45c029d61.png)

****

#### listen()内部执行的时候干了什么

```c++
int listen(int fd, int backlog);
```

1. 服务器创建了一个`socket`

2. 内核执行`listen`

   * `listen`系统调用：
     * 根据用户传入的文件描述符来查找到对应的 `socket` 内核对象
     * 获取了系统里的 `net.core.somaxconn` 内核参数的值，和用户传入的 `backlog `比较后取一个最小值传入到下一步中
     * 接着通过调用 sock->ops->listen 进入协议栈的 listen 函数
   * 协议栈`listen`
     * 实际调用的是`int  inet_listen(struck socket *sock, int backlog)`
     * `sk->sk_max_ack_backlog = backlog`，服务器全连接队列长度是`listen`时传入的`backlog`和`net.core.somaxconn`之间较小的那个值
   * 接受队列定义
     * 全连接队列：通过`head`和`tail`指针以链表的形式来管理
     * 半连接队列：通过哈希表来管理，服务器端能在第三次握手时快速的查找出第一次握手时留存的 `request_sock`对象
   * 接收队列申请和初始化
     * 全连接列头设置为`NULL`
     * 半连接队列上每个元素分配的是一个指针，这其实是一个哈希表，真正的半连接用的对象是在握手过程中分配，计算完`Hash`值后挂到这个`Hash`表上

   #### listen最主要的工作就是申请和初始化接收队列，包括全连接队列和半连接队列。

   全连接队列长度是：`backlog`和内核参数`net.core.somaxconn`之间的较小值

   半连接队列的长度：`min(backlog, somaxconn, tcp_max_syn-backlog) + 1` 再上取到2的整次幂，最小不能小于16

****

`listen()`调用完毕,生成一个大小为backlog*3/2的等待队列(队列的长度,不同内核版本，系数（这个3/2）可能会有差异),这时客户端如果调用connect()并且队列没满则TCP连接直接就建立好了(三次握手建立双向连接).如果队列已经满了则服务段拒绝建立新的TCP连接. 实际上调用一次accept()是从该队列取出一个客户端的 fd 返回,同时队列的容量也会减一,如果队列为空,则accept()就会阻塞.

`accept()`函数功能是，从处于 `established` 状态的连接队列头部取出一个已经完成的连接，如果这个队列没有已经完成的连接，`accept()`函数就会阻塞，直到取出队列中已完成的用户连接为止。