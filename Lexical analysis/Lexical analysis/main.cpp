//
//  main.cpp
//  Lexical analysis
//
//  Created by Dixi-Chen on 2017/5/17.
//  Copyright © 2017年 Dixi-Chen. All rights reserved.
//
#include <string>
#include <iostream>
#include <cstdlib>
#include "DXEaseLexAnalysis.h"
using namespace std;

int main()
{
    DXEaseLexAnalysis analyzer;
    analyzer.scanSource();
    return 0;
}
