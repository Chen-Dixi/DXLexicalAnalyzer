#ifndef _Base_H_
#define _Base_H_

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable: 4996)

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <string>


//c���Թؼ��� 
static char keywords[50][13] = { "short", "int", "long", "float", "double", "char"
, "struct", "union", "enum", "typedef", "const", "unsigned", "signed", "extern"
, "static", "void", "if", "else", "switch", "case", "for", "do", "while",
"goto", "continue", "break", "default", "sizeof", "return", "true", "false" };

class Base
{
public:
	int buffernum;

public:
	Base(){ buffernum = 0; }
	virtual ~Base();

	int charkind(char c);//�ж��ַ�����
	int spaces(char c); //��ǰ�ո��Ƿ��������
	int characters(char c);//�Ƿ�����ĸ
	int keyword(char str[]);//�Ƿ��ǹؼ���
	int signwords(char str[]);//�Ƿ��Ǳ�ʶ��
	int numbers(char c);//�Ƿ�������
	int integers(char str[]);//�Ƿ�������
	int floats(char str[]);//�Ƿ��Ǹ�����
};

#endif
