//
//  main.cpp
//  Parser
//
//  Created by Dixi-Chen on 2017/6/16.
//  Copyright © 2017年 Dixi-Chen. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>

#include "Parser.cpp"
using namespace std;
int main(int argc, const char * argv[]) {
    TokenScanner scanner("./main.c");
    
    scanner.scanTokens();
    Parser parser(scanner.getTokenList());
    
    
}

