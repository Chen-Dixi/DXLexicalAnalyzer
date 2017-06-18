//
//  Scanner.cpp
//  Parser
//
//  Created by Dixi-Chen on 2017/6/18.
//  Copyright © 2017年 Dixi-Chen. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <map>
#include <stack>
#include <vector>

using namespace std;

static string keywords[] = { "main", "int", "char","float", "double","void","struct","long","signed","unsigned", "while","if","else","for","do","switch","case","break","continue","return" };


enum AnnotationState:int{ //anotation state enum,
    None=0,
    DoubleSlashComment=1,//单行注释
    MultiLineCommnt=2,  // 多行注视
};

struct Token{
    int tokenTag;
    string value;
    Token(int tag,string value):tokenTag(tag),value(value){}
};

struct Error{
    string content;
    int row,col;
    Error(string s,int row,int col):content(s),row(row),col(col){}
};


class TokenScanner{
public:
    FILE *infile;
    FILE *outfile;
    ifstream instream;
    ofstream outstream;
    
    TokenScanner(const char* infileName="./in.txt",const char* outfileName="./out.txt"){
        infile = fopen(infileName, "r");
        outfile = fopen(outfileName, "w");
        instream.open(infileName);
        outstream.open(outfileName);
        initTokenCode();//初始化
    }
    
    
    ~TokenScanner(){
        fclose(infile);
        fclose(outfile);
        instream.close();
        outstream.close();
    }
    
    void solve(){
        currentCol=1;
        for(int i=0;i<currentCodeInLine.size();i++){
            char c = currentCodeInLine[i];
            if ( annotationState==None){ //not commenting
                if(c<0 || c>126){
                    LogError("Ilegal character",i);
                }else if (characters(c)==1 || c == '$' || c == '_'){
                    i=scanIdentifier(i) - 1;
                }else if(isdigit(c)){
                    i=scanNumber(i)-1;
                }else if (c=='/'){
                    int res = scanSlash(i);
                    if(res==1)
                        return;
                    if(res==2)
                        i+=1;
                }else{
                    bracketMatch(i);
                    scanComma(i);
                    scanSemicolon(i);
                    i=scanOperation(i);
                }
            }else if(annotationState==MultiLineCommnt){
                if (c=='*' && currentCodeInLine[i+1]=='/'){
                    annotationState=None;
                    i=i+1;
                }
            }
        }
        if (annotationState==DoubleSlashComment) //when finish reading one line
            annotationState=None;
        
        
    }
    //最顶层的函数，开始对源代码进行词法分析
    //一行一行读数据
    void scanTokens(){
        tokenList.clear();
        errorMsg.clear();
        annotationState = None;
        while( !bracketStack.empty() )//清空括号站
            bracketStack.pop();
        currentCodeInLine="";
        currentRow=0;
        
        while(getline(instream, currentCodeInLine)){
            currentRow++;
            solve();
        }
        
        if (!bracketStack.empty()){
            LogError("error bracket order", 0);
        }
        
    }
    
    vector<Token> getTokenList(){
        return tokenList;
    }
    
    void printError(){
        for(vector<Error>::iterator it = errorMsg.begin();it!=errorMsg.end();it++)
            cout<< it->content<<"  "<<it->row<<" , "<<it->col<<endl;
    }

private:
    string currentCodeInLine;
    int currentRow;
    int currentCol;
    AnnotationState annotationState;
    vector<Token> tokenList;
    stack<char> bracketStack;
    vector<Error> errorMsg;
    map<string,int> tokenCode;
    void initTokenCode(){
        tokenCode["int"]=1;
        tokenCode["char"]=2;
        tokenCode["float"]=3;
        tokenCode["double"]=4;
        tokenCode["long"]=5;
        tokenCode["struct"]=6;
        tokenCode["unsigned"]=7;
        tokenCode["signed"]=8;
        tokenCode["void"]=9;
        tokenCode["while"]=10;
        tokenCode["if"]=11;
        tokenCode["else"]=12;
        tokenCode["for"]=13;
        tokenCode["do"]=14;
        tokenCode["switch"]=15;
        tokenCode["case"]=16;
        tokenCode["break"]=17;
        tokenCode["continue"]=18;
        tokenCode["default"]=19;
        tokenCode["id"]=20;
        tokenCode["constInteger"]=21;
        tokenCode["constFloat"]=22;
        tokenCode["constString"]=23;
        tokenCode["="]=24;
        tokenCode["<"]=25;
        tokenCode[">"]=26;
        tokenCode[">="]=27;
        tokenCode["<="]=28;
        tokenCode["=="]=39;
        tokenCode["!="]=30;
        tokenCode["'"]=31;
        tokenCode["\""]=32;
        tokenCode[","]=33;
        tokenCode["("]=34;
        tokenCode[")"]=35;
        tokenCode["{"]=36;
        tokenCode["}"]=37;
        tokenCode["["]=38;
        tokenCode["]"]=39;
        tokenCode["+"]=40;
        tokenCode["-"]=41;
        tokenCode["*"]=42;
        tokenCode["/"]=43;
        tokenCode["&"]=44;
        tokenCode["|"]=45;
        tokenCode["~"]=46;
        tokenCode["&&"]=47;
        tokenCode["||"]=48;
        tokenCode["!"]=49;
        tokenCode[";"]=50;
    }
#pragma mark ilegal word
    
    void LogError(string msg,int col){
        
        errorMsg.push_back( Error(msg,currentRow,col) );
    }
    
    
    
#pragma mark helper function
    
    int characters(char c)
    {
        if ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'))
        {
            return 1;
        }
        return 0;
    }
    
    
    int keyword(string str)
    {
        int i;
        for (i = 0; i < 20; i++)
        {
            if (str==keywords[i])
            {
                return i+1;
            }
        }
        return 0;
    }
    
    int spaces(char c)
    {
        if ((c > 'z' || (c < 'a' && c > 'Z') || (c < 'A' && c > '9') || (c < '0')) && c != '_' && c != '$')
        {
            return 1;
        }
        return 0;
    }

    int signwords(char str[])
    {
        int i;
        if (str[0] == '$' || str[0] == '_' || characters(str[0]) == 1)
        {
            for (i = 0; str[i] != '\0'; i++)
            {
                if (spaces(str[i]) == 1)
                {
                    return 0;
                }
            }
            return 1;
        }
        return 0;
    }
    
    int isNumComp(char ch){
        if(isdigit(ch) || ch=='.' || ch=='e' || ch=='E' || ch == '+' || ch=='-' )
            return 1;
        return 0;
    }
    
    
#pragma mark further scan
    //扫描标识符；
    int scanIdentifier(int startIndex){
        int index;
        string str="";
        str+=currentCodeInLine[startIndex++];
        
        for(index=startIndex; index<currentCodeInLine.size() && (spaces(currentCodeInLine[index])==0) ;index++){
            str+=currentCodeInLine[index];
        }
        
        if(keyword(str)!=0){
            tokenList.push_back( Token(tokenCode[str],str) );
        }else{
            tokenList.push_back( Token(tokenCode["id"],str) );
        }
        return index;
    }
    
    //扫描数字常数
    int scanNumber(int startIndex){
        int index;
        string str="";
        str+=currentCodeInLine[startIndex++];
        int state=1;
        for(index=startIndex; index<currentCodeInLine.size() && isNumComp(currentCodeInLine[index]) ;index++){
            char c=currentCodeInLine[index];
            if(!isNumComp(c) || ((c=='+'|| c=='-') && state!=2)){
                break;
            }
            
            switch (state) {
                case 0:{
                    if(isdigit(c))
                        state=1;
                    break;
                }
                case 1:{
                    if(isdigit(c))
                        state=1;
                    else if(c=='E' || c=='e'){
                        state=2;
                    }else if (c=='.')
                        state=3;
                    else if(characters(c)==1){
                        state=7;
                    }
                    break;
                }
                case 2:
                {
                    if(isdigit(c))
                        state=5;
                    else if (c=='+'|| c=='-')
                        state=4;
                    else if(c=='E' || c=='e')
                        state=7;
                    break;
                }
                case 3:
                {
                    if(isdigit(c))
                        state=6;
                    else if (c=='.'){
                        state=7;
                    }
                    break;
                }
                case 4:
                {
                    if(isdigit(c))
                        state=5;
                    break;
                }
                case 5:{
                    if(isdigit(c))
                        state=5;
                }
                case 6:
                {
                    if(isdigit(c))
                        state=6;
                    else if(c=='e'||c=='E')
                        state=2;
                }
                default:
                    break;
            }
            str+=c;
        }
        if (state==1 || state==5){
            tokenList.push_back( Token(tokenCode["constInteger"], str) );
        }else if (state==6){
            tokenList.push_back( Token(tokenCode["constFloat"], str) );
        }else{
            LogError("Ilegal number "+str, index);
        }
        return index;
    }
    
    //注释
    void scanDoubleSlash(int startIndex){
        if (annotationState==None){
            if (currentCodeInLine[startIndex]=='/' && currentCodeInLine[startIndex+1]=='/')
                annotationState = DoubleSlashComment;
        }
    }
    
    void scanSlashStar(int startIndex){
        if (annotationState==None){
            if (currentCodeInLine[startIndex]=='/' && currentCodeInLine[startIndex+1]=='*')
                annotationState = MultiLineCommnt;
        }
    }
    
    void bracketMatch(int curPos){
        string bracket="";
        if(currentCodeInLine[curPos]=='(' || currentCodeInLine[curPos]=='{' || currentCodeInLine[curPos]=='['){
            bracket+=currentCodeInLine[curPos];
            
            //printf("%d %s\n",tokenCode[op],op);
            bracketStack.push(currentCodeInLine[curPos]);
            tokenList.push_back( Token(tokenCode[bracket] ,bracket) );
        }else if(currentCodeInLine[curPos]==')' || currentCodeInLine[curPos]=='}' || currentCodeInLine[curPos]==']'){
            if(bracketStack.empty()){
                LogError("Bracket order error:", curPos);
            }else{
                
                char ch=bracketStack.top();
                if(ch=='(' && currentCodeInLine[curPos]==')'){
                    //printf("%d %s\n",tokenCode[")"],")");
                    tokenList.push_back( Token (tokenCode[")"] ,")") );
                    bracketStack.pop();
                    
                }else if(ch=='{' && currentCodeInLine[curPos]=='}'){
                    //printf("%d %s\n",tokenCode["]"],"}");
                    tokenList.push_back( Token (tokenCode["}"] ,"}") );
                    bracketStack.pop();
                
                }else if(ch=='[' && currentCodeInLine[curPos]==']'){
                    //printf("%d %s\n",tokenCode["}"],"]");
                    tokenList.push_back( Token (tokenCode["]"] ,"]") );
                    bracketStack.pop();
                }else{
                    LogError("Bracket order error:", curPos);
                }
            }
        }
        
    }
    
    int scanSemicolon(int curPos){
        if(currentCodeInLine[curPos]==';'){
            
            tokenList.push_back(Token(tokenCode[";"],";"));
        }
        return curPos;
    }
    
    int scanComma(int curPos){
        if(currentCodeInLine[curPos]==','){
            //printf("%d %s\n",tokenCode[","],",");
            tokenList.push_back(Token(tokenCode[","],","));
        }
        return curPos;
    }
    int scanOperation(int startIndex){//次函数需要在非注释状态下能成立
        
        
        string s="";
        char c = currentCodeInLine[startIndex];
        int i=0;
        switch (c) {
            case '+':{
                tokenList.push_back(Token(tokenCode["+"],"+"));
                break;
            }
            case '-':{
                tokenList.push_back(Token(tokenCode["-"],"-"));
                break;
            }
            case '*':{
                tokenList.push_back(Token(tokenCode["*"],"*"));
                break;
            }
            case '>':{
                c=currentCodeInLine[startIndex+1];
                switch (c) {
                    case '=':{
                        tokenList.push_back(Token(tokenCode[">="],">="));
                        i++;
                        break;
                    }
                    default:{
                        tokenList.push_back(Token(tokenCode[">"],">"));
                        break;
                    }
                }
                break;
            }
            case '<':{
                c=currentCodeInLine[startIndex+1];
                switch (c) {
                    case '=':{
                        tokenList.push_back(Token(tokenCode["<="],"<="));
                        i++;
                        break;
                    }
                    default:{
                        tokenList.push_back(Token(tokenCode["<"],"<"));
                        break;
                    }
                }
                break;
            }
            case '=':{
                c=currentCodeInLine[startIndex+1];
                switch (c) {
                    case '=':{
                        tokenList.push_back(Token(tokenCode["=="],"=="));
                        i++;
                        break;
                    }
                    default:
                    {
                        tokenList.push_back(Token(tokenCode["="],"="));
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
        
        return startIndex+i;
    }
    int scanSlash(int startIndex){
        string s="";
        char c = currentCodeInLine[startIndex+1];
        if (c=='/'){
            return 1;
        }
        else if (c=='*'){
            annotationState = MultiLineCommnt;
            return 2;
        }else{
            tokenList.push_back(Token(tokenCode["/"],"/"));
        }
        return 0;
    }
};
