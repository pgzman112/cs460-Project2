/*******************************************************************************
* File name: SyntacticalAnalyzer.h                                             *
* Project: CS 460 Project 2 Fall 2020                                          *
* Author:                                                                      *
* Date:                                                                        *
* Description: This file contains                                              *
*******************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	token_type token;
	ofstream p2file;
	int program ();
	int moreDefines();
	int defines();
	int stmtList();
	int stmt();
	int literal();
	int quotedLit();
	int moreTokens();
	int paramList();
	int stmtPair();
	int stmtPairBody();
	int elsePart();
	int assignsPair();
	int moreAssigns();
	int action();
	int anyOtherToken();

};
	
#endif
