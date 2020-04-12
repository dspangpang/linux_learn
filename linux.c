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








