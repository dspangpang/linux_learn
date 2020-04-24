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






/*****************************************共享内存 ***************************************************************************/
//！！！！！！！！！！！！！！！！共享内存的数据可多次读取
//1.查看系统中的共享存储段
   ipcs -m

//2.删除系统中的共享存储段
	ipcrm -m [shmid];

//3.共享内存的创建  
//（1）ftok函数的使用 
key_t ftok(const char *pathname, int proj_id)；
  //第一个参数为 一个存在可读写的文件路径    第二个参数 计划ID 可以随机填写
  
//	成功：返回key_t值（即IPC 键值）
//	失败返回-1


int shmget(key_t key, size_t size, int shmflg);    //共享内存的创建函数
//第二个参数  是 要创建的共享内寸的大小  
//第三个参数shmflg是一组标志，创建一个新的共享内存，将shmflg 设置了IPC_CREAT标志后，共享内存存在就打开。
//而IPC_CREAT | IPC_EXCL则可以创建一个新的，唯一的共享内存，如果共享内存已存在，返回一个错误。一般我们会还或上一个文件权限
//|上创建的权限

//返回值 ；成功返回创建共享内存的ID号 失败返回-1

//将共享内存映射到用户空间中
void *shmat(int shmid, const void *shmaddr, int shmflg);
//第一个参数为 是创建的共享内存的ID号  
//第二个参数是共享内存要映射的地址 填NULL 系统会自动分配
//第二个函数 是标志位 SHM_RDONLY 是设置该共享内存只读  默认是 0 可读写

//返回值是 映射的地址

//将共享内存的映射空间删除

int shmdt(const void *shmaddr);
//成功返回0 失败返回-1

//删除创建的共享内存对象

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
//第一个参数位 共享内存的标识符
//第二个参数是 控制命令 
//IPC_STAT  获取对象属性 
//IPC_SET   设置对象属性
//IPC_RMID   删除创建的共享内存

//第三个参数是 使用与对象属性有关的结构体 并没有涉及




//操作过程 

  key = ftok("a.c","b");                            //每次创建时 要判断是否失败！！！！！！！
  shmid = shmget( key, size,shmflg);
  p = (char*)shmat(shmid, NULL,shmflg);
  fgets(p,size,stdin);   //每写一次相当于对原来共享内存内容的覆盖
  printf("%s",p);
  shmdt(p);
  
  shmctl(shmid,IPC_RMID,NULL);//删除创建的共享内存





/***************************************************消息队列****************************************************************************/

//需要包含的头文件 
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


//消息队列的创建函数 

int msgget(key_t key, int msgflag);

//参数：
//key：某个消息队列的名字，用ftok()产生
//msgflag：有两个选项IPC_CREAT和IPC_EXCL，单独使用IPC_CREAT，如果消息队列不存在则创建之，如果存在则打开返回；单独使用IPC_EXCL是没有意义的；
//两个同时使用，如果消息队列不存在则创建之，如果存在则出错返回。
//返回值：成功返回一个非负整数，即消息队列的标识码，失败返回-1
//////函数使用之后要判断是否成功！！！！！！！





//消息队列的控制函数


int msgctl(int msqid, int cmd, struct msqid_ds *buf);

//msqid：由msgget函数返回的消息队列标识码
//cmd：有三个可选的值，在此我们使用IPC_RMID

//IPC_STAT 把msqid_ds结构中的数据设置为消息队列的当前关联值
//IPC_SET 在进程有足够权限的前提下，把消息队列的当前关联值设置为msqid_ds数据结构中给出的值
//IPC_RMID 删除消息队列
//返回值：
//成功返回0，失败返回-1



//把一条消息添加到消息队列中


int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);

/*
参数：
msgid：由msgget函数返回的消息队列标识码
msgp：指针指向准备发送的消息
msgze：msgp指向的消息的长度（不包括消息类型的long int长整型）
msgflg：默认为0
返回值：成功返回0，失败返回-1

消息结构一方面必须小于系统规定的上限，另一方面必须以一个long int长整型开始，接受者以此来确定消息的类型
*/

struct msgbuf{
	long mtye;
	char mtext[1];
};




//是从一个消息队列接受消息

ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);

/*
参数：与msgsnd相同
返回值：成功返回实际放到接收缓冲区里去的字符个数，失败返回-1
*/




//两个系统命令

ipcs://显示IPC资源
ipcrm://手动删除IPC资源





//使用逻辑  

      key = ftok("./a.c","b")           //创建key
	  msgid = msgget( key, msgflag);    //创建消息队列
	  struct mybuf buf;
	  msgsnd(msqid, (void *)&buf, msgsz,msgflg);
	  msgrcv(msqid,(void *)&buf,msgsz, buf.typ, msgflg);                 //有返回值的函数 都需要接受返回值 并判断是否创建失败
	  msgctl(msqid,IPC_RMID, NULL);       //第三个参数数消息列队的属性常用NULL
	  









   



/*******************************************线程**************************************************************************/

/********************************************************线程的创造***********************************************************/


1.线程的ID
//获取线程的id
pthread_t  = tid;
tid = pthread_selt();          //获取线程的ID    函数不会失败；

//有的系统 的pthread_t 类型是结构体   无法直接打印

2.创造一个新的线程
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);

1.第一个参数是新线程的id，线程创建成功后会回填到 thread 所在的地址
2.第二个参数是设置线程的属性
3.第三个参数是回调的函数，即线程创建成功后要去哪个部分执行代码
4.第四个函数是回调函数的参数如果需要传递很多参数，可以建立一个结构体

成功返回0，失败返回错误码；


//操作方式

void thread_fun(void *argv){                  //线程的回调函数
	return (void*)0;                           //返回值也是 void*
}
int err;
pthread_t  = tid;
 err = pthread_create(*tid,NULL,thread_fun, void *arg);
 //如果出错 就返回！！！！！！！！！
 

//当主线程调用  return 的时候 进程就结束了
//可以使用函数
void pthread_exit(void *retval);   //等所有线程执行完了再结束进程    此参数是一个回填参数 调用函数时自动装填，只需要设置一个同类型的变量

//使用方式 就是
	void *ret;
	pthread_exit(ret);


//线程的回收
  
  ！！自己创建的资源要自己进行回收  例如 malloc   或者 互斥量 条件变量  再任何线程都可以回收 





//线程的连接或者说是线程资源的回收   

 int pthread_join(pthread_t thread, void **retval);
 
 pthread_join使一个线程等待另一个线程结束。

 代码中如果没有pthread_join主线程会很快结束从而使整个进程结束，从而使创建的线程没有机会开始执行就结束了。加入pthread_join后，主线程会一直等待
 直到等待的线程结束自己才结束，使创建的线程有机会执行。      //可以达到阻塞主线程的效果


 第一个参数是线程的id ， 第二个参数是线程结束后的返回时，不需要时可以用NULL 填充
 线程被取消时 回返回 PTHREAD_CANCELED



//线程的取消   

线程可以通过自身执行结束来结束，也可以通过调用pthread_exit()来结束线程的执行。另外，线程甲可以被线程乙被动结束。这个通过调用pthread_cancel()
来达到目的。

int pthread_cancel(pthread_t thread);
函数调用成功返回0。

当然，线程也不是被动的被别人结束。它可以通过设置自身的属性来决定如何结束

线程的被动结束分为两种，一种是异步终结，另外一种是同步终结。

异步终结就是当其他线程调用pthread_cancel的时候，线程就立刻被结束。

而同步终结则不会立刻终结，它会继续运行，直到到达下一个结束点cancellation point当一个线程被按照默认的创建方式创建，那么它的属性是同步终结。


void pthread_testcancel(void);  //在函数中建立取消点





//更改线程的取消状态      可被取消    不可被取消


int pthread_setcancelstate(int state, int *oldstate);
  第二个参数是，上一个状态
 
 PTHREAD_CANCEL_ENABLE     线程可被取消  
PTHREAD_CANCEL_DISABLE     线程不可被取消

//更改线程的取消类型   延时取消  异步取消
int pthread_setcanceltype(int type, int *oldtype);

第二个参数是上一个类型


PTHREAD_CANCEL_DEFERRED   延时取消
PTHREAD_CANCEL_ASYNCHRONOUS  异步取消                     //异步取消 即立即被取消



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
pthread_mutex_lock(&mutex)          //连续两次上锁会造成死锁 ，线程挂起


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



/*****************************************条件变量*********************************************************************************/

//避免因线程逻辑上的无法执行而导致CPU空转


pthread_cond_t     //条件变量 
 pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
//条件变量的初始化
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *cond_attr);     //同上初始化函数

//条件变量使用完后销毁

int pthread_cond_destroy(pthread_cond_t *cond);


//   条件变量的使用要配合互斥量   成对存在

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex); 
//当函数没有收到改条件变量时，会将该互斥量解锁  然后将该线程阻塞
//等到 收到该条件变量的时候，互斥量继续加锁  该线程继续执行

int pthread_cond_timedwait(pthread_cond_t *cond,  pthread_mutex_t  *mutex, const struct timespec *abstime);

//相比较第一个函数这个函数多了一个时间  当时间到了 条件还没有返回就 直接返回   这个 等以后用到了再做研究；




//当条件满足只会  唤醒正在阻塞的线程

int pthread_cond_signal(pthread_cond_t *cond);

int pthread_cond_broadcast(pthread_cond_t *cond);   //第二个函数 可以唤醒所有等待条件的线程  而第一个函数不一定

//条件变量自己创建




//条件变量使用逻辑

    pthread_mutex_lock(&mutex);      //互斥量上锁
	
	if(/*判断是否具备该线程执行的逻辑条件 */){
		
		pthread_cond_wait(&cond,&mutex);       //如果不满足  解锁该互斥量  并阻塞该线程  不占用CPU
	}
	
	pthread_cond_signal(&cond);    //当状态改变时  即满足某个线程的逻辑执行条件时  发送条件变量到线程 
	
	pthread_mutex_unlock(&mutex);    //互斥量解锁
	
	 
	
	
/******************************************************一次性初始化*************************************************************/

//很多初始化进程中只需要进行一次就够了  比如 条件变量初始化 互斥锁初始化 等

//使用库函数的一次性初始化 即使该函数 被调用多次 也只会初始化 一次

int  pthread_once(pthread_once_t  *once_control,  void  (*init_routine)(void));
// 第一个参数是 声明的一次性初始化的变量     仅当为 0 时 参数 才可以调用  参数 为1是会 阻塞  参数为2会立刻返回；
// 第二个参数是 定义的一个没有参数没有返回值的函数

pthread_once_t once = PTHREAD_ONCE_INIT;            //声明一个一次性初始化的变量
//PTHREAD_ONCE_INIT 参数为0 在执行过后 会变成 2   在函数执行中 该参数变为1
void thread_init(){                               //  定义的一个没有参数没有返回值的函数
	
	
}

//操作方式  

//在某个 进程的某个线程中 
   pthread_once(once,thread_init);

/*******************************************线程的属性*****************************************************************************/

//线程属性的初始化和销毁   
pthread_attr_t  attr;
int pthread_attr_init(pthread_attr_t *attr);

int pthread_attr_destroy(pthread_attr_t *attr);






//线程的分离属性
int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);    //设置分离状态属性

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);	//获取分离状态属性

//PTHREAD_CREATE_DETACHED     //分离状态
//PTHREAD_CREATE_JOINABLE      /*不可分离状态 */   int detachstate的值；

若线程设置为可销毁 
则  使用 int pthread_detach(pthread_t thread); 使线程分离  使线程分离进程 独自运行

成功返回 0  失败返回错误码

//操作方式

	1.初始化变量        pthread_attr_t  attr;	int detachstate          
	2.调用初始化函数    pthread_attr_init(&attr);                  //与销毁配套出现
	3.设置属性 			pthread_attr_setdetachstate(&attr, detachstate);
	4.创建线程
	



//线程的栈的大小和地址

 进程的虚拟空间是一定的，通常一个进程只有一个线程栈，当线程多了之后，线程栈的数量有可能超过进程的虚拟空间，所以要改变创建线程的线程栈


//===============================修改线程栈属性
int pthread_attr_setstack(pthread_attr_t *attr,void *stackaddr, size_t stacksize);


//获得线程栈属性
int pthread_attr_getstack(const pthread_attr_t *attr, void **stackaddr, size_t *stacksize);


//修改线程栈的大小                不能小于   PTHREAD_STACK_MIN   
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

//获取线程栈的大小     
int pthread_attr_getstacksize(const pthread_attr_t *attr, size_t *stacksize);


//===========栈尾警戒区，防止空间溢出，在栈尾拓展一些空间

//默认是PAGESIZE 个字节   设置0将不会设置缓冲区

int pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);


int pthread_attr_getguardsize(const pthread_attr_t *attr, size_t *guardsize);

//使用时需要判断是否支持线程栈属性的修改

#ifdef _POSIX_THREAD_ATTR_STACKSIZE

#endif


//=====================================线程的同步属性


//互斥量的属性
       
int pthread_mutexattr_init(pthread_mutexattr_t *attr);          //互斥量属性的初始化

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);        //互斥量属性的销毁


//互斥量的共享属性     //读写锁  //条件变量 //同理 都有进程共享属性

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr,int *pshared);     //获取互斥量的共享属性



int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr,int pshared);             //设置互斥量的共享属性


PTHREAD_PROCESS_PRIVATE   //仅在一个进程内多个线程使用

PTHREAD_PROCESS_SHARED    // 多个进程可共享互斥量

//设置时判断 系统支持 

#ifdef _POSIX_PTHREAD_PROCESS_SHARED
#endif




//互斥量的类型属性


int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);

int pthread_mutexattr_gettype(const pthread_mutexattr_t *attr, int *kind);

/* 
                             没有解锁再次加锁            不占用时解锁          解锁后解锁
PHTREAD_MUTEX_NORMAL      |       死锁			  |        未定义          |     未定义
PTHREAD_MUTEX_ERRORCHECK  |		返回错误		  |       返回错误         |	返回错误
PTHREAD_MUTEX_RECURSIVE	  |      允许（递归）     |       返回错误         |	返回错误
PTHREAD_MUTEX_DEFAULT     |		 未定义           |        未定义          |     未定义

*/




/******************线程的私有数据*/


//私有数据在每个线程中虽然叫同样的名字 但是却互不影响  

//在使用是由数据之前我们需要创建一个 键 来获得对私有数据的访问权限 

pthread_key_t         //声明这个变量

int pthread_key_create(pthread_key_t *key, void (*destr_function) (void *));      //键的创建函数

功能：

创建一个类型为 pthread_key_t 类型的私有数据变量( key )。

參数：

key：在分配( malloc )线程私有数据之前，须要创建和线程私有数据相关联的键( key )，这个键的功能是获得对线程私有数据的訪问权。

destructor：清理函数名字( 如：fun )。当线程退出时，假设线程私有数据地址不是非 NULL，此函数会自己主动被调用。该函数指针能够设成 NULL ，
这样系统将调用默认的清理函数。

回调函数其定义例如以下：

void fun(void *arg)

{

// arg 为 key 值,在此处清空数据

}


返回值：

成功：0

失败：非 0


不论哪个线程调用 pthread_key_create()，所创建的 key 都是全部线程可訪问，但各个线程可依据自己的须要往 key 中填入不同的值，
相当于提供了一个同名不同值的变量。



//键的销毁函数 

int pthread_key_delete(pthread_key_t key);       //  键的销毁 并不销毁与他相关联的私有数据   数据应该提前销毁！！
功能：

注销线程私有数据。

这个函数并不会检查当前是否有线程正使用线程私有数据( key )，也不会调用清理函数 destructor() ，
而仅仅是将线程私有数据( key )释放以供下一次调用 pthread_key_create() 使用。
參数：

key：待注销的私有数据。

返回值：


成功：0

失败：非 0









//键和私有数据的关联 


int pthread_setspecific(pthread_key_t key, const void *pointer);

功能：

设置线程私有数据( key ) 和 value 关联，注意，是 value 的值（不是所指的内容）和 key 相关联。

參数：

key：线程私有数据。

value：和 key 相关联的指针。

返回值：

成功：0

失败：非 0




//获取私有数据的地址，如果没有数据和KEY关联则返回0

void * pthread_getspecific(pthread_key_t key);

功能：

读取线程私有数据( key )所关联的值。

參数：

key：线程私有数据。

返回值：

成功：线程私有数据( key )所关联的值的地址。

失败：NULL





////*****************在线程中安全使用fork
  在创造进程时只会，复制父线程的内容，有可能无法加锁或解锁从父线程拷贝过来的 互斥量等   所以要使用 pthread_atfork函数
    
	  int pthread_atfork(void (*prepare)(void),void (*parent)(void),  void (*child)(void));
//里面是三个回调函数
	
	第一个回调函数，会在使用fork之前调用
	第二个回调函数，父进程返回前调用
	第三个回调函数，子进程进程返回前调用

	针对互斥量的情况

void prepare(void){
	 pthread_mutex_lock(&mutex);    // fork 调用之前先上所 上锁 若其他线程中已经上过锁，则在解锁前一直阻塞
}


void parent(void){
	 pthread_mutex_unlock(&mutex);   //把prepare 上过的锁解开  这样新的进程中就不再有上锁的
}

void child (void){
	pthread_mutex_unlock(&mutex);   //把prepare 上过的锁解开  这样新的进程中就不再有上锁的
}



/****************************************************************网络编程*******************************************************************/

//大端数据  ；把高位数据 存在 低地址   即数据尾部为高地址

//小端数据  ：把低位数据 存在 低地址   即数据尾部为低地址


//UDP协议
#include<apra/inet.h>
//32位 字节序转换     主机——————————>网络
uint32_t htonl( uint32_t hostint 32);        //主机——————————>网络 长整型字节序转换

//字节序转换     网络——————————>主机
uint32_t ntohl( uint32_t hostint 32);        // 网络——————————>主机 长整型字节序转换
//函数默认    网路的数据是大端，这个函数会判断主机的存储方式，然后判断是否需要转码
//同样的也有 16 位


int inet_pton(int af, const char *src, void *dst);
//这个函数转换字符串到网络地址，
//第一个参数af是地址簇，
//第二个参数*src是来源地址，
//第三个参数* dst接收转换后的数据。
//inet_pton 是inet_addr的扩展，支持的多地址族有下列：

/*af = AF_INET    //也是 IP V4
src为指向字符型的地址，即ASCII的地址的首地址（ddd.ddd.ddd.ddd格式的），函数将该地址转换为in_addr的结构体，并复制在*dst中。
af = AF_INET6
src为指向IPV6的地址，函数将该地址转换为in6_addr的结构体，并复制在*dst中*/

//成功返回 1 失败返回其他

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size); //最后一个是点分十进制的缓存区大小；

//同上   将网络中的地址转换为 点分十进制
//最后一个参数有宏定义
#define INET_ADDRSTRLEN 16 //for IPv4
#define INET6_ADDRSTRLEN 46 //for IPv6   //冒分16进制

//成功 返回字符串首地址        失败返回NULL




















