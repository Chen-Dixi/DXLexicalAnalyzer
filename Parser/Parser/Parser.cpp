//
//  Parser.cpp
//  Parser
//
//  Created by Dixi-Chen on 2017/6/18.
//  Copyright © 2017年 Dixi-Chen. All rights reserved.
//

#include "TokenScanner.cpp"
#include "Log.h"
class Parser{
    
public:
    
    Parser(vector<Token> list):tokenList(list){
        resetTokenIterator();
    }
    
    Token nextToken(){
//        vector<Token>::iterator temp = it;
//        it++;
        if(it==tokenList.end()){
            XINFO("this is the last Token");
            return *it;
        }
        return *it++;
    }
    
    Token currentToken(){
        return *it;
    }
    
    Token lastToken(){
        if(it==tokenList.begin()){
            XINFO("this is the first Token");
            return *it;
        }
        return *it--;
    }
    
    void resetTokenIterator(){
        it=tokenList.begin();
    }
    
    void printToken(){
        for(vector<Token>::iterator it = tokenList.begin();it!=tokenList.end();it++)
            cout<< it->value<<" , "<<it->tokenTag<<endl;
    }
    
    //开始解析的接口
    void startParse(){
        Program();
        XINFO("parse success ");
    }
    
    
private:
    vector<Token> tokenList;
    vector<Token>::iterator it; //iterator;
    
    void Program(){
        Type();
        if (nextToken().value != "main"){
            ExitWithError();
        }
        if (nextToken().value != "("){
            ExitWithError();
        }
        if (nextToken().value != ")"){
            ExitWithError();
        }
        StatementBlock();

    }
    
    void Type(){
        Token token = nextToken();
        if(token.tokenTag>=1 && token.tokenTag<=5){
            // do something
        }
        else{
            //error
            ExitWithError();
        }
    }
    
    void StatementBlock(){
        if (!TokenValueIs(nextToken(), "{"))
            ExitWithError();
        StatementSequence();
        if (!TokenValueIs(nextToken(), "}"))
            ExitWithError();
    }
    
    void StatementSequence(){
        DeclareSequence();
        ExecuteSequence();
    }
    
#pragma mark decaler parser
    void DeclareSequence(){
        if (TokenTagInRange(currentToken(), 1, 5))
        {
            Type();
            IdSequence();
            if (nextToken().value != ";"){
                ExitWithError();
            }
            DeclareSequence();
        }
    }
    
    
    
    void IdSequence(){
        OneId();
        IdSequence1();
    }
    
    void IdSequence1(){
        if (currentToken().value == ",")
        {
            nextToken();
            OneId();
            IdSequence1();
        }
    }
    
    void OneId(){
        if (!TokenTagIs(nextToken(), 20))
            ExitWithError();
        Assign();
        
    }
    
    void Assign(){
        if (currentToken().value == "="){
            nextToken();
            Express();
        }
    }
    
    void Express(){
        Term();
        Express1();
    }
    
    void Express1(){
        if(currentToken().value=="+"||currentToken().value=="-"){
            nextToken();
            Term();
            Express1();
        }
    }
    
    void Term(){
        Factor();
        Term1();
    }
    
    void Factor(){
        
        if (TokenTagInRange(currentToken(), 20,3)){
            nextToken();
        }else if(TokenValueIs(currentToken(), "(")){
            nextToken();
            Express();
            if(!TokenValueIs(nextToken(), ")"))
                ExitWithError();
        }
    }
    
    void Term1(){
        if(currentToken().value=="*"||currentToken().value=="/"){
            nextToken();
            Factor();
            Term1();
        }
    }
    
    void Semicolon(){
        if ( !TokenValueIs(nextToken(), ";") ){
            ExitWithError();
        }
    }
#pragma mark execute parser
    
    void ExecuteSequence(){
        if( TokenTagIs(currentToken(), 20)|| TokenTagInRange(currentToken(), 10, 3) ){
            Execute();
            ExecuteSequence();
        }
    }
    void Execute(){
        if(TokenTagIs(currentToken(), 20)){
            OneId();
            Semicolon();
        }else if( TokenTagIs(currentToken(), 10) ){
            WhileLoop();
        }else if( TokenTagIs(currentToken(), 11) ){
            Branch();
        }else if (TokenTagIs(currentToken(), 12)){
            FoorLoop();
        }
    }
    
    void WhileLoop(){
        if(!TokenTagIs(nextToken(), 10))
            ExitWithError();
        if(!TokenValueIs(nextToken(), "(")){
            ExitWithError();
        }
        Condition();
        if(!TokenValueIs(nextToken(), ")")){
            ExitWithError();
        }
        if (!TokenValueIs(nextToken(), "{"))
            ExitWithError();
        ExecuteSequence();
        if (!TokenValueIs(nextToken(), "}"))
            ExitWithError();
    }
    
    void Branch(){
        if(!TokenTagIs(nextToken(), 11))
            ExitWithError();
        if(!TokenValueIs(nextToken(), "(")){
            ExitWithError();
        }
        Condition();
        if(!TokenValueIs(nextToken(), ")")){
            ExitWithError();
        }
        if (!TokenValueIs(nextToken(), "{"))
            ExitWithError();
        ExecuteSequence();
        if (!TokenValueIs(nextToken(), "}"))
            ExitWithError();
        Branch1();
    }
    
    void Branch1(){
        if(TokenTagIs(currentToken(), 13)){
            nextToken();
            if(TokenTagIs(currentToken(), 11)){
                Branch();
            }else{
                if (!TokenValueIs(nextToken(), "{"))
                    ExitWithError();
                ExecuteSequence();
                if (!TokenValueIs(nextToken(), "}"))
                    ExitWithError();
            }
        }
    }
    
    void FoorLoop(){
        if(!TokenTagIs(nextToken(), 12))
            ExitWithError();
        if(!TokenValueIs(nextToken(), "(")){
            ExitWithError();
        }
        A();
        Semicolon();
        B();
        Semicolon();
        A();
        if(!TokenValueIs(nextToken(), ")")){
            ExitWithError();
        }
        if (!TokenValueIs(nextToken(), "{"))
            ExitWithError();
        ExecuteSequence();
        
        if (!TokenValueIs(nextToken(), "}"))
            ExitWithError();

    }
    
    void Condition(){
        Factor();
        Cmp();
    }
    
    void Cmp(){
        if(TokenTagInRange(currentToken(), 25, 5)){
            nextToken();
            Factor();
        }
    }
    
    void A(){
        if(TokenTagIs(currentToken(), 20)){
            OneId();
        }
    }
    
    void B(){
        if(!TokenValueIs(currentToken(), ";")){
            Condition();
        }
    }
#pragma mark compare helper
    bool TokenValueIs(Token token,string value){
        if(token.value==value)
            return true;
        return false;
    }
    
    bool TokenTagIs(Token token,int tag){
        if (token.tokenTag==tag)
            return true;
        return false;
    }
    
    bool TokenTagInRange(Token token, int start,int length){
        if (token.tokenTag>=start && token.tokenTag<start+length)
            return true;
        return false;
    }
    
    bool TokenTagIn(Token token,int n,...){
        int *temp = &n;
        ++temp;
        for (int i = 0; i < n; ++i)
            if(token.tokenTag==*(temp+i))
                return true;
        return false;
    }
    
#pragma mark parse failed
    void ExitWithError(){
        XERROR("parse failed");
        exit(-1);
    }
    
    
};


