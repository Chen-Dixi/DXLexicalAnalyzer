//
//  DXEaseLexAnalysis.h
//  Lexical analysis
//
//  Created by Dixi-Chen on 2017/5/17.
//  Copyright © 2017年 Dixi-Chen. All rights reserved.
//

#ifndef DXEaseLexAnalysis_h
#define DXEaseLexAnalysis_h
#include <fstream>
#include <iostream>
#include "Log.h"

using namespace std;

static string keywords[] = { "short", "int", "long", "float", "double", "char"
    , "struct", "union", "enum", "typedef", "const", "unsigned", "signed", "extern"
    , "static", "void", "if", "else", "switch", "case", "for", "do", "while",
    "goto", "continue", "break", "default", "sizeof", "return", "true", "false" };


class DXEaseLexAnalysis{
    
public:
    FILE *infile;
    FILE *outfile;
    ifstream instream;
    ofstream outstream;
    
    DXEaseLexAnalysis(){
        infile = fopen("./in.txt", "r");
        outfile = fopen("./out.txt", "w");
        instream.open("./in.txt");
        outstream.open("./out.txt");
    }
    ~DXEaseLexAnalysis(){
        fclose(infile);
        fclose(outfile);
        instream.close();
        outstream.close();
    }
    
    void scanSource()//最顶层的函数，开始对源代码进行词法分析
    {
        char c;
        while(1){
            c = fgetc(infile);
            if(c==EOF){
                break;
            }
            
            if(characters(c)==1){//开头是字符
                string s="";
                s=s+c;
                do{
                    c=fgetc(infile);
                    if(characters(c)||numbers(c)){
                        s=s+c;
                    }
                }while(characters(c)||numbers(c));
                
                if(keyword(s)==1){//保留字
                    printToken(s, "keyword");
                }else{
                    printToken(s, "signword");
                }
                rollbackOneCharacer(infile);
            }else if(c=='/'){
                handleCom();
            }else if(numbers(c)==1){
                handleNum(c);
            }else if(c=='>'||c=='='||c=='<'){
                handleRecogDel(c);
            }
            
            
        }
    }
    
    
    
    
private:
    char *temp;
    int charkind(char c);//≈–∂œ◊÷∑˚¿‡–Õ
    int spaces(char c); //µ±«∞ø’∏Ò «∑Òø…“‘œ˚≥˝
    int characters(char c);// «∑Ò «◊÷ƒ∏
    int keyword(string str);// «∑Ò «πÿº¸◊÷
    int signwords(char str[]);// «∑Ò «±Í ∂∑˚
    int numbers(char c);// «∑Ò « ˝◊÷
    int integers(char str[]);// «∑Ò «’˚ ˝
    int floats(char str[]);// «∑Ò «∏°µ„–Õ
    void printToken(string,string);
    void handleCom(){
        char c=fgetc(infile);
        if (c!='*'){
            printToken("//", "注释");
            fgets(temp, 1024, infile);
        }
        else {
            c=fgetc(infile);
            char c0;
            while(!feof(infile)){
                c0=c;
                c=fgetc(infile);
                if(c0=='*' && c=='/'){
                    printToken("/**/", "嵌套注释");
                    break;
                }
            }
        }
    }
    
    void handleNum(char c){//处理常数
        string s="";
        s+=c;
        int state=1;//q0,q1,q2,q3,q4,q5,q6. 其中q1,q5,q6可以终结。 此时状态为1
        while(!feof(infile)){
            c=fgetc(infile);
            if((numbers(c)==0 && c!='E' && c!='e' && c!='+' && c!= '-') || ((c=='+'|| c=='-') && state!=2)){
                rollbackOneCharacer(infile);
                break;
            }
            switch (state) {
                case 0:{
                    if(numbers(c)==1)
                        state=1;
                    break;
                }
                case 1:{
                    if(numbers(c)==1)
                        state=1;
                    else if(c=='E' || c=='e'){
                        state=2;
                    }else if (c=='.' || c=='.')
                        state=3;
                    break;
                }
                case 2:
                {
                    if(numbers(c)==1)
                        state=5;
                    else if (c=='+'|| c=='-')
                        state=4;
                    break;
                }
                case 3:
                {
                    if(numbers(c)==1)
                        state=6;
                    break;
                }
                case 4:
                {
                    if(numbers(c)==1)
                        state=5;
                    break;
                }
                case 5:{
                    if(numbers(c)==1)
                        state=5;
                }
                case 6:
                {
                    if(numbers(c)==1)
                        state=6;
                    else if(c=='e'||c=='E')
                        state=2;
                }
                
                default:
                    break;
            }
            s+=c;
        }
        
        if (state==1 || state==5 || state==6){
            printToken(s, "const number");
        }
        
    }
    void handleRecogDel(char c){
        switch (c) {
            case '+':{
                printToken("+", "plus");
                break;
            }
            case '>':{
                c=fgetc(infile);
                switch (c) {
                    case '=':{
                        printToken(">=", "GE");
                        break;}
                    case '>':{
                        printToken(">>", "?");
                        break;
                    }
                    default:{
                        printToken(">", "GT");
                        rollbackOneCharacer(infile);
                        break;
                    }
                }

                break;
            }
            case '<':{
                c=fgetc(infile);
                switch (c) {
                    case '=':{
                        printToken("<=", "LE");
                        break;}
                    case '>':{
                        printToken("<>", "NE");
                        break;
                    }
                    case '<':{
                        printToken("<<", "?");
                        break;
                    }
                        
                    default:{
                        printToken("<", "LT");
                        rollbackOneCharacer(infile);
                        break;
                    }
                }
                break;
            }
            case '=':{
                c=fgetc(infile);
                switch (c) {
                    case '=':{
                        printToken("==", "E");
                        break;
                    }
                    default:
                    {
                        printToken("=", "assign");
                        rollbackOneCharacer(infile);
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    
    void rollbackOneCharacer(FILE* fp){
        fseek(fp, -1, SEEK_CUR);
    }
    
    
};

struct Token{
    int type;
    std::string value;
    
};

#endif /* DXEaseLexAnalysis_h */






