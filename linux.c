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





  










