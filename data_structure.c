#include"stdio.h"
#include "stdlib.h"
#define ERROR 0
#define OVERFLOW 0
#define OK 1
#define LIST_INIT_SIZE 10
#define LISTINCREMENY 10
typedef int Status;
typedef int ElemType;
typedef struct SqList{     		//顺序线性表 以整形数据举例
	int length;
	ElemType data[LIST_INIT_SIZE];
}SqList;
int i;
SqList L;
/********************************************顺序线性表*************************************************************************************/
Status Init_Sq_line(SqList *L){  					 //顺序线性表键入数据初始化
	printf("整形顺序线性表建立成功，请键入数据\n");
	for(i=0;i<LIST_INIT_SIZE;i++){
		printf("input: ");
		if(scanf("%d",&(L->data[i]))!=EOF){
			L->length = i+1;
			printf("数据输入成功，当前位置%d \n",L->length);
		}
		else{
			i--;
			printf("数据录入失败，请重试\n");
		}
		
	}
	printf("顺序线性表数据录入完成\n");
	return OK;
}



Status Sq_GetElem_line(SqList *L,int i,ElemType e){    //从顺序线性表中获取数据

	if(L->length == 0 || i<1 || i>L->length)            //线性表中没有数据  查询数据小于1  插叙数据大于总长度 报错
		return ERROR;
	e = L->data[i-1];
	return OK;
}  


Status Sq_Inset_line(SqList *L,int i,ElemType e){
	int k;
	if(L->length == LIST_INIT_SIZE)            //超出设置容量
		return ERROR;
	
	if(i > (L->length)+1 || i<1){                  //i不在范围 或者说 i 的插入无法使原有顺数线性表连续
		
		return ERROR;
	}
	if(i <= L->length){						   //当插入的i不在表尾部 
		for(k = (L->length)-1;k>=i-1;k--){
			L->data[k+1]=L->data[k];
		}
	}
	L->data[i-1]=e;
	L->length++;
	return OK;
}



Status Sq_Dele_line(SqList *L,int i){
	int k;
	if(i>L->length || i<1){                  //i不在范围 
		
		return ERROR;
	}
	if(i<=L->length){						   //当删除的i不在表尾部 
		for(k=i-1;k<=L->length-2;k++){
			L->data[k]=L->data[k+1];
		}
	}
	L->data[L->length-1]=0;
	L->length--;
	return OK;
}

/*********************************************************链式线性表***********************************************************************/
typedef struct Node{           //链表结构体

	ElemType data;
	struct Node *next;
}Node;


typedef struct Node* Linklist;           //链表结构体


Status Sq_GetElem_link(Linklist* L,int i,ElemType e){   //从链式线性表中获取数据
	int j;
	j = 1;
	Linklist p;
	p=(*L)->next;
	while(p && j < i){
		p = p->next;
		j++;
	}
	if( !p || j>i){
		return ERROR;
	}
	e = p->data;
	return OK;
}


Status Sq_InsertElem_link(Linklist* L,int i,ElemType e){   //从链式线性表中获取数据
	
	int j;
	j = 1;
	Linklist p;
	Linklist s;
	p= (*L);
	while(p && j < i){
		p = p->next;
		j++;
	}
	if( !p || j>i){
		return ERROR;
	}
	s = (Linklist )malloc(sizeof(Node));
	s->data = e;
	s->next = p->next;
	p->next = s;
	return OK;
}

//头插法***************始终让新的节点在第一个位置*********************************************/

Status Sq_Create_Linklist_head(Linklist *L，int n){        //L是头节点的地址 即头指针;
	Linklist p;
	int i;
	srand(time(0));      //初始化随机数种子

	*L = (Linklist)malloc(sizeof(Node));
	(*L)->next = NULL;

	for(i = 0; i<n ;i++ ){
		
		p=(Linklist)malloc(sizeof(Node));
		p->data = rand() %100 +1;  				//数据取 1~100
		p->next = (*L)->next;
		(*L)->next = p;
	}


}   

/***********尾插法************/

Status Sq_Create_Linklist_tail(Linklist *L，int n){        //L是头节点的地址
	Linklist p,r;
	int i;
	srand(time(0));      //初始化随机数种子

	*L = (Linklist)malloc(sizeof(Node));
	r = *L;                                          //r 只是一个标志地址 指向最后的 节点;

	for(i = 0; i<n ;i++ ){
		
		p=(Linklist)malloc(sizeof(Node));
		p->data = rand() %100 +1;  				//数据取 1~100
		r->next = p;
		r = p;
	}
	r->next = NULL;


}   


Status linklist_Dele(Linklist *L){                    //单链表内容的删除
	
	Linklist p,q;
	p = (*L)->next;
	while(p){                                         //当P的值不为空时一直清理内存
		q = p->next;
		free(p);
		p = q;
	}
    (*L)->next = NULL;

	return OK;
}

/***************************************静态链表**********************************************************************/
////*********************************************************************************************************/
typedef struct{
	Elemtype data;
	int cur;
}StaticLinklist[LIST_INIT_SIZE],Component;

StaticLinklist space;


int listlength_get(StaticLinklist space){          //获取静态链表长度
	int i,j;
	j = 0;
	i = space[LIST_INIT_SIZE-1].cur;
	while(i){
		j++
		i = space[i].cur;
	}
	return j;
}

Status StaticlinklistInit ( StaticLinklist space ){																		//静态链表的初始化
	int i;
	for( i = 0; i < LIST_INIT_SIZE-1;i++){
		space[i].cur = i+1;
	}
	space[LIST_INIT_SIZE-1].cur = 0;
	return OK;
							
}


Status Creatspace (StaticLinklist space){								//静态链表模拟malloc
	int i = space[0].cur;
	if(space[0].cur > LIST_INIT_SIZE-2)
		return ERROR;
	if(space[0].cur)
		space[0].cur = space[i].cur;
	return i;


}

Status Static_Linklist_Insert(StaticLinklist space,int i,ElemType e){                              //静态链表的插入操作
	int n,j,k;
	k = LIST_INIT_SIZE-1;
	if(i<1 || i>listlength_get(space)+1)
		return ERROR;
	j = Creatspace(space);
	if(j){
		space[j].data = e;
		for(n = 1;n<= i-1;n++)          //从最后一个数据往前走，一直到第i-1个数据，并不一定按数组顺序走，利用游标找到第i-1个数据
			k =  space[k].cur;
		space[j].cur = space[k].cur;
		space[k].cur = j;
		return OK;
	}
	return ERROR;
}



Status Freespace (StaticLinklist space,int k){          //把下标为K的空闲回收到备用链表
	if(i<1 || i>space[0].cur-1)
		return ERROR;
	space[k].cur = space[0].cur;
	space[0].cur = k;
	return OK;
}


Status Static_Linklist_Dele(StaticLinklist space,int i,ElemType e){
	int j,k;
	if(i<1 || i>listlength_get(space))
		return ERROR;
	k = LIST_INIT_SIZE-1;
	for(j = 1;j<= i-1;n++)
		k =  space[k].cur;
	j = space[k].cur;
	space[k].cur = space[j].cur;
	Freespace(space,j);
	return OK;

}

////////****************************************************单链表算法**********************************************************************/

Status GetMidNode(Linklist L，ElemType *e){              ///快慢指针的原理   一个指针二倍速前进，另一个指针就是中间位置！！！！！！！
	Linklist search,mid;
	mid = search = L;
	while(search->next != NULL){
		if(search->next->next != NULL){
			search = search->next->next;
			mid = mid->next;
		}
		else{
			search = search->next;
		}

	}
	*e = mid->data;
	return Ok;
}


////////***********************************************循环链表*************************************************************************/

typedef struct CirLinklist{
	
	ElemType data;
	struct CirLinklist *next;	

};
typedef struct CirLinklist * CirLinklist ;
CirLinklist node;

Status cir_linklist_Init(CirLinklist * pNode){                  //传参为头节点的头指针；   头节点是一个结构体指针
	int item;
	CirLinklist temp;
	CirLinklist target;
	printf("请输入节点的值\n");

	while(1){
		scanf("%d",&item);
		fflush(stdin)              //清空输入缓存

		if(item = 0){
			return 0;
		}
		if(*pNode == NULL){                 //头节点为空 ，即头节点的变量没有初始化
			(*pNode) ==(CirLinklist)malloc(sizeof(struct CirLinklist));
			if(!(*pNode)){
				exit(0);
			}
			(*pNode)->data = item;
			(*pNode)->next = *pNode;                     //循环链表中只有一个节点 构成循环链表
		}
		else{
			for(target = (*pNode);target->next!=(*pNode);target = target->next);         //找到指向第一个节点的节点  即尾为节点
			temp = (CirLinklist)malloc(sizeof(struct CirLinklist));
			if(!temp){
				exit(0);
			}
			temp->data = item;
			temp->next = (*pNode);
			target->next = temp;                                         //使新建的节点的变成最后一个节点
		}

	}
}
//插入节点
Status cir_linklist_Insert(CirLinklist * pNode,int i;){                        //参数为头节点的头指针和要插入的位置
	CirLinklist temp;
	CirLinklist target;
	CirLinklist p;
	int item;
	int j = 1;
	printf("要插入参数的值\n");
	scanf("%d",&item);
	if(i == 1){																//如果要把值插入到第一个节点
		temp = (CirLinklist)malloc(sizeof(struct CirLinklist));
		if(!temp){
		    exit(0);
		}
		
		temp->data =item;
		
		for(target = (*pNode);target->next!=(*pNode);target = target->next);              //找到指向第一个节点的节点  即尾为节点
		
		temp->next = (*pNode);
		target->next = temp;                                                              //此时temp为头节点；
		*pNode = temp;                                                           //更换该链表的头节点地址；
		
	}
	else{
		target = *pNode;
		for(;j<(i-1);j++){       //比如 i = 3;j = 1 从头节点 指向第三个节点    只要 令target 为第二个节点，第三个节点为target->next
			target = target->next;
		}                     
		temp = (CirLinklist)malloc(sizeof(struct CirLinklist));
		if(!temp){
		    exit(0);
		}

		temp->data = item;
		p = target->next;
		target->next = temp;
		temp->next = p;
	}
}



//删除指定节点
Status cir_linklist_Insert(CirLinklist * pNode,int i;){  
	CirLinklist temp;
	CirLinklist target;
	int j = 1;

	if(i == 1){                      //如果要删除第一个节点
		for(target = (*pNode);target->next!=(*pNode);target = target->next);              //找到指向第一个节点的节点  即尾为节点
		temp = (*pNode);
		(*pNode) = (*pNode)->next;
		target->next = (*pNode);
		free(temp);
	}
	else{
		target = (*pNode);
		for(;j<(i-1);j++){			//比如 i = 3;j = 1 从头节点 指向第三个节点    只要 令target 为第二个节点，第三个节点为target->next
			target = target->next;
		}
		temp = target->next;
		target->next = temp->next;
		free(temp);
	}
}





//！！可以设置尾指针  ，这样可以减少 遍历寻找最后一个元素的时间


/*如何识别 一个单链表有环    

方法一：   设置两个指针  ，第一个 每次移动一次 把步数累加起来 ，步数即与头节点相隔的距离
			另一个指针 每次都从头节点开始  移动第一个与头节点的距离 ，判断是否为同一内存 若非同一内存   则存在 环路 




方法二：   快慢指针的方法，设置两个指针 一个指针 每次移动两个距离  另一个每次移动一个距离 若 存在环 会出现两个指针会指向同一位置的情况；




*/







////////////////////////////////////////*********双向链表************************//////////////////////////////////////////////////////////


typedef struct DualNode{
	Elemtype data;
	struct DualNode *piror;          //前驱节点   指向 前一个节点
	struct DualNode *next;			 //后继节点	  指向 后一个节点
}DualNode;

typedef struct DualNode* DualNodelist;


//增，删，改，插   操作注意多了一个指针     










/****************************************************栈************************************************************************************/


	1.栈其实是特殊的线性表
	
	2.要符合先入后出的规则
	
	3.只能在表尾进行操作。    表尾称为栈顶top    表头称为栈底bottom



//创建一个顺序栈

typedef struct{
	
	ElemType *base;
	ElemType *top;
	int stackSize;
}sqStack;

#define STATCK_INIT_SIZE 100;

Status Stack_init(sqStack* s){
	s->base = (ElemType *)malloc(sizeof(ElemType)*STATCK_INIT_SIZE);
	if(!s->base){
		exit(0;)
	}
	s->top = s-base;               //最开始  栈顶就是栈底
	s->stackSize = STATCK_INIT_SIZE;

	return OK;

} 


//栈的插入操作push

#define STATCK_ADD_SIZE 10;          //当栈满动态追加栈的空间的大小


void *realloc(void *ptr, size_t size);      //重新生成空间   第一个参数是 要获取空间的地址的首位        第二个参数是要重新生成空间的大小

Status Stack_Push(sqStack* s , ElemType e){
	if(s->top - s->base >= s->stackSize){      //两个指针相减是  两个地址之间 相差的指针类型的个数
		s->base = (ElemType *)realloc(s->base,sizeof(ElemType)*(s->stackSize + STATCK_ADD_SIZE));
		if(!s->base ){
			exit(0);
		}
		s->top = s->base + s->stackSize;         //设置栈顶
		s->stackSize = s->stackSize + STATCK_ADD_SIZE ;    //设置栈的最大容量
	}
	*(s->top) = e;
	s->top++;
	return OK;
}

//栈的删除操作  pop


Status Stack_Pop(sqStack* s , ElemType *e){       //出栈并返回 出栈的数据
	if(s->top == s->base){             //栈只剩最后一个元素
		*e = *(s->top);
		*(s->top) = 0;
		return OK;
	}

	*e = (*(s->top)--);
	return OK;
}
















