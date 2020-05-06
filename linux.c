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



//信号集知识点 

1.信号集
  每个进程都有一个信号屏蔽字，它规定了当前要阻塞递送到该进程的信号集。对于每种可能的信号，该屏蔽字中都有一bit位与之对应。信号数可能会超过一个整型数所包含的二进制位数，因此POSIX.1定义了一个新数据类型sigset_t，它包括一个信号集
  int sigempytset(sigset_t *set)  初始化set指向的信号集，清除其中的所有信号
  int sigfillset(sigset_t *set)   初始化set指向的信号集，其中包含所有信号
  int sigaddset(sigset_t *set,int signo);//向set中添加一个signo信号
  int sigdelset(sigset_t *set,int signo);
  以上均成功返回0 出错返回-1
 
  测试某个信号是否被设置在信号集中
  int sigismember(sigset_t *set,int signo) 为真返回1 为假返回0   出错返回-1

2.sigprocmask ()   函数检测或更改进程的屏蔽字
  int sigprocmask(int flag,const sigset_t *set,sigset_t *oldset);
  若set非空指针，则按照flag方式进行设置新的信号屏蔽字
SIG_BLOCK : 取当前进程的屏蔽字与set的并集   <不影响其它基础上 设置屏蔽>
SIG_UNBLOCK：取当前进程的屏蔽字与set的补集 的交集  <不影响其它基础上  解除屏蔽>
SIG_SETMASK:直接设置

3.int sigsuspend(sigset_t *mask)
  把进程信号屏蔽字设成为mask并原子的阻塞等侍一个某信号从捕捉函数返回

4.int sigaction(int how,const sigset_t *act,sigset_t *oldact );
   改变一个信号的行为



/***********************************************信号灯***********************************************************************/
/////POSIX 信号量

POSIX信号量接口，意在解决XSI信号量接口的几个不足之处：

POSIX信号量接口相比于XSI信号量接口，允许更高性能的实现。
POSIX信号量接口简单易用：没有信号量集，其中一些接口模仿了我们熟悉的文件系统操作。
POSIX信号量删除时的处理更加合理。XSI信号量被删除后，使用该信号量标识符的操作将会出错返回，并将errno设置为EIDRM。而对于POSIX信号量，操作可以继续正常执行，直到对该信号量的最后一个引用被释放。
POSIX信号量有两种形式可供选用：有名和无名。它们的区别在于，如何被创建和销毁，其他方面则完全相同。无名信号量只存在于内存中，并且规定能够访问该内存的进程才能够使用该内存中的信号量。这就意味着，无名信号量只能被这样两种线程使用：（1）来自同一进程的各个线程（2）来自不同进程的各个线程，但是这些进程映射了相同的内存范围到自己的地址空间。相反，有名信号量则是通过名字访问，因此，来自于任何进程的线程，只要知道该有名信号量的名字都可以访问。

//调用sem_open函数可以创建一个新的有名信号量，或打开一个现存的有名信号量。

#include <semaphore.h>
sem_t *sem_open(const char *name, int oflag, ... /* mode_t mode, unsigned int value */ );

返回值：若成功则返回指向信号量的指针，若出错则返回SEM_FAILED
如果使用一个现存的有名信号量，我们只需指定两个参数：信号量名和oflag（oflag取0）。
把oflag设置为O_CREAT标志时，如果指定的信号量不存在则新建一个有名信号量；如果指定的信号量已经存在，那么打开使用，无其他额外操作发生。
如果我们指定O_CREAT标志，那么就需要提供另外两个参数：mode和value
mode用来指定谁可以访问该信号量。它可以取打开文件时所用的权限位的取值（参考http://www.cnblogs.com/nufangrensheng/p/3502097.html中表4-5）。最终赋予信号量的访问权限，是被调用者文件创建屏蔽字所修改过的（http://www.cnblogs.com/nufangrensheng/p/3502328.html）。然而，注意通常只有读写权限对我们有用，但是接口不允许在我们打开一个现存的信号量时指定打开模式（mode）。实现通常以读写打开信号量。

value参数用来指定信号量的初始值。它可取值为：0-SEM_VALUE_MAX。

如果我们想要确保我们在创建一个新的信号量，可以把oflag参数设置为：O_CREAT|O_EXCL。
如果信号量已经存在的话，这会导致sem_open调用失败。

为了提高移植性，我们在选择信号量名字的时候，必须遵循一定的约定：

名字的首字符必须是斜杠（/）。
除首字符外，名字中不能再包含其他斜杠（/）。
名字的最长长度由实现定义，不应超过_POSIX_NAME_MAX个字符。
sem_open函数返回一个信号量指针，该指针可供其他对该信号量进行操作的函数使用。

//调用sem_close函数释放与信号量相关的资源。
#include <semaphore.h>
int sem_close(sem_t *sem);

返回值：若成功则返回0，出错返回-1
如果进程还没有调用sem_close就已经退出，那么内核会自动关闭该进程打开的所有信号量。
注意，这并不会影响信号量值的状态——例如，如果我们增加了信号量的值，我们退出后这个值不会改变。
同样，如果我们调用了sem_close，信号量值也不会受到影响。POSIX信号量机制中并没有如同XSI信号量中的SEM_UNDO标志。

//调用sem_unlink函数来销毁一个有名信号量。

#include <semaphore.h>
int sem_unlink(const char *name);

返回值：若成功则返回0，出错则返回-1
sem_unlink函数移除信号量的名字。如果当前没有打开的对该信号量的引用，那么就销毁它。否则，销毁被推迟到最后一个打开的引用被关闭。

与XSI信号量不同，我们只能对POSIX信号量的值进行加1或减1。
对信号量值减1，就类似于对一个二值信号量加锁或请求一个与计数信号量相关的资源。

注意，POSIX信号量并没有区分信号量类型。
使用二值信号量还是计数信号量，取决于我们如果对信号进行初始化和使用。
如果信号量值只能取0和1，那么它就是一个二值信号量。当一个二值信号量值为1，我们则说它未加锁；若它的值为0，则说它已加锁。

调用sem_wait或sem_trywait函数，请求一个信号量（对信号量值执行减1操作）。

#include <semaphore.h>
int sem_trywait(sem_t *sem);
int sem_wait(sem_t *sem);
两个函数返回值：若成功则返回0，出错则返回-1

如果信号量计数为0，这时如果调用sem_wait函数，将会阻塞。直到成功对信号量计数减1或被一个信号中断，sem_wait函数才会返回。
我们可以使用sem_trywait函数以避免阻塞。
当我们调用sem_trywait函数时，如果信号量计数为0，sem_trywait会返回-1 ，并将errno设置为EAGAIN。

//第三种方法是可以阻塞一段有限的时间，这时我们使用sem_timedwait函数。

#include <semaphore.h>
#include <time.h>

int sem_timedwait(sem_t *restrict sem, const struct timespec *restrict tsptr);

返回值：若成功则返回0，出错则返回-1
tsptr参数指定了希望等待的绝对时间。
如果信号量可以被立即减1，那么超时也无所谓，即使你指定了一个已经过去的时间，试图对信号量减1的操作也会成功。
如果直到超时，还不能对信号量计数减1，那么sem_timedwait函数将会返回-1 ，并将errno设置为ETIMEDOUT。

//调用sem_post函数对信号量值加1。这类似于对一个二值信号量解锁或释放一个与计数信号量有关的资源。

#include <semaphore.h>
int sem_post(sem_t *sem);
返回值：若成功则返回0，出错则返回-1
当我们调用sem_post的时，如果此时有因为调用sem_wait或sem_timedwait而阻塞的进程，那么该进程将被唤醒，
并且刚刚被sem_post加1的信号量计数紧接着又被sem_wait或sem_timedwait减1。

如果我们想要在一个单一进程内使用POSIX信号量，那么使用无名信号量会更加简单。
无名信号量只是创建和销毁有所改变，其他完全和有名信号量一样。

//我们调用sem_init函数创建一个无名信号量。

#include <semaphore.h>
int sem_init(sem_t *sem, int pshared, unsigned int value);
返回值：若成功则返回0，出错返回-1
pshared参数指示我们是否要在多进程之间使用该无名信号量。如果要在多个进程之间使用，则将pshared设置为非0值。value参数指定信号量的初始值。

另外，我们需要声明一个sem_t类型的变量，并把它的地址传给sem_init，以便对该变量进行初始化。如果我们要在两个进程之间使用该无名信号量，我们需要确保sem参数指向这两个进程共享的内存范围内。

//我们可以调用sem_destroy函数来销毁用完的无名信号量。

#include <semaphore.h>
int sem_destroy(sem_t *sem);
返回值：若成功则返回0，出错则返回-1
调用sem_destroy后我们将不能再以sem为参数调用任何信号量函数，除非我们再次使用sem_init对sem进行初始化。

//我们可以调用sem_getvalue函数来获取信号量值。

#include <semaphore.h>
int sem_getvalue(sem_t *sem, int *restrict valp);
返回值：若成功则返回0，出错则返回-1
如果sem_getvalue执行成功，信号量的值将存入valp指向的整型变量中。但是，需要小心，我们刚读出来的信号量值可能会改变（因为我们随时可能会使用该信号量值）。如果不采取额外的同步机制的话，sem_getvalue函数仅仅用来调试。



//////////////////////////////////////////////////
1）int semget(key_t key, int nsems, int semflg) 

参数key是一个键值，由ftok获得，唯一标识一个信号灯集，用法与msgget()中的key相同；
参数nsems指定打开或者新创建的信号灯集中将包含信号灯的数目；
semflg参数是一些标志位。
参数key和semflg的取值，以及何时打开已有信号灯集或者创建一个新的信号灯集与msgget()中的对应部分相同，不再祥述。 

该调用返回与健值key相对应的信号灯集描述字。 
调用返回：成功返回信号灯集描述字，否则返回-1。 

注：如果key所代表的信号灯已经存在，且semget指定了IPC_CREAT|IPC_EXCL标志，那么即使参数nsems与原来信号灯的数目不等，返回的也是EEXIST错误；
如果semget只指定了IPC_CREAT标志，那么参数nsems必须与原来的值一致，在后面程序实例中还要进一步说明。




2）int semop(int semid, struct sembuf *sops, unsigned nsops); 
semid是信号灯集ID，sops指向数组的每一个sembuf结构都刻画一个在特定信号灯上的操作。nsops为要操作的信号灯的个数。 
sembuf结构如下：

struct sembuf {
	unsigned short  	sem_num;		/* 对第几个信号量 */
	short			    sem_op;		/* semaphore operation */
	short			    sem_flg;		/* operation flags */  
};
sem_num对应信号集中的信号灯，0对应第一个信号灯。


sem_flg可取IPC_NOWAIT以及SEM_UNDO两个标志 或者 0 阻塞方式。
如果设置了SEM_UNDO标志，那么在进程结束时，相应的操作将被取消，这是比较重要的一个标志位。
如果设置了该标志位，那么在进程没有释放共享资源就退出时，内核将代为释放。
如果为一个信号灯设置了该标志，内核都要分配一个sem_undo结构来记录它，为的是确保以后资源能够安全释放。
事实上，如果进程退出了，那么它所占用就释放了，但信号灯值却没有改变，此时，信号灯值反映的已经不是资源占有的实际情况，
在这种情况下，问题的解决就靠内核来完成。这有点像僵尸进程，进程虽然退出了，资源也都释放了，但内核进程表中仍然有它的记录，
此时就需要父进程调用waitpid来解决问题了。 


sem_op的值大于0，等于0以及小于0确定了对sem_num指定的信号灯进行的三种操作。

sem_op = 0;   等待 直到信号量变成 0
sem_op = 1;   释放资源 	 V操作   加一操作
sem_op = -1;  分配资源   P操作   减一操作

这里需要强调的是semop同时操作多个信号灯，在实际应用中，对应多种资源的申请或释放。semop保证操作的原子性，这一点尤为重要。
尤其对于多种资源的申请来说，要么一次性获得所有资源，要么放弃申请，要么在不占有任何资源情况下继续等待，
这样，一方面避免了资源的浪费；另一方面，避免了进程之间由于申请共享资源造成死锁。 

也许从实际含义上更好理解这些操作：信号灯的当前值记录相应资源目前可用数目；

sem_op>0对应相应进程要释放sem_op数目的共享资源；
sem_op=0可以用于对共享资源是否已用完的测试；
sem_op<0相当于进程要申请-sem_op个共享资源。再联想操作的原子性，更不难理解该系统调用何时正常返回，何时睡眠等待。 

调用返回：成功返回0，否则返回-1。



3) int semctl(int semid，int semnum，int cmd，union semun arg) 
该系统调用实现对信号灯的各种控制操作，
参数semid指定信号灯集，
参数cmd指定具体的操作类型；
参数semnum指定对哪个信号灯操作，只对几个特殊的cmd操作有意义；arg用于设置或返回信号灯信息。 
该系统调用详细信息请参见其手册页，这里只给出参数cmd所能指定的操作。
	
 
/*联合体变量*/
union semun
{
 int val; //信号量初始值                   
 struct semid_ds *buf;        
 unsigned short int *array;  
 struct seminfo *__buf;      
}; 




IPC_STAT	 获取信号灯信息，信息由arg.buf返回；
IPC_SET	     设置信号灯信息，待设置信息保存在arg.buf中（在manpage中给出了可以设置哪些信息）；
GETALL	     返回所有信号灯的值，结果保存在arg.array中，参数sennum被忽略；
GETNCNT	     返回等待semnum所代表信号灯的值增加的进程数，相当于目前有多少进程在等待semnum代表的信号灯所代表的共享资源；
GETPID	     返回最后一个对semnum所代表信号灯执行semop操作的进程ID；
GETVAL	     返回semnum所代表信号灯的值；
GETZCNT	     返回等待semnum所代表信号灯的值变成0的进程数；
SETALL	     通过arg.array更新所有信号灯的值；同时，更新与本信号集相关的semid_ds结构的sem_ctime成员；
SETVAL	     设置semnum所代表信号灯的值为arg.val；

调用返回：调用失败返回-1，成功返回与cmd相关：

Cmd	      return value
GETNCNT	    Semncnt
GETPID	    Sempid
GETVAL	    Semval
GETZCNT	    Semzcnt
回页首

//信号灯的限制
1、 一次系统调用semop可同时操作的信号灯数目SEMOPM，semop中的参数nsops如果超过了这个数目，将返回E2BIG错误。SEMOPM的大小特定与系统，redhat 8.0为32。

2、 信号灯的最大数目：SEMVMX，当设置信号灯值超过这个限制时，会返回ERANGE错误。在redhat 8.0中该值为32767。

3、 系统范围内信号灯集的最大数目SEMMNI以及系统范围内信号灯的最大数目SEMMNS。超过这两个限制将返回ENOSPC错误。redhat 8.0中该值为32000。

4、 每个信号灯集中的最大信号灯数目SEMMSL，redhat 8.0中为250。 SEMOPM以及SEMVMX是使用semop调用时应该注意的；SEMMNI以及SEMMNS是调用semget时应该注意的。SEMVMX同时也是semctl调用应该注意的。




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
uint32_t ntohl( uint32_t hostint 32);        // 网络——————————>主机 长整型字节序转换    //ntohs  十六位短整型
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


////////////创建套接字*******socket

int socket (int family , int type , int protocol)

/*功能*/

      创建一个用于网络通信的套接字（描述符）
/* 参数*/

	   family  :协议族(AF_INET,AF_INET6 )等
	   type :  套接字类(SOCK_STREAM,SOCK_DGRAM,SOCK_TAW等)
	   protocol : 协议类别 (0,IPPROTO_TCP,IPPROTO_UDP 等 ) //0 自动补齐
	   
int sendto(int s, const void * msg, int len, unsigned int flags, const struct sockaddr * to, int tolen);


函数说明：sendto() 用来将数据由指定的socket 传给对方主机.
参数s 为已建好连线的socket, 如果利用UDP协议则不需经过连线操作.
参数msg 指向欲连线的数据内容, 
参数flags 一般设0, 详细描述请参考send(). 
参数to 用来指定欲传送的网络地址, 结构sockaddr 请参考bind(). 
参数tolen 为sockaddr 的结果长度.

返回值：成功则返回实际传送出去的字符数, 失败返回－1, 错误原因存于errno 中.

错误代码：
1、EBADF 参数s 非法的socket 处理代码.
2、EFAULT 参数中有一指针指向无法存取的内存空间.
3、WNOTSOCK canshu s 为一文件描述词, 非socket.
4、EINTR 被信号所中断.
5、EAGAIN 此动作会令进程阻断, 但参数s 的soket 为补课阻断的.
6、ENOBUFS 系统的缓冲内存不足.
7、EINVAL 传给系统调用的参数不正确.

//地址结构 
struct sockaddr{
	sa_family_t     sin_family;  //2字节
	char            sa_data[14]  //14字节
};     //和socketaddr_in 大小相同 所以可传入

struct in_addr{
	in_addr_t  s_addr;
}


struct socketaddr_in{
	sa_family_t     sin_family;  //2字节
	in_port_t       sin_port;    //2字节    //使用的时候应变成网络端数据
	struct in_addr  sin_addr;    //4字节
	char            sin_zero[8]  //8字节
};                                                       //使用时 类型强转一下！！


bzero(地址，大小); //清空函数









