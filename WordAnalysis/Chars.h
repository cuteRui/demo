#if !defined(AFX_CHARS_H__D2786D29_6773_4679_93E5_3FC3AF73385F__INCLUDED_)
#define AFX_CHARS_H__D2786D29_6773_4679_93E5_3FC3AF73385F__INCLUDED_

// c是否为英文字符或下划线
bool IsEnglishCharOr_(char c)
{
	return ((c >= 'a' && c <= 'z')|| (c >= 'A' && c <= 'Z'))|| (c == '_');
}

// c是否为数字
bool IsNumChar(char c)
{
	return (c >= '0' && c <= '9');
}

// c是否为运算符
bool IsOperator(char c)
{
	switch (c)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		return true;
	}
	return false;
}
#endif