# `IP` 地址和端口号

## `IP`（`Internet Protocol`）:

本质是一个整形数，用于表示计算机在网络中的地址。

`IP` 协议版本有两个: `IPv4` 和 `IPv6`

- `IPv4`（`Internet Protocol version4`）:
  - 使用一个 `32` 位的整形数描述一个 `IP` 地址，`4` 个字节，`int` 型
  - 也可以使用一个点分十进制字符串描述这个 `IP` 地址:  `192.168.247.135`
  - 分成了 `4` 份，每份 `1` 字节，`8bit`（`char`），最大值为 `255`
    - `0.0.0.0` 是最小的 `IP` 地址
    - `255.255.255.255` 是最大的 `IP` 地址
按照 `IPv4` 协议计算，可以使用的 `IP` 地址共有 `2^32` 个

- `IPv6`（`Internet Protocol version6`）:
  - 使用一个 `128` 位的整形数描述一个 `IP` 地址，`16` 个字节
  - 也可以使用一个字符串描述这个 `IP` 地址: `2001:0db8:3c4d:0015:0000:0000:1a2f:1a2b`
  - 分成了 `8` 份，每份 `2` 字节，每一部分以 `16` 进制的方式表示
  - 按照 `IPv6` 协议计算，可以使用的 `IP` 地址共有 `2^128` 个

查看 `IP` 地址:

```
# linux
$ ifconfig

# windows
$ ipconfig

# 测试网络是否畅通
$ ping xxx.xxx.xxx.xxx

# 特殊的 IP 地址: 127.0.0.1  ==> 和本地的IP地址是等价的
```

## 端口

端口的作用是定位到主机上的某一个进程，通过这个端口，进程就可以接受到对应的网络数据了。

端口也是一个整形数 `unsigned short`，一个 `16` 位整形数，有效端口的取值范围是: `0~65535`（`2^16-1`）

# 网络协议

网络协议指的是计算机网络中互相通信的对等实体之间交换信息时所必须遵守的规则的集合。一般系统网络协议包括五个部分: 通信环境，传输服务，词汇表，信息的编码格式，时序、规则和过程。

# `socket` 编程

与套接字相关的函数被包含在头文件 `sys/socket.h` 中。

## 字节序

在各种计算机体系结构中，对于字节、字等的存储机制有所不同，因而引发了计算机通信领域中一个很重要的问题，即通信双方交流的信息单元（比特、字节、字、双字等等）应该以什么样的顺序进行传送。如果不达成一致的规则，通信双方将无法进行正确的编 / 译码从而导致通信失败。

字节序，顾名思义字节的顺序，就是大于一个字节类型的数据在内存中的存放顺序，也就是说对于单字符来说是没有字节序问题的。

目前在各种体系的计算机中通常采用的字节存储机制主要有两种: `Big-Endian` 和 `Little-Endian`。

- `Little-Endian` -> 主机字节序（小端）

  - 数据的低位字节存储到内存的低地址位，数据的高位字节存储到内存的高地址位
  - 我们使用的 `PC` 机，数据的存储默认使用的是小端

- `Big-Endian` -> 网络字节序（大端）
- 据的低位字节存储到内存的高地址位，数据的高位字节存储到内存的低地址位
- 套接字通信过程中操作的数据都是大端存储的，包括: 接收/发送的数据、`IP` 地址、端口。

![001]()

![002]()

- 函数

```c
#include <arpa/inet.h>
// u:unsigned
// 16: 16位, 32:32位
// h: host, 主机字节序
// n: net, 网络字节序
// s: short
// l: int

// 这套api主要用于 网络通信过程中 IP 和 端口 的转换
// 将一个短整形从主机字节序 -> 网络字节序
uint16_t htons(uint16_t hostshort);	
// 将一个整形从主机字节序 -> 网络字节序
uint32_t htonl(uint32_t hostlong);	

// 将一个短整形从网络字节序 -> 主机字节序
uint16_t ntohs(uint16_t netshort)
// 将一个整形从网络字节序 -> 主机字节序
uint32_t ntohl(uint32_t netlong);
```

除了向 `sockaddr_in` 结构体变量填充数据外，其他情况不需要考虑字节序。

## `IP` 地址转换

虽然 `IP` 地址本质是一个整形数，但是在使用的过程中都是通过一个字符串来描述，下面的函数描述了如何将一个字符串类型的 `IP` 地址进行大小端转换:

```c
// 主机字节序的IP地址转换为网络字节序
// 主机字节序的IP地址是字符串, 网络字节序IP地址是整形
int inet_pton(int af, const char *src, void *dst); 
```

- 参数:
  - `af`: 地址族 (`IP` 地址的家族包括 `ipv4` 和 `ipv6`) 协议
    - `AF_INET`: `ipv4` 格式的 `ip` 地址
    - `AF_INET6`: `ipv6` 格式的 `ip` 地址
  - `src`: 传入参数，对应要转换的点分十进制的 `ip` 地址: `192.168.1.100`
  - `dst`: 传出参数，函数调用完成，转换得到的大端整形 `IP` 被写入到这块内存中
- 返回值: 成功返回 `1`，失败返回 `0` 或者 `-1`

```c
#include <arpa/inet.h>
// 将大端的整形数, 转换为小端的点分十进制的IP地址        
const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
```

- 参数:
  - `af`: 地址族协议
    - `AF_INET`: `ipv4` 格式的 `ip` 地址
    - `AF_INET6`: `ipv6` 格式的 `ip` 地址
  - `src`: 传入参数，这个指针指向的内存中存储了大端的整形 `IP` 地址
  - `dst`: 传出参数，存储转换得到的小端的点分十进制的 `IP` 地址
  - `size`: 修饰 dst 参数的，标记 `dst` 指向的内存中最多可以存储多少个字节
  - 返回值:
    - 成功: 指针指向第三个参数对应的内存地址，通过返回值也可以直接取出转换得到的 `IP` 字符串
    - 失败: `NULL`

还有一组函数也能进程 `IP` 地址大小端的转换，但是只能处理 `ipv4` 的 `ip` 地址:

```c
// 点分十进制IP -> 大端整形
in_addr_t inet_addr (const char *cp);

// 大端整形 -> 点分十进制IP
char* inet_ntoa(struct in_addr in);
// 点分十进制IP ->  大端整形
char* inet_aton(const char* string, struct in_addr* in);
```

注意，`inet_ntoa` 返回 `char*` 类型，函数内部申请了内存并保存了该字符串，调用完该函数后，应立即将字符串信息复制到其他内存空间，否则再次调用可能覆盖。

## `sockaddr` 数据结构

![sockaddr](./Image/sockaddr.png)

```c
// 在写数据的时候不好用
struct sockaddr {
	sa_family_t sa_family;       // 地址族协议, ipv4
	char        sa_data[14];     // 端口(2字节) + IP地址(4字节) + 填充(8字节)
}

typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;
typedef unsigned short int sa_family_t;
#define __SOCKADDR_COMMON_SIZE (sizeof (unsigned short int))

struct in_addr
{
    in_addr_t s_addr;
};  

// sizeof(struct sockaddr) == sizeof(struct sockaddr_in)
struct sockaddr_in
{
    sa_family_t sin_family;		/* 地址族协议: AF_INET */
    in_port_t sin_port;         /* 端口, 2字节-> 大端  */
    struct in_addr sin_addr;    /* IP地址, 4字节 -> 大端  */
    /* 填充 8字节 */
    unsigned char sin_zero[sizeof (struct sockaddr) - sizeof(sin_family) -
               sizeof (in_port_t) - sizeof (struct in_addr)];
};  
```

## 测试

[大端小端转换测试](./src/endianTest.cpp)

## 套接字函数

使用套接字通信函数需要包含头文件 `<arpa/inet.h>`，包含了这个头文件 `<sys/socket.h>` 就不用在包含了。

```c
// 创建一个套接字
int socket(int domain, int type, int protocol);
```

- 参数:
  - `domain`: 使用的地址族协议
    - `AF_INET`: 使用 `IPv4` 格式的 `ip` 地址
    - `AF_INET6`: 使用 `IPv6` 格式的 `ip` 地址
  - `type`:
    - `SOCK_STREAM`: 使用流式的传输协议
    - `SOCK_DGRAM`: 使用报式 （报文） 的传输协议
  - `protocol`: 一般写 `0` 即可，使用默认的协议
    - `SOCK_STREAM`: 流式传输默认使用的是 `TCP`
    - `SOCK_DGRAM`: 报式传输默认使用的 `udp`
- 返回值:
    - 成功: 可用于套接字通信的文件描述符
    - 失败: `-1`

函数的返回值是一个文件描述符，通过这个文件描述符可以操作内核中的某一块内存，网络通信是基于这个文件描述符来完成的。

```c
// 将文件描述符和本地的IP与端口进行绑定   
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

- 参数:
  - `sockfd`: 监听的文件描述符，通过 `socket()` 调用得到的返回值
  - `addr`: 传入参数，要绑定的 `IP` 和端口信息需要初始化到这个结构体中，`IP` 和端口要转换为网络字节序
  - `addrlen`: 参数 `addr` 指向的内存大小，`sizeof(struct sockaddr)`
- 返回值: 成功返回 `0`，失败返回 `-1`

```c
// 给监听的套接字设置监听
int listen(int sockfd, int backlog);
```

- 参数:
  - `sockfd`: 文件描述符，可以通过调用 `socket()` 得到，在监听之前必须要绑定 `bind()`
  - `backlog`: 同时能处理的最大连接要求，最大值为 `128`
- 返回值: 函数调用成功返回 `0`，调用失败返回 `-1`

```c
// 等待并接受客户端的连接请求, 建立新的连接, 会得到一个新的文件描述符(通信的)		
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

- 参数:
  - `sockfd`: 监听的文件描述符
  - `addr`: 传出参数，里边存储了建立连接的客户端的地址信息
  - `addrlen`: 传入传出参数，用于存储 `addr` 指向的内存大小
- 返回值: 函数调用成功，得到一个文件描述符，用于和建立连接的这个客户端通信，调用失败返回 `-1`

这个函数是一个阻塞函数，当没有新的客户端连接请求的时候，该函数阻塞；当检测到有新的客户端连接请求时，阻塞解除，新连接就建立了，得到的返回值也是一个文件描述符，基于这个文件描述符就可以和客户端通信了。

```c
// 接收数据
ssize_t read(int sockfd, void *buf, size_t size);
ssize_t recv(int sockfd, void *buf, size_t size, int flags);
```

- 参数:
  - `sockfd`: 用于通信的文件描述符，`accept()` 函数的返回值
  - `buf`: 指向一块有效内存，用于存储接收是数据
  - `size`: 参数 `buf` 指向的内存的容量
  - `flags`: 特殊的属性，一般不使用，指定为 `0`
- 返回值:
  - 大于 `0`: 实际接收的字节数
  - 等于 `0`: 对方断开了连接
  - `-1`: 接收数据失败了

如果连接没有断开，接收端接收不到数据，接收数据的函数会阻塞等待数据到达，数据到达后函数解除阻塞，开始接收数据，当发送端断开连接，接收端无法接收到任何数据，但是这时候就不会阻塞了，函数直接返回 `0`。

```c
// 发送数据的函数
ssize_t write(int fd, const void *buf, size_t len);
ssize_t send(int fd, const void *buf, size_t len, int flags);
```

- 参数:
  - `fd`: 通信的文件描述符，`accept()` 函数的返回值
  - `buf`: 传入参数，要发送的字符串
  - `len`: 要发送的字符串的长度
  - `flags`: 特殊的属性，一般不使用，指定为 `0`
- 返回值:
  - 大于 `0`: 实际发送的字节数，和参数 `len` 是相等的
  - `-1`: 发送数据失败了

```c
// 成功连接服务器之后, 客户端会自动随机绑定一个端口
// 服务器端调用accept()的函数, 第二个参数存储的就是客户端的IP和端口信息
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

- 参数:
  - `sockfd`: 通信的文件描述符，通过调用 `socket()` 函数就得到了
  - `addr`: 存储了要连接的服务器端的地址信息: `iP` 和 `端口`，这个 `IP` 和 `端口` 也需要转换为大端然后再赋值
  - `addrlen`: `addr` 指针指向的内存的大小 `sizeof(struct sockaddr)`
- 返回值: 连接成功返回 `0`，连接失败返回 `-1`

# `TCP` 通信流程

`TCP` 是一个面向连接的，安全的，流式传输协议，这个协议是一个传输层协议。

- 面向连接: 是一个双向连接，通过三次握手完成，断开连接需要通过四次挥手完成。
- 安全: `TCP` 通信过程中，会对发送的每一数据包都会进行校验，如果发现数据丢失，会自动重传
- 流式传输: 发送端和接收端处理数据的速度，数据的量都可以不一致

![TCP](./Image/tcp.jpg)

- 在 `TCP` 的服务器端，有两类文件描述符
  - 监听的文件描述符
    - 只需要有一个
    - 不负责和客户端通信，负责检测客户端的连接请求，检测到之后调用 `accept` 就可以建立新的连接
  - 通信的文件描述符
    - 负责和建立连接的客户端通信
    - 如果有 `N` 个客户端和服务器建立了新的连接，通信的文件描述符就有 `N` 个，每个客户端和服务器都对应一个通信的文件描述符

![buffer](./Image/buffer.png)

- 文件描述符对应的内存结构:
  - 一个文件文件描述符对应两块内存, 一块内存是读缓冲区, 一块内存是写缓冲区
  - 读数据: 通过文件描述符将内存中的数据读出, 这块内存称之为读缓冲区
  - 写数据: 通过文件描述符将数据写入到某块内存中, 这块内存称之为写缓冲区

- 监听的文件描述符:
  - 客户端的连接请求会发送到服务器端监听的文件描述符的读缓冲区中
  - 读缓冲区中有数据，说明有新的客户端连接
  - 调用 `accept()` 函数，这个函数会检测监听文件描述符的读缓冲区
    - 检测不到数据，该函数阻塞
    - 如果检测到数据，解除阻塞，新的连接建立

- 通信的文件描述符:
  - 客户端和服务器端都有通信的文件描述符
  - 发送数据: 调用函数 `write() / send()`，数据进入到内核中
    - 数据并没有被发送出去，而是将数据写入到了通信的文件描述符对应的写缓冲区中
    - 内核检测到通信的文件描述符写缓冲区中有数据，内核会将数据发送到网络中
  - 接收数据: 调用的函数 `read () / recv ()`, 从内核读数据
    - 数据如何进入到内核我们不需要关系，数据进入到通信的文件描述符的读缓冲区中
    - 数据进入到内核，必须使用通信的文件描述符，将数据从读缓冲区中读出即可

基于 `TCP` 的服务器端通信代码

[server.cpp](./src/TCPTest/server_linux.cpp)


基于 `TCP` 通信的客户端通信代码:

[client.cpp](./src/TCPTest/client_linux.cpp)

# `Window` 套接字通信

## 初始化套接字环境

使用 `Windows` 中的套接字函数需要额外包含对应的头文件以及加载响应的动态库:

```c
// 使用包含的头文件 
include <winsock2.h>
// 使用的套接字库 
ws2_32.dll       
```

在 `Windows` 中使用套接字需要先加载套接字库（套接字环境），最后需要释放套接字资源。

```c
// 初始化Winsock库
// 返回值: 成功返回0，失败返回SOCKET_ERROR。
WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
```

- 参数:
  - `wVersionRequested`: 使用的 `Windows Socket` 的版本，一般使用的版本是 `2.2`
    - 初始化这个 `MAKEWORD(2, 2);` 参数
  - `lpWSAData`: 一个 `WSADATA` 结构指针，这是一个传入参数
    - 创建一个 `WSADATA` 类型的变量，将地址传递给该函数的第二个参数

注销 `Winsock` 相关库，函数调用成功返回 `0`，失败返回 `SOCKET_ERROR`。

```c
int WSACleanup (void);
```

## 套接字通信函数

和 `Linux` 平台上的通信流程完全相同。

### 结构体

```c
///////////////////////////////////////////////////////////////////////
/////////////////////////////// Windows ///////////////////////////////
///////////////////////////////////////////////////////////////////////
typedef struct in_addr {
　　union {
　　	struct{ unsigned char s_b1,s_b2, s_b3,s_b4;} S_un_b;
　　	struct{ unsigned short s_w1, s_w2;} S_un_w;
　　	unsigned long S_addr;	// 存储IP地址
　　} S_un;
}IN_ADDR;

struct sockaddr_in {
　　short int sin_family; /* Address family */
　　unsigned short int sin_port; /* Port number */
　　struct in_addr sin_addr; /* Internet address */
　　unsigned char sin_zero[8]; /* Same size as struct sockaddr */
};

///////////////////////////////////////////////////////////////////////
//////////////////////////////// Linux ////////////////////////////////
///////////////////////////////////////////////////////////////////////
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;
typedef unsigned short int sa_family_t;

struct in_addr
{
    in_addr_t s_addr;
};  

// sizeof(struct sockaddr) == sizeof(struct sockaddr_in)
struct sockaddr_in
{
    sa_family_t sin_family;     /* 地址族协议: AF_INET */
    in_port_t sin_port;         /* 端口, 2字节-> 大端  */
    struct in_addr sin_addr;    /* IP地址, 4字节 -> 大端  */
    /* 填充 8字节 */
    unsigned char sin_zero[sizeof (struct sockaddr) - sizeof(sin_family) -
                      sizeof (in_port_t) - sizeof (struct in_addr)];
};  
```

### 大小端转换函数

```c
// 主机字节序 -> 网络字节序
u_short htons (u_short hostshort );
u_long htonl ( u_long hostlong);

// 网络字节序 -> 主机字节序
u_short ntohs (u_short netshort );
u_long ntohl ( u_long netlong);

// linux函数, window上没有
inet_ntop(); 
inet_pton();
inet_aton();

// windows 和 linux 都使用, 只能处理ipv4的ip地址
// 点分十进制IP -> 大端整形
unsigned long inet_addr (const char FAR * cp);	// windows
in_addr_t     inet_addr (const char *cp);			// linux

// 大端整形 -> 点分十进制IP
// window, linux相同
char* inet_ntoa(struct in_addr in);
```

### 套接字函数

`window` 的 `api` 中套接字对应的类型是 `SOCKET` 类型，`linux` 中是 `int` 类型, 本质是一样的。

```c
// 创建一个套接字
// 返回值: 成功返回套接字, 失败返回INVALID_SOCKET
SOCKET socket(int af,int type,int protocal);
参数:
    - af: 地址族协议
        - ipv4: AF_INET (windows/linux)
        - PF_INET (windows)
        - AF_INET == PF_INET
   - type: 和linux一样
       	- SOCK_STREAM
        - SOCK_DGRAM
   - protocal: 一般写0 即可
       - 在windows上的另一种写法
           - IPPROTO_TCP, 使用指定的流式协议中的TCP协议
           - IPPROTO_UDP, 使用指定的报式协议中的udp协议

 // 关键字: FAR NEAR, 这两个关键字在32/64位机上是没有意义的, 指定的内存的寻址方式
// 套接字绑定本地IP和端口
// 返回值: 成功返回0，失败返回SOCKET_ERROR
int bind(SOCKET s,const struct sockaddr FAR* name, int namelen);

// 设置监听
// 返回值: 成功返回0，失败返回SOCKET_ERROR
int listen(SOCKET s,int backlog);

// 等待并接受客户端连接
// 返回值: 成功返回用于的套接字，失败返回INVALID_SOCKET。
SOCKET accept ( SOCKET s, struct sockaddr FAR* addr, int FAR* addrlen );

// 连接服务器
// 返回值: 成功返回0，失败返回SOCKET_ERROR
int connect (SOCKET s,const struct sockaddr FAR* name,int namelen );

// 在Qt中connect用户信号槽的连接, 如果要使用windows api 中的 connect 需要在函数名前加::
::connect(sock, (struct sockaddr*)&addr, sizeof(addr));

// 接收数据
// 返回值: 成功时返回接收的字节数，收到EOF时为0，失败时返回SOCKET_ERROR。
//		==0 代表对方已经断开了连接
int recv (SOCKET s,char FAR* buf,int len,int flags);

// 发送数据
// 返回值: 成功返回传输字节数，失败返回SOCKET_ERROR。
int send (SOCKET s,const char FAR * buf, int len,int flags);

// 关闭套接字
// 返回值: 成功返回0，失败返回SOCKET_ERROR
int closesocket (SOCKET s);		// 在linux中使用的函数是: int close(int fd);

//----------------------- udp 通信函数 -------------------------
// 接收数据
int recvfrom(SOCKET s,char FAR *buf,int len,int flags,
         struct sockaddr FAR *from,int FAR *fromlen);
// 发送数据
int sendto(SOCKET s,const char FAR *buf,int len,int flags,
       const struct sockaddr FAR *to,int tolen);
```

[server.cpp](./src/TCPTest/server_win.cpp)

[client.cpp](./src/TCPTest/client_win.cpp)
