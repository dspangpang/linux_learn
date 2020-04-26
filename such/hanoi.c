

/*
汉诺塔问题 可划分为两个问题

1. 将x中的 n-1 个盘子 ， 借助z 放在 y 上 
2. 将y中 n-1 个盘子 ， 借助x 放在 z 上



问题 1 又可以分解为 将x中的前 n-2 个盘子借助 y 放在 z 上
	   再第 n-1 个盘子放在 y 上
	   最后把 z 上 的 n-2 个盘子放在 y 上
	   
	   
	   同理 可向下递归 ；
*/








#include <stdio.h>

//将 N 个盘子  从 X ,借助 Y 移动到 Z;
void hanoi_move(int n , char x, char y, char z){
	if(1 == n){
		printf("%c-->%c \n",x,z);
	}
	else{
		hanoi_move(n-1,x,y,z);           //将n-1个盘子从 x 借助 y 到 z 上
		printf("%c-->%c \n",x,z);        //将第 n 个盘子 从 x  放到 z 上
		hanoi_move(n-1,y,x,z);           //将n-1个盘子 从 y 借助 x 放到 z 上
	}
	
}

int main {
	int n;
	printf("请输入汉诺塔的层数 ：")
	scanf("%d",&n);
	printf("\n");
	hanoi_move(n,"X","y","Z");
	
	return 0;
}