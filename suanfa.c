/**********************************滑动窗口***************************************************/
//常用在字符串和数组的处理上 ，寻找最小长度最大长度之类

//一个字符串，要在里面找出最长且没有重复字符的子串，就像拿着卷尺在上面不停地缩拉测量

//子串就是这个卷尺的伸出部分，即一个窗口，左边缩进，右边拉出

//因为不能有重复的字符，在右端逐渐拉长的过程中，每新增加的一个新字符都要拿来和左侧子串中的字符做对比

//给定一个字符串，请你找出其中不含有重复字符的 最长子串 的长度。


int lengthOfLongestSubstring(char * s){
	int i = 0, j = 0;															//i为左窗口   ，j为右窗口； 
	int maxlen = 0;
	int curlen = 0;
	int len = strlen(s);
	if (len == 0)
		return 0;                                                               //字符串长度为 0时返回 0；
	for (; j < len && len - i > maxlen; j++){
		curlen++;
		for (int k = i; k <= j; k++){
			if (s[k] == s[j + 1]){                                       //当发现 右窗口出现重复，左窗口向右移动到左侧发生重复的字符的右侧
				if (curlen > maxlen) 
					maxlen = curlen;
				i = k + 1;
				curlen = j - i + 1;
				break;
			 }
		}	
	}

	if (curlen > maxlen);
		return curlen;
	else
		return maxlen;
}

////*********************************************中心扩散法**************************************************************************/

//中心扩散法的思路是：遍历每一个索引，以这个索引为中心，利用“回文串”中心对称的特点，往两边扩散，看最多能扩散多远。

//题目  给定一个字符串 s，找到 s 中最长的回文子串。你可以假设 s 的最大长度为 1000*//// 



/*枚举“中心位置”时间复杂度为 O(N)，从“中心位置”扩散得到“回文子串”的时间复杂度为 O(N)，因此时间复杂度可以降到O(N2）
 在这里要注意一个细节：回文串在长度为奇数和偶数的时候，“回文中心”的形式是不一样的。
 奇数回文串的“中心”是一个具体的字符，例如：回文串 “aba” 的中心是字符 “a”；
 偶数回文串的“中心”是位于中间的两个字符的“空隙”，例如：回文串串 “abba” 的中心是两个 “b” 中间的那个“空隙”。*/







char * longestPalindrome(char * s){
    if(strlen(s)==0||strlen(s)==1)
		return s;
	int i,start,left,right,count,len;
	start = len =0;
	for(i=0;s[i]!='\0';i+=count){
		count = 1;
		left=i-1;
		right = i+1;
		while(s[right]!='\0'&&s[i]==s[right]){ //处理重复字符串
			right++;
			count++;
		}
		while(left>=0 && s[right]!='\0' && s[left] == s[right]){
			left--;
			right++;
	    }
        if(right-left-1>len){
		start = left+1;											
		len = right-left-1;
		}
						
	}	
	s[start + len] = '\0';      // 原地修改返回
	return s + start;
}























