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




