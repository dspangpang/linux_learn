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
































