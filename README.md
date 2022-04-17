# <center>**数据安全传输基础设施平台** 
## 系统文件
### Client端文件
<center>![Client.png](https://s2.loli.net/2022/04/17/7EmYSyatGjhVT2N.png)</center>
### Server端文件
<center>![Server.png](https://s2.loli.net/2022/04/17/cO6jURXhmP8AQ37.png)</center>
---
# 一、基本概念
## 1.1 应用场景    

 &emsp;&emsp;涉及到网络数据传输的，对数据传输安全有需求的，就需要对数据进行加密；加密就需要用到秘钥，秘钥如何产生？ ----> 就用到了秘钥协商系统。秘钥协商服务系统就是用来生成秘钥的, 该秘钥是对称秘钥。

## 1.2 总体流程

1. 客户端发起密钥协商请求，并向服务端发送随机字符串(假设为abc)；
2. 服务端收到客户端的随机字符串之后，自己也生成一个随机字符串（假设为123），且将123发送给客户端；
3. 服务端将abc123作为生成密钥的原材料，通过**某种算法**生成一个新的密钥；
4. 客户端收到服务端发送的123之后，也进行组合abc123，也使用与服务器相同的算法，也生成新的密钥（这个密钥应该与生成的密钥一样）。
5. 当客户端和服务端生成密钥后，可以通过**密钥校验**，验证客户端与服务端生成的密钥是否一致；
6. 客户端和服务端都将密钥写入**共享内存**，供其他模块调用；   
<center>![项目框架.png](https://s2.loli.net/2022/03/01/PzNcwdVfFbn3lZm.png  "项目框架图")  
**图1-1 项目框架**    

## 1.3 知识点归纳
**1. 网络通信**  
&emsp;&emsp;socket API函数：socket bind listen accept read | recv send write  
&emsp;&emsp;三次握手和四次挥手  
&emsp;&emsp;TCP协议  
&emsp;&emsp;服务端开发技术：  
&emsp;&emsp;&emsp;&emsp;多进程、多线程、多路IO复用技术：select poll epoll 、多路IO复用与多进程或者多进程结合使用、第三方库：libevent  
&emsp;&emsp;服务端基本开发流程：  
		&emsp;&emsp;&emsp;&emsp;1、socket  
		&emsp;&emsp;&emsp;&emsp;2、setsockopt(设置端口复用)  
		&emsp;&emsp;&emsp;&emsp;3、bind  
		&emsp;&emsp;&emsp;&emsp;4、listen  
		&emsp;&emsp;&emsp;&emsp;5、while(1){}    
&emsp;&emsp;客户端基本开发流程：  
		&emsp;&emsp;&emsp;&emsp;1、socket  
		&emsp;&emsp;&emsp;&emsp;2、connect   
		&emsp;&emsp;&emsp;&emsp;3、while(1){}   
**2. 报文编解码**  
&emsp;&emsp;为了兼容不同的操作系统。  
**3. 进程间通信**  
&emsp;&emsp;方式：  
&emsp;&emsp;&emsp;&emsp;pipe、fifo、mmap、本地套接字、网络套接字、**共享内存**；  
**4. 数据库相关知识**  
**5. QT相关知识**   
**6. 信号相关知识**   
&emsp;&emsp;SIGUSR1、SIGUSR2；  
**7. shell编程相关知识**   
**8. 加密算法相关**  
**9. 守护进程相关**  
**10. 多线程开发**  
**11. C++基础概念**  
&emsp;&emsp;封装（将数据和行为进行封装，并添加相应的权限）   
&emsp;&emsp;继承（子类继承父类所有的属性和方法（共性），也有自己的属性和方法（特性））   
&emsp;&emsp;多态（继承、父类指针或引用指向子类对象、子类重写父类虚函数）  

## 1.4 模块划分
1. 报文编解码模块
2. 网络通信模块
3. 共享内存操作模块
4. 数据库操作模块
5. 外联接口模块

## 1.5 数据的加密和解密

### 1.5.1 加密三要素
  
1. 密钥  
2. 算法
3. 明文/密文        
<center>![数据的加密和解密.png](https://s2.loli.net/2022/03/01/Sx3AwYCu7LTRvGi.png#pic_center)   
**图1-2 数据的加密和解密**    

### 1.5.2 常用的加密方式  
1. 对称加密
	1. 加密和解密所使用的是通过一个密钥
	2. 特点
		- 密钥分发困难
		- 加密效率高
2. 非对称加密
	1. 加密和解密所使用的密钥不同--是一个密钥对
		- 公钥--可以公开的密钥
		- 私钥--不能公开
	2. 传输数据对谁更重要，谁就使用私钥解密
		- 如果使用公钥加密，必须使用私钥解密
		- 如果使用私钥加密，必须使用公钥解密
	3. 特点
		- 密钥分发简单
		- 加密效率低
		- 安全级别高
### 1.5.3 常用的加密算法
1. **对称加密**
	1. DES/3DES  
		- DES--已经被破解了，要求密钥长度为8字节。
		- 3DES--密钥长度为24字节
	2. AES
		- 使用最广泛的对称算法
		- 密钥要求：16、24、32；密钥长度越长效率越低，但安全性更高。
2. **非对称加密**
	1. RSA（数字签名和密钥交换）
2. **Hash算法（单向散列函数）** --Hash算法不是用于加密的
	1. 将任意长度的数据，生成一个固定长度的字符串
	2. MD4/MD5
		- 散列值长度为16个字节
	3. SHA-1
		- 散列值长度为20字节
	4. SHA-2
		- SHA224/SHA256(后面的数字单位是位，除8之后为散列值的长度)
### 1.5.4 Windows下openssl的使用


# 二、报文编解码

## 2.1 为什么要进行报文编解码？
1. 两台机器的操作系统平台不同:  
&emsp;&emsp;一台是32为windows, 一台是linux 64系统   

2. 网络传输的时候用的是大端模式  
&emsp;&emsp;只要是传输的数据的数据类型超过了一个字节都需要进行网络字节序的转换  
&emsp;&emsp;关于字节序序的函数:  
&emsp;&emsp;htons htonl ntohs ntohl   
&emsp;&emsp;&emsp;&emsp;h: host  
&emsp;&emsp;&emsp;&emsp;to: 到    
&emsp;&emsp;&emsp;&emsp;n: network    
&emsp;&emsp;&emsp;&emsp;l: long    
&emsp;&emsp;&emsp;&emsp;s: short    
&emsp;&emsp;假如在从A传递数据给B, 需要传递一个结构体  

3. 字节序对齐不一样    

		 struct test  
		 {  
		 	int a;  0-3
		 	char c;  4-7
		 	double d;  8-15
		 }  

4. 两台服务器的开发语言不同  
&emsp;&emsp;对于C语言, 一个字符占用一个字节  
&emsp;&emsp;对于java, 一个字符占用2个字节  

**报文: 其实就是数据。**  
上述几种情况, 涉及到网络数据传输, 都需要对数据进行编解码操作。

## 2.2 大端模式和小端模式
以数字0x12 34 56 78在内存中的表示形式为例。
- **大端模式：**高位字节排放在内存的低地址端，低位字节排放在内存的高地址端。  
&emsp;&emsp;低地址 --------------------> 高地址  
&emsp;&emsp; 0x12  |  0x34  |  0x56  |  0x78
- **小端模式：**低位字节排放在内存的低地址端，高位字节排放在内存的高地址端  
&emsp;&emsp;低地址 --------------------> 高地址  
&emsp;&emsp;0x78  |  0x56  |  0x34  |  0x12  

**通过代码判断CPU对内存所采用的读写模式**：  

	在union中所有的数据成员共用一个存储空间，在同一时间只能存储其中的一个数据成员，所有的数据成员具相同的起始地址，相对于基地址的偏移量都为0
	BOOL IsBigEndian()  
	{  
	    union NUM  //联合体union的存放顺序是所有成员都从低地址开始存放
	    {  
	        int a;    // 低地址
	        char b;   // 高地址
	    }num;  
	    num.a = 0x1234;  
	    if( num.b == 0x12 )  
	    {  
	        return TRUE;  // 小端模式
	    }  
	    return FALSE;  // 大端模式
	}
	
## 2.3 UML类图
单向关联关系:  
&emsp;&emsp;使用带有箭头的实线表示, 箭头指向类的成员对象.

双向关联关系:  
&emsp;&emsp;使用带有双向箭头的实线来表示, 两个类相互包含
	
自关联:  
&emsp;&emsp;最常见的就是链表操作, 箭头执行类自己.

聚合关系:  
&emsp;&emsp;使用带有空心菱形的带箭头的实线来表示, 空心的菱形在聚合类一侧, 箭头指向成员对象

组合关系:  
&emsp;&emsp;使用带有实心菱形的带箭头的实线来表示, 实心的菱形在组合类一侧, 箭头指向成员对象
	
聚合和组合:  
&emsp;&emsp;都是整体和部分的关系.

依赖关系: 类中的成员函数的参数用到了某个类的对象  
&emsp;&emsp;使用带有箭头的虚线表示, 箭头指向被依赖的类.

## 2.4 几种常见的报文编解码方式

1. XML  
	- 以标签的形式表现

2. Json  
	- 以键值对的形式表现

3. Protocal Buffer

4. **ASN.1抽象语法标记（Abstract Syntax Notation One）**
	1.	编码规则：  	

    BER（Basic Encoding Rules）。BER传输语法的格式一直都是TLV三元组<Type,Length,Value>或者是<Tag,Length,Value>。TLV每个域都是一系列八位组，对于组合结构，其中V还可以是TLV三元组。BER传输语法是基于八位组（为了避免不同系统上的混淆，没有采用Byte为单位），自定界的编码，因为其中L明确界定了八位组的长度。BER是大端编码的，其八位组的高位比特再左手边。  
	BER编码中的Tag（通常是一个八位组），指定了值的类型，其中一个比特表征是基本类型还是组合类型。        
**先将结构体数据转换成一个TLV形式的链表，再将整个链表转换为一个TLV格式。**
	
5. boost序列化的类

6. 自定义格式
<center>![ASN_1 _2_.png](https://s2.loli.net/2022/04/17/eZ57spluRof1WxU.png)    
**图 ASN.1 BER编码规则**   

用这些格式组织数据，将数据转换成数据字节流，服务端收到后进行解析。


# 三、工厂模式  
## 3.1 基础概念  
工厂: 使用一个单独的类来做创建实例的过程, 这就是工厂。
  
简单工厂：把对象的创建放到一个工厂类中，通过参数来创建不同的对象。 特点：  

- 缺点：每添一个对象，就需要对简单工厂进行修改（尽管不是删代码，仅仅是添一个switch case，但仍然违背了“不改代码”的原则, 尽量做到添加代码而不是修改原有代码）  
- 优点：去除了与具体产品的依赖。  

使用流程：
1. 创建一类, 用于生产对象, 这个类就是工厂类
2. 在这个类中添加一工厂函数

工厂类的使用：  

	// 1. 创建工厂类
	Factory* factory = new Factory();
	// 2. 使用工厂函数创建对象, 返回父类指针
	Codec*codec = factory->createFactory(0);
	// 3. 编解码
	codec->msgEncode();  

## 3.2 工厂模式--需要有N个工厂类

工厂方法：每种产品由一种工厂来创建, 不同工厂创建不同的对象  

特点：基本完美，完全遵循 “不改代码”的原则  
- 创建一个工厂类 - 基类
- 在基类的工厂类中添加工厂函数, 这是一个虚函数
- 根据要创建的子对象添加子工厂类, 每个子对象的创建都对应一个子工厂类
- 在子工厂类中实现父类的工厂函数, 完成创建对象的操作 

# 四、单向散列函数
## 4.1 基础概念
&emsp;&emsp;单向散列函数（one-wayftnction）有一个输入和一个输出，其中输入称为消息（message），输出称为散列值（hashvalue）。单向散列函数可以根据消息的内容计算出散列值，而散列值就可以被用来检查消息的完整性。
- 单向散列函数也称为消息摘要函数（message digest function）、哈希函数或者杂凑函数。
- 单向散列函数输出的散列值也称为消息摘要（message digest）或者指纹（fingerprint）。
- 完整性也称为一致性。

**哈希函数不能用来加密，用来保证消息的不变性；**

## 4.2 哈希函数的特点
- 压缩性
	- 任意长度的数据，算出的值长度都是固定的。
- 容易计算
	- 计算散列值所花费的时间必须要短。尽管消息越长，计算散列值的时间也会越长，但如果不能在现实的时间内完成计算就没有意义了。
- 抗修改性
	- 对原数据进行任何改动，哪怕只修改1个字节，所得到的值都有很大区别。
- 强抗碰撞性
	- 已知原数据和其哈希值，想找到一个具有相同哈希值的数据（即伪造数据）是非常困难的。
- 单向性（不可逆）
 
## 4.2 哈希函数的应用
1. 检测软件是否被篡改
2. 消息认证码  
对于发送端:  
&emsp;&emsp;将要发送的数据进行哈希运算, 参与运算的数据是: 原始数据+秘钥, 可以得到一个散列值将散列值与原始数据进行拼接, 一起发送给对方;
对于接收端:  
&emsp;&emsp;接收对方发来的数据, 并将原始数据和散列值拆分开, 得到散列值和原始数据对原始数据进行哈希运算: 参与运算的数据也是:原始数据+秘钥, 可以得到一个散列值将计算得到的散列值与接受到的散列值进行比较, 相同则认为没有被篡改, 否则认为被篡改了。
3. 数字签名  
A将明文进行哈希运算得到一个散列值, 并且用私钥对哈希值进行加密, 然后将明文和加了密的哈希值一起发送给B;  
B收到之后, 使用公钥对哈希值进行解密, 得到原始的哈希值, 并且对明文进行哈希运算也得到一个哈希值, 最后对自己生成的哈希值和A发送过来的哈希值进行比较, 如果一样, 则认为没有被篡改。使用数字签名的目的是为了不可抵赖性.(表明拿私钥的一方发送的数据, 是不可抵赖的）。
4. 一次性口令或登录验证

## 4.3 常用的哈希函数
1. Md4、Md5
	- 16个字节
2. SHA-1、SHA-256、SHA-384、SHA-512
	- SHA-1的消息长度存在上限，但这个值接近2^64bit;
	- SHA-1产生20字节的散列值 

**通过哈希函数得到的散列值需要转换成16进制的格式进行表示。**

# 五、日志相关
- 日志的作用--记录系统运行时候的信息，包括正确信息和错误信息。
- 日志级别越高打印的日志越少，反之越多。
- 日志的本质是写文件操作，反复的写文件会影响系统效率。

# 六、客户端的开发流程

1. 创建socket,得到一个通信的文件描述符cfd   

 
    int socket(int domain, int type, int protocol);
2. 绑定---不是必须的  


	int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
3. connect服务端  


	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
  注意:在给IP和端口赋值的时候需要进行主机字节序到网络字节序的转换--htonl htons

4.

	 while(1)
	  {
	  	//发送数据
	  	ssize_t write(int fd, const void *buf, size_t count);
	  	int send(int s, const void *msg, size_t len, int flags);
	  	
	  	//接收数据
	  	ssize_t read(int fd, void *buf, size_t count);
	  	ssize_t recv(int sockfd, void *buf, size_t len, int flags);
	  }

5 关闭连接  


	int close(int fd);
	int shutdown(int sockfd, int how); // 可以只关闭写端和读端（半关闭）  

**需要注意的是: 读或者写都是操作的内核的缓冲区, 并不赋值发送或接收数据, 发送或者接受数据是有内核实现的。**

# 七、服务端的开发流程  
1. 创建socket,得到一个监听的文件描述符lfd  


	int socket(int domain, int type, int protocol);
2. 设置端口复用(当服务端关闭后可以立即启动)  


	int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
3. 绑定--必须的  


	int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
	INADDR_ANY: 本地任意可用IP
	IP+端口可以唯一确定网络中唯一一台主机的唯一一个服务
4. 监听


	int listen(int s, int backlog);
5. 调用accept函数接受新的客户端连接


	int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
	--返回一个通信的文件描述符cfd
	--addr: 输出参数, 客户端的地址信息
	--addrlen: 输入输出参数, 输入: addr内存大小, 输出: 传输的地址大小  
6. 


	while(1)
	  {
	  	//接收数据
	  	ssize_t read(int fd, void *buf, size_t count);
	  	ssize_t recv(int sockfd, void *buf, size_t len, int flags);
	  	
	  	//发送数据
	  	ssize_t write(int fd, const void *buf, size_t count);
	  	int send(int s, const void *msg, size_t len, int flags);
	  }

7. 关闭套接字
 	
	
	close(fd);

# 八、网络通信相关概念
1. **同步和异步**  
  同步: 例如, 客户端发送数据给服务端, 发送完成之后, 就read阻塞等待读数据, 读数据不完成,则不进行后续操作。  
  异步: 例如: 客户端发送数据给服务端, 发送完成之后, 没有等待read数据, 而是处理后续操作。 
2. **阻塞和非阻塞**  
  阻塞: 比如accept和read都是阻塞函数, 条件不满足, 就一直阻塞等待。    
  非阻塞: 如将文件描述符设置为非阻塞(调用fcntl函数), 则read函数会立刻返回。    
  同步非阻塞: 若客户端发送数据之后, read函数不阻塞(文件描述符设置为非阻塞)。    
  同步阻塞:客户端发送数据之后, read数据, 若对方不发送应答数据, 就一直阻塞。    
  异步阻塞: 比如: select poll epoll, 若没有事件发生, select 或者epoll可以一直阻塞。  
  异步非阻塞: 比如: 将epoll设置非阻塞, 不管有没有事件发生都会立刻返回。   

3. **长连接和短连接**  
  长连接: 通常用于通信双方**数据交换频繁**的情况下。  
&emsp;&emsp;--心跳包. 给对方发一个字符串，对方回一个。  
&emsp;&emsp;0004AAAA ---- 0004BBBB（前四个字节表示长度，后四个自己表示数据）  
&emsp;&emsp;00101234567890  
  短连接: 通常用于通信双方数据交换完成后就断开连接。 

同步短连接：  
异步长连接：   

# 九、网络相关知识  
1. 一个文件描述符对应两个内核缓冲区(读缓冲区和写缓冲区)。
2. 三次握手
	1. 客户端发送连接请求(SYN)
	2. 服务端响应(ACK)并发送连接请求(SYN)
	3. 客户端响应(ACK)
3. 四次挥手
	1. 两者都可以发送关闭连接请求（假设服务端先请求），服务算发送断开连接请求（FIN）以及（ACK）（中间连接都会有ACK）
	2. 客户端响应(ACK)
	3. 客户端发送关闭连接请求(FIN)
	4. 服务端响应(ACK)   
	 
**TIME_WAIT处于主动关闭方**。
4. 滑动窗口：主要作用是进行流量控制。  
5. TCP状态转换图：  

	- CLOSED
	- SYN_SENT
	- SYN_RCVD
	- LISTEN
	- ESTABLISHEN
	- FIN_WAIT_1
	- FIN_WAIT_2
	- TIME_WAIT
	- CLOSE_WAIT
	- LAST_WAIT  

6. 多进程版本的服务器  
7. 多线程版本的服务器   
8. 多路IO复用: select poll epoll  
   epoll反应堆:将事件和事件对应的回调函数进行封装   
   线程池:主要是思想搞明白--其实就是生产者和消费者模型  
   线程相关, 互斥锁 条件变量   
9. libevent：用于开发网络服务器      
10. UDP协议 本地socket通信  
11. http协议 html文件  
12. 七层模型和四层TCP/IP模型

# 十、进程间通信  
1. **匿名管道pipe:**  


    int pipe(int pipefd[2]);  
特点:   
- 只能用于有血缘关系的进程间通信  
- 管道有两端, 管道的数据流向是从管道的写端到管道的读端  
- 管道的本质是一块内核缓冲区  
- 数据从管道中读走之后就不存在了  
- **管道的实现实际上是环形队列**  
- 默认情况下管道的读端和写端都是阻塞的  

2. **命名管道: fifo**  


    mkfifo  
   	int mkfifo(const char *pathname, mode_t mode);
特点: 
- 有无血缘关系的进程间通信都可以
- **创建的fifo文件大小为0, 是linux文件类型之一**
- **使用fifo需要先创建一个fifo文件**
- **使用fifo完成通信两个进程必须打开相同的fifo文件**
- 效率比pipe低 (本质上是IO操作) 

3. **mmap（共享映射区）**  


	void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
**共享映射区本质是将文件内容映射到内存.**  
特点:
- 有无血缘关系都可以完成进程间通信
- 如果完成没有血缘关系的进程间通信必须使用文件.
- 若使用的是MAP_SHARED, 则对内存的修改会反映到文件中去
- 需要注意mmap可能存在调用失败的情况
- **匿名映射（没有文件）只能用于有血缘关系的进程间通信**  

4. **信号**  

进程A给进程B发送信号的实现机制: 本质上是进程A先给内核发送信号, 然后内核给进程B发送  
	

	int kill(pid_t pid, int sig);(发送信号)
	signal或者sigaction（注册信号）
	
	相关函数:
		kill(发送信号)
		signal
		sigaction
		abort
		raise（给自己发信号）
		alarm|setitimer（定时发送信号）
		（信号集相关函数：sigemptyset  sigaddset sigfillset sigismember sigdelset sigpromask sigpending）
		SIGHUP SIGINT SIGABRT SIGQUIT SIGALRM SIGTERM  SIGSEGV
		SIGKILL SIGCONT SIGSTOP SIGPIPE SIGUSR1 SIGUSR2 
		SIGCHLD信号（子进程退出父进程回收）
	特点: 
		1. 信号不能携带大量信息
		2. 信号的优先级高, 产生信号之后会打断程序的执行
		3. 不建议使用信号完成进程间通信。（信号优先级太高，可能影响程序指向顺序）
		4. 一般使用kill命令给一个进程发送信号, 进程收到信号之后调用信号处理函数完成操作。
	信号的处理动作:
		1. 忽略信号
		2. 执行默认处理动作
		3. 执行用户自定义的函数  

5. **本地socket通信**


		unix_socket = socket(AF_UNIX, SOCK_STREAM, 0);
		unix_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
1 本地socket通信既可以使用TCP也可以使用UDP  
2 如果使用TCP通信, bind的时候需要指定一个文件, 若文件存在会保存, unlink删除(使文件的引用基数-1).  
3 读或者写其实是通过文件描述符去操作内核的缓冲区  
4 编写流程, 可以直接参考TCP或者UDP开发流程.  



6. **共享内存**  
共享内存的实质是将内核的一块内存映射到进程中的内存, 操作本地内存就相当于操作共享内存.  
	使用共享内存的步骤:  
	1. 创建共享内存  
	2. 关联共享内存  
	3. 使用共享内存--读写共享内存  
	4. 断开与共享内存的关联  
	5. 删除共享内存  

共享内存和以上的管道, mmap和本地socket通信比较起来, 共享内存不需要文件描述符,后者需要。共享内存是进程间通信方式中效率最高的。


# 十一、共享内存  
头文件:


		#include <sys/ipc.h>
		#include <sys/shm.h>
**创建共享内存:**

	int shmget(key_t key, size_t size, int shmflg);
	函数描述: 创建或者获得共享内存ID
	参数:
		key: 一个无符号整形值, 唯一标识了一块共享内存
		size: 创建共享内存的大小
		shmflag: 
			IPC_CREAT: 创建共享内存
				-创建文件并指定权限: IPC_CREAT|0664
			IPC_EXCL: 只能和IPC_CREAT一起使用,若共享内存存在, 则报错, errno=EEXIST
	返回值:
		成功: 返回共享内存的ID值
		失败: 返回-1, 并设置errno

**一般用法:** 


	1. 如果共享内存已经存在:
		key_t key = 0x1234;
		int shmID = shmget(key, 0, 0); 
	2. 如果共享内存不存在:
		int shmID = shmget(key, 100, IPC_CREAT|IPC_EXCL|0755);
		if(shmID<0)
		{
			if(errno==EEXIST)
			{
				shmID = shmget(key, 0, 0); 
				return 0;
			}
		}
	3. 如果不知道到底存不存在:
		int shmID = shmget(key, 100, IPC_CREAT|0755);

**连接共享内存:**	

   	void *shmat(int shmid, const void *shmaddr, int shmflg);
   	函数描述: 连接共享内存
   	参数:
   		shmid: shmget函数返回的共享内存的ID值
   		shmaddr: 传NULL,表示让内核分配地址
   		shmflg:
   			SHM_RDONLY: 只能对共享内存进行读操作
   			0: 可读和可写
   	返回值:
   		成功: 内存地址
   		失败: (void *) -1
   	如: void *p = shmat(shmid, NULL, 0);

**断开与共享内存的关联:**

   	int shmdt(const void *shmaddr);
   	函数描述: 断开与共享内存的关联
   	参数:
   		shmaddr: shmat返回的内存地址
   	返回值:
   		成功: 返回0
   		失败: 返回-1, 并设置errno

**删除共享内存:**

   	int shmctl(int shmid, int cmd, struct shmid_ds *buf);
    函数描述: 设置或者删除共享内存
    函数参数:
    	shmid: shmget函数返回的共享内存的ID值
    	cmd:
    		IPC_STAT: 获得共享内存的状态信息
    			--buf
    		IPC_SET: 设置共享内存信息
    		IPC_RMID: 删除共享内存
    	buf:
    		若cmd为IPC_RMID则buf为NULL
    	
    	如: shmctl(shmid, IPC_RMID, NULL); 

shm与mmap的区别：  
- mmap如果用于没有血缘关系的进程间通信必须使用文件，shm不需要
- shm操作数据的效率比mmap快；
- mmap比shm安全（如果断电mmap使用的文件不会消失，数据可以映射回文件）

相同点：
- mmap和shm都使用的是内存地址。
- mmap和shm都可以用于有无血缘的进程间通信。
- mmap和shm读取数据之后并不会消失。 

**ftok函数:**

	key_t ftok(const char *pathname, int proj_id);
	函数说明:使用文件名生成key值
	函数参数:
		pathname: 带文件路径的文件名, 必须保证文件是存在的, 对文件权限没有要求/bin /home
		proj_id: 只是用一个字节, 取值范围为0-255, 'x'
	返回值:
		成功: 返回key
		失败: 返回-1

# 十二、共享内存在项目中的使用

向共享内存中写入下面的结构体：  
	
	struct NodeSHMInfo{
		int status; // 表示密钥的状态（可用或者不可用）
		int seckeyID;	// 密钥的ID值（每个密钥都不一样）
		char clientID[12];	// 客户端ID
		char serverID[12];	// 服务端ID
		char seckey[128];	// 密钥
	}NODE;

客户端只有一个结构体，服务端必须要有多个。服务端通过客户端发送的clientID和serverID就能找到对应的密钥。

# 十三、密钥协商处理流程 

1. 客户端生成一个随机数r1, 同时使用openssl中哈希函数对r1进行哈希运算, 得到一个哈希值
2. 将要发送的数据进行编码
3. 发送数据给服务端
4. 服务端收到请求数据之后, 首先进行解码操作
5. 服务端根据客户端ID+服务端ID查询数据库, 校验客户端是否合法,如不合法直接拒绝服务
6. 服务端校验r1消息认证码  
&ensp;&ensp;--首先使用与客户端相同的算法生成哈希值, 然后将这个哈希值与接受到的哈希值做比较;  
&ensp;&ensp;如果不一样, 则拒绝服务; 如果一致则继续后续操作。
7. 服务端也生成随机数r2
8. 服务端将r1和r2进行拼接, 然后使用与客户端相同的哈希算法进行哈希运算, 得到一个哈希值;这个哈希值就当做新的秘钥seckey2;
9. 将秘钥信息写入共享内存和数据库
10. 服务端发送应答信息给客户端; 
11. 客户端等待接收服务端的应答
12. 对接收到的数据进行解码
13. 判断rv的值, 若rv为-1表示生成秘钥失败
14. 如果成功:rv=0;  
&ensp;&ensp;获得服务端发来的随机字符串r2, 将r2和r1进行拼接, 然后进行哈希运算得到一个新的秘钥seckey1;
15. 客户端将秘钥信息写入共享内存; 


# 十四、密钥校验处理流程 
秘钥校验:
1. 客户端将秘钥进行哈希运算, 得到一个哈希值
2. 将哈希值发送给服务端
3. 服务端收到哈希值, 并且自己也生成一个哈希值
4. 将两个哈希值进行比较, 相同则秘钥协商成功, 否则秘钥协商失败。 

**密钥协商与密钥校验可以合并，服务端将生成的密钥进行哈希运算，将得到的哈希值发送给客户端，客户端将服务端发送的s2与s1进行合并，生成密钥，并且进行哈希运算，并与服务端生成的哈希值进行比较，如果一样则密钥协商和密钥校验成功。否则失败。** 

# 十五、密钥注销处理流程 
秘钥注销:
1. 将clientID, serverID, 秘钥ID发送给服务端。
2. 服务端收到请求之后, 将共享内存中的秘钥的状态修改为不可用状态;将数据库中的秘钥的使用状态修改为不可用状态。



**客户端给服务器发送数据的时候用到的数据结构**


	struct RequestMsg
	{
	    int cmdType; 	   //0-秘钥协商 1-秘钥校验 2-秘钥注销 3-秘钥查看
	    char clientId[12]; //客户端ID, 是唯一的, 不同的客户端有不同的ID
	    char authCode[65]; //消息验证码, 对r1进行验证
	    char serverId[12]; //服务端ID
	    char r1[64];       //客户端生成的随机字符串
	};


**服务器给客户端回复的时候用的到结构**

	struct RespondMsg
	{
	    int rv; 		   //0-表示成功 -1-表示失败
	    char clientId[12]; //客户端ID
	    char serverId[12]; //服务端ID 
	    char r2[64]; 	   //秘钥协商: 随机字符串, 秘钥查看: 秘钥  秘钥校验和注销:可不写 
	    int seckeyid; 	   //秘钥ID   
	};

# 十六、秘钥协商系统服务端开发流程

1. 服务器的前期准备工作, 通过调用TcpServer::setListen()函数  
  - 创建共享内存  
  - socket()  
  - setsockopt()  
  - bind()  
  - listen()   

2.   
 
	while(1)
	  {
	  	 //TcpSocket* sock = TcpServer::acceptConn();
	  	 //创建子线程
	  	 pthread_create(&threadid, NULL, working, arg);
	  	 //设置子线程为分离属性
	  	 pthread_detach();
	  }


3. 子线程处理流程:


	void *working(void *arg)
	{
		//读取客户端发来的数据
		char *inData;
		int dataLen;
		TcpSocket::recvMsg(&inData, dataLen);
		
		//解码数据
		CodecFactory *factory = new RequestFactory();
		Codec *pCodec = factory->createCodec();
		RequestMsg *pMsg = (RequestMsg *)pCodec->msgDecode(inData, dataLen);
		
		//判断是哪种服务
		swtch(pMsg->cmdType)
		{
			case RequestCodec::NewOrUpdate:
				secKeyAgree();
				break;
	
			case RequestCodec::Check:
				secKeyCheck();
				break;
			case RequestCodec::Revoke:
				secKeyRevoke();
				break;
			case RequestCodec::View:
				secKeyView();
				break;
	
			default:
				break; 
		}
	
	}

4. 秘钥协商流程分析:  


	int secKeyAgree()
	{
		//判断clientID释放合法
		if(不合法)
		{
			//拒绝服务
			return -1;
		}
		
		//验证随机字符串是否被篡改
		//生成消息认证码
		HMAC(EVP_sha256().....); ---消息认证码为autoCode
		//比较生成的消息认证码和客户端发来的消息认证码是否相同
		if(strcmp(autoCode, pMsg->authCode)!=0)
		{
			//拒绝服务
			return -1;
		}
		
		//生成随机字符串
		getRandString(sizeof(r2), r2);
		
		//组织应答数据给客户端
		struct RespondMsg
		{
		    int rv; 		   //0-表示成功 -1-表示失败
		    char clientId[12]; //客户端ID
		    char serverId[12]; //服务端ID 
		    char r2[64]; 	   //秘钥协商: 随机字符串, 秘钥查看: 秘钥  秘钥校验和注销:可不写 
		    int seckeyid; 	   //秘钥ID   
		};
		rv = 0;
		seckeyid是从数据库中查询获得的, 是唯一的一个值.
		
		//对RespondMsg变量进行编码操作, 生成字符串outData
		sendMsg(outData, dataLen);
		
		//关闭网络连接
		disConnect();
		
		//将客户端的随机字符串和r2进行拼接, 然后调用SHA1()函数生成秘钥
		SHA1();
		
		//将秘钥信息写入共享内存
		class NodeSHMInfo
		{
		public:
			int status;    //0
			int seckeyID;  //是从数据库表中查询获得的
			char clientID[12]; //
			char serverID[12]; 
			char seckey[128];  //就是调用SHA1函数生成的
		};
		
		shmWrite(&node);
		
		//释放资源	
	}

# 十六、守护进程
创建守护进程的步骤:
1. 父进程fork子进程, 然后父进程退出  
  fork();  
  父进程退出的手段: exit(), return(), kill(getpid(), SIGKILL); abort()  raise(SIGKILL);
2. 子进程调用setsid()函数创建一个会话.  
  --注意:调用setsid()函数的进程不能是组长进程.  
3. 改变当前的工作目录--chdir();  
  --不是必须的
4. 修改当前进程的文件掩码--umask(0022);    
  --不是必须的
5. 关闭或者重定向,标准输入 标准输出和标准错误输出  


	  close(STDIN_FILENO);
	  close(STDOUT_FILENO);
	  close(STDERR_FILENO);
	  
	  int fd = open("/dev/null", O_RDWR);
	  dup2(fd, STDIN_FILENO);
	  dup2(fd, STDOUT_FILENO);
	  dup2(fd, STDERR_FILENO);
	  /dev/null是设备文件, 这个文件写不满  
6. 核心操作.

守护进程的特点：

- Linux后台服务进程
- 独立于控制终端
- 周期性的执行某种任务
- 不受用户登入和注销的影响
- 一般采用以d结尾的名字 

**测试端口是否属于监听状态：**

	netstat -anp | grep 端口号