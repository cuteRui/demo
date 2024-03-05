#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "Chars.h"
#define MAX_DATA_LEN 256 // 数据缓冲区长度

#define WT_OPERATOR 0 // 操作符
#define WT_UINT 1	  // 非负整数
#define WT_VARIABLE 2 // 变量

struct WOODNOTE
{
	unsigned short byType;	  // 类别
	char Value[MAX_DATA_LEN]; // 值
	WOODNOTE *pNext;		  // 下一结点
};

// 预处理：将多余空格去掉
void Prefix(char c[])
{
	int i, j;
	for (i = 0, j = 0; j < MAX_DATA_LEN && c[j] != '\0'; j++)
	{
		if (c[j] != ' ')
		{

			c[i++] = c[j];
		}
	}
	c[i] = '\0';
}

// 清空链表
void Clear(WOODNOTE *pHeader)
{
	WOODNOTE *pNode;
	while (pHeader != NULL)
	{
		pNode = pHeader->pNext;
		free(pHeader);
		pHeader = pNode;
	}
}

// 增加结点
WOODNOTE *AddNode(char c[], int nBegin, int nEnd, unsigned short byType, WOODNOTE *pTail)
{
	WOODNOTE *pNode = (WOODNOTE *)malloc(sizeof(WOODNOTE));
	pNode->byType = byType;
	pNode->pNext = NULL;
	int nChars = nEnd - nBegin + 1;
	memcpy(pNode->Value, &c[nBegin], nChars);
	pNode->Value[nChars] = '\0';
	pTail->pNext = pNode;
	return pNode;
}
// 根据上一状态获取单词类别
int Judd(char pass_c) // 我自己写的类型判别函数
{
	if (IsOperator(pass_c))
		return 0;
	if (IsNumChar(pass_c))
		return 1;
	if (IsEnglishCharOr_(pass_c))
		return 2;
	else
		return 0xFF;
}

WOODNOTE *IdentifyOneWord(char c[], int &nCur, WOODNOTE *pTail)
{
	// WOODNOTE *p;
	int con_num;		   // 状态变量
	int num_star, num_ter; // the place of starting and terminal of word
	num_star = nCur;
	con_num = Judd(c[nCur]);
	do
	{
		nCur++;
		if (con_num != 2)
		{
			if (con_num == 0 && Judd(c[nCur]) == 0)
			{
				num_ter = nCur - 1;
				return AddNode(c, num_star, num_ter, con_num, pTail);
				num_star = nCur;
			}
			if (con_num != Judd(c[nCur]))
			{
				num_ter = nCur - 1;
				return AddNode(c, num_star, num_ter, con_num, pTail);
				num_star = nCur;
			}
		}
		else if (con_num == 2)
		{
			if (!(Judd(c[nCur]) == 1 || Judd(c[nCur]) == 2))
			{
				num_ter = nCur - 1;
				return AddNode(c, num_star, num_ter, con_num, pTail);
				num_star = nCur;
			}
			// return 0;
		}
	} while (c[nCur] != '\0');
}

// 词法分析
WOODNOTE *WordAnalysis(char c[])
{
	// 第一个结点作为头结点，不使用
	WOODNOTE *pHeader = (WOODNOTE *)malloc(sizeof(WOODNOTE));
	pHeader->pNext = NULL;
	WOODNOTE *pTail = pHeader, *pNode = NULL;
	// 词法分析
	for (int nCur = 0; c[nCur] != '\0';)
	{
		// 识别一个单词
		pNode = IdentifyOneWord(c, nCur, pTail);
		if (pNode == NULL) // 出错
		{
			Clear(pHeader);
			return NULL;
		}
		// 识别下一个单词
		pTail = pNode;
	}
	return pHeader;
}

bool Save(WOODNOTE *pHeader)
{
	// 文件名
	char FileName[256];
	printf("单词序列输出文件名（如a.txt）：\n");
	scanf("%s", FileName);
	// 打开文件
	FILE *f = fopen(FileName, "w");
	if (f == NULL)
	{
		Clear(pHeader);
		return false;
	}
	// 空出第一个结点
	WOODNOTE *pNode = pHeader->pNext;
	// 保存数据
	while (pNode != NULL)
	{
		fprintf(f, "%c,%s\n", pNode->byType + '0', pNode->Value);
		pNode = pNode->pNext;
	}
	// 关闭文件
	fclose(f);
	return true;
}

// 主函数
int main(int argc, char *argv[])
{
	char c[MAX_DATA_LEN];
	printf("请输入表达式：\n");
	//读入字符串
	gets(c);
	// 预处理：去除多余空格
	Prefix(c);
	// 词法分析
	WOODNOTE *pHeader = WordAnalysis(c);
	if (pHeader == NULL)
	{
		printf("\n词法分析错误!\n");
		return 0;
	}
	// 保存
	if (!Save(pHeader))
	{
		printf("\n保存文件失败\n");
		return 0;
	}
	// 清空数据
	Clear(pHeader);
	// 完成
	printf("\n词法分析成功，并已保存到文件\n", c);
	printf("按任意键退出\n", c);
	getchar();
	getchar();
	return 0;
}