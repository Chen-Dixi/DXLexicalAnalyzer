//
//  Parser.cpp
//  Parser
//
//  Created by Dixi-Chen on 2017/6/18.
//  Copyright © 2017年 Dixi-Chen. All rights reserved.
//

#include "TokenScanner.cpp"

class Parser{
    
public:
    
    Parser(vector<Token> list):tokenList(list){
        resetTokenIterator();
    }
    
    Token nextToken(){
//        vector<Token>::iterator temp = it;
//        it++;
        return *it++;
    }
    
    void resetTokenIterator(){
        it=tokenList.begin();
    }
    
    void printToken(){
        for(vector<Token>::iterator it = tokenList.begin();it!=tokenList.end();it++)
            cout<< it->value<<" , "<<it->tokenTag<<endl;
    }
    
private:
    vector<Token> tokenList;
    vector<Token>::iterator it; //iterator;
    
};


