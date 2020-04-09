#include <fcntl.h> 
#include <unistd.h>
//进程间通信：

/****************************创建子进程************************************************************************/
pid_t pid_err;
pid_t pid_err = fork();
if(pid_err<0)
	printf("create process failur\n")            //返回值 小于0 创建失败      如有需要可以 return 结束进程

if(pid_err>0){
	printf("This is father process \n")		    //返回值大于0 即返回值为 子进程的 pid号  为父进程
}

if(pid_err==0){
    printf("This is child process \n")				//返回值等于0，即为子进程  通过 pid的不同 执行不同进程的不同命令
}


/******************************无名管道的用法************************************************************************/



/*单独构成一种独立的文件系统：管道对于管道两端的进程而言，就是一个文件，但它不是普通的文件，它不属于某种文件系统，而是自立门户，
单独构成一种文件系统，并且只存在与内存中。*/


//管道是半双工的 ，一管道只能  一边进一边出。要想实现两个进程间的像互通信  需要两个管道;  类似于队列

int pipe(int pipefd[2]);  //pipe 函数
int ret;
//使用方法            先创建管道 再创建子进程

int pipefd[2];       //pipe是一种特殊的文件   就需要创建他的文件描述符

ret = pipe(pipefd);     //成功返回0 失败返回 -1，需要判断是否创建成功；

if(ret<0)
	printf("create pipe failure\n");     //注意 printf 为行缓存 ,如果创建失败 ，可以 return 结束进程

char write_buffer[] = "hello linux";
char read_buffer[128] = {0};               // 注意 如果想要给字符串赋值  只能使用  memset   和  memcpy  的  字符串操作函数

write(fd[1],write_buffer,sizeof(write_buffer));
read(fd[0],read_buffer,128);                       //向fd[1]中写入数据 ，从 fd[0]中读出数据；读出相当于出队 数据消失



/*管道提供了简单的流控制机制，进程试图读空管道时，进程将阻塞。同样，管道已经满时，进程再试图向管道写入数据，进程将阻塞*/

//！！只能再亲缘进程中使用










/*******************************************有名管道******************************************************************************/

//有名管道的创建函数 
int mkfifo(const char *pathname, mode_t mode);   //第一个参数为 创建管道的文件的路径    ，第二个参数是文件的读写权限  回收umask的影响

//写端操作
	int fd;
	
	char buf[]  = "yyyyyyy";
	
	mkfifo(FIFO_NAME, S_IFIFO|0666);
	
	fd = open(FIFO_NAME, O_WRONLY);
	
	write(fd, buf, strlen(buf)+1);
	
	close(fd);
//读端操作
	
	int fd;
	char buf[BUF_SIZE];

	fd = open(FIFO_NAME, O_RDONLY);
				    
    read(fd, buf, BUF_SIZE);

    printf("%s\n", buf);

    close(fd);
//注意！！调用open打开有名管道的进程可能会被阻塞。但如果同时用读写方式（O_RDWR）打开，则一定不会导致阻塞；如果以只读方式
//（O_RDONLY）打开，则调用open函数的进程将会被阻塞直到有写方式打开管道；同样以写方式（O_WRONLY）打开也会阻塞直到有读方式打开管道

	






/******************************************信号通信***********************************************************************/
#include <signal.h>

/*  1）信号是在软件层次上对中断机制的一种模拟，是一种异步通信方式

　　2）信号可以直接进行用户空间进程和内核进程之间的交互，内核进程也可以利用它来通知用户空间进程发生了哪些系统事件。

　　3）如果该进程当前并未处于执行态，则该信号就由内核保存起来，直到该进程恢复执行再传递给它；如果一个信号被进程设置为阻塞，
		则该信号的传递被延迟，直到其阻塞被 取消时才被传递给进程。



 / *1）忽略信号：对信号不做任何处理，但是有两个信号不能忽略：即SIGKILL及SIGSTOP。

　　2）捕捉信号：定义信号处理函数，当信号发生时，执行相应的处理函数。

　　3）执行缺省操作：Linux对每种信号都规定了默认操作                  */




//信号发送函数 

int kill(pid_t pid, int sig);     //返回值：成功 0；失败 -1


// 向自己发送一个信号
int raise(int sig);			                      //sig：要发送的信号



//在自己进程中设置一个定时器
unsigned int alarm(unsigned int seconds)；             //参数：seconds：定时时间，单位为秒
														//返回值：如果调用此alarm()前，进程中已经设置了闹钟时间
														//，则返回上一个闹钟时间的剩余时间，否则返回0。
								//注意：一个进程只能有一个闹钟时间。如果在调用alarm时已设置过闹钟时间，则之前的闹钟时间被新值所代替


//功能：用于将调用进程挂起直到收到信号为止。
int pause(void);



//！！！！！signal函数

typedef void (*sighandler_t)(int);    //定义 一种 void..(int)型的函数指针  typedef 为 sighandler_t；

sighandler_t signal(int signum, sighandler_t handler);      //signal 函数的定义      第一个参数是 要改变的信号     第二个是  要执行的函数
															//第二个参数  可是是 SIG_ING  忽略这个函数
															//                   SIG_DFL  按默认 方式执行
void signal_handler_fun(int signum) {
                                                               //声明一个函数  用于signal 函数的调用 ，参数是   信号的num

}

//signal    成功反会 该信号上一次执行的函数的指针      失败返回错误码；

//The signals SIGKILL and SIGSTOP cannot be caught or ignored.


//每个signal   每个信号只会 记住 最后一次的 修改  

/*******************************************线程**************************************************************************/









/***********************互斥量*****************************************************************************************/

//加锁
int pthread_mutex_lock(pthread_mutex_t *mutex)  //成功返回 0 失败返回错误码 如果互斥量已经被锁住 ， 线程将会阻塞

int pthread_mutex_trylock(pthread_mutex_t *mutex)  //成功返回 0 失败返回错误码 如果互斥量已经被锁住 ， 线程！不会阻塞

//解锁

int pthread_mutex_unlock(pthread_mutex_t *mutex)  //成功返回 0 失败返回错误码

//////////*******************************/
//操作过程
pthread_mutex_t mutex;   //可设置为一全局变量
//互斥量初始化
pthread_mutex_init (&mutex,NULL)  //前一个变量为 声明的互斥量的地址   ，后一个量为 互斥量属性 ，不设置可设为NULL

//加锁
pthread_mutex_lock(&mutex)


//中间进行数据读取操作


//解锁 
pthread_mutex_unlock(&mutex)




////****************************************读写锁**************************************************************************/



/*读锁又称为（共享锁）如果进程1对一个数据上了读锁，进程1只能读，其他进程也只能对这个数据上读锁；写锁又叫排他锁，
如果进程1给数据上写锁，则进程1可以对该数据进行读写，而其他的进程不能对该数据操作。*/

//成功返回 0  失败返回错误码



//读写锁的初始化函数
int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,const pthread_rwlockattr_t *restrict attr);  //第一个参数为声明的读写锁   
																								//第二个是读写锁的属性						
//使用完的读写锁需要销毁
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
  

//解锁函数
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);                 


//上读锁
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);         // 上锁失败会堵塞

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);       //失败不会堵塞

//上 写锁
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);          // 上锁失败会堵塞

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);         //失败不会堵塞










