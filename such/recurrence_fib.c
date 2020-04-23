#include "stdio.h"

int fib(int i){
	if(i<=1){
		return  i == 0 ? 0:1;
	}
	return fib(i-1)+fib(i-2);
}

int main(){
    int i = 0;
	int n;
	printf("最多打印第一百位的斐波那契数\n");
	printf("请输入你要打印的个数\n");
	scanf("%d",&n);
	
	for(i=0;i<n;i++){
		printf("%d ",fib(i));
	}
	return 0;
}
