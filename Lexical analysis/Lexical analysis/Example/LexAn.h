#ifndef _LEXAN_H_
#define _LEXAN_H_
#include"Base.h"
class LexAn : public Base
{
	private:
		FILE *fin;
		FILE *fout;
		char bufferin[2][256];
		char bufferscan[256];
	public:
		LexAn()
		{
			fin = fopen("in.txt", "r");
			fout = fopen("out.txt", "w");
		}
		virtual ~LexAn();
		void scanwords(); //����ÿһ��
		void clearnotes();//���ע�ͺͶ���Ŀո�
		void getwords(int state);//���������
		void wordkind(char str[]);//�жϵ������Ͳ������
};
#endif 
