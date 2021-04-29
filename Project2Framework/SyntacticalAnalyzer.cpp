/*******************************************************************************
* File name: SyntacticalAnalyzer.cpp                                           *
* Project: CS 460 Project 2 Fall 2020                                          *
* Author:                                                                      *
* Date:                                                                        *
* Description: This file contains                                              *
*******************************************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <set>
#include "SyntacticalAnalyzer.h"

using namespace std;

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
	lex = new LexicalAnalyzer (filename);
	token_type t;
	string name = filename;
	for(int i = 0; i < 3; i++){
	    name.pop_back();
	}
	name = name + ".p2";
	p2file.open(name);
    int totalErrors = program ();


}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
	delete lex;
	p2file.close();
}

int SyntacticalAnalyzer::program ()
{
	int errors = 0;
	// Get the first token
	token = lex->GetToken();
	set<token_type> firsts = {LPAREN_T, EOF_T};
	set<token_type> follows = {EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
	if(token == LPAREN_T){
	    p2file << "Using Rule 1" << endl;
	    // Consume the LPAREN Because its correct
	    token = lex->GetToken();
	    if(token == DEFINE_T){
	        // Consume the Define because its correct then call defines
	        //token = lex->GetToken();
	        //cout << "NEAR HERE AT ALL?" << endl;
	        errors += defines();
	        if(token == LPAREN_T) {
                //cout << "nearrrrr?   " << lex->GetTokenName(token)<< endl;
                token = lex->GetToken();
                //cout << "nearrrrr?   " << lex->GetTokenName(token)<< endl;
                errors += moreDefines();
                if (token == EOF_T) {
                    token = lex->GetToken();
                }
                else {
                    errors++;
                    string mess;
                    mess += lex->GetLexeme();
                    mess += " unexpected";
                    lex->ReportError(mess);
                }
            }
	        else{
	            errors++;
                string mess;
                mess += lex->GetLexeme();
                mess += " unexpected";
                lex->ReportError(mess);
	        }
	    }
	    else{
	        errors++;
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);

	    }
	}
	else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
	    errors++;
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
	}
	return errors;
}

int SyntacticalAnalyzer::moreDefines() {
    int errors = 0;
    set<token_type> firsts = {IDENT_T, DEFINE_T, EOF_T};
    set<token_type> follows = {EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == IDENT_T){
        p2file << "Using Rule 3" << endl;
        token = lex->GetToken();
        errors += stmtList();
        if(token == RPAREN_T){
            token = lex->GetToken();
        }
        else{
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else if(token == DEFINE_T){
        p2file << "Using Rule 2" << endl;
        //token = lex->GetToken();
        errors += defines();
        if(token == LPAREN_T){
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
            errors += moreDefines();
        }
        else{
            errors++;
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
        }
        //errors += moreDefines();
    }
    else{
        errors++;
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::defines() {
    int errors = 0;
    p2file << "Using Rule 4" << endl;
    set<token_type> firsts = {DEFINE_T, EOF_T};
    set<token_type> follows = {LPAREN_T, EOF_T};
    //cout << "we here tok-> " << lex->GetTokenName(token) << endl;
    if(firsts.find(token) == firsts.end()){
        //cout << "REEEEEEE   " << lex->GetTokenName(token) << endl;
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
            //cout << "REEEEEEE   " << lex->GetTokenName(token) << endl;
        }
    }
    //cout << "we here2 tok-> " << lex->GetTokenName(token) << endl;
    token = lex->GetToken();
    // Token should correcntly be a Lparen. if not its an error.
    if(token == LPAREN_T){
        //cout << "we here3 tok-> " << lex->GetTokenName(token) << endl;
        // If correct consume that bbbbbb
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        if(token == IDENT_T){
           // cout << "we here4 tok-> " << lex->GetTokenName(token) << endl;
            // Token is an IDEN so its correct, so consume and call paramList()
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
            // Param list does not necesarrily need to consume anything
            errors += paramList();
            if(token == RPAREN_T){
                //Token is an Rparen so consume and move on.
                token = lex->GetToken();
                // "curr tok before stmt: " << lex->GetTokenName(token) << endl;
                errors += stmt();
               // cout << "curr tok before stmtlst after stmt: " << lex->GetTokenName(token) << endl;
                errors += stmtList();
               // cout << "after stmtList: " << lex->GetTokenName(token) << endl;
                //token = lex->GetToken();
                if(token == RPAREN_T){
                    // This should be consuming the very last token.
                    token = lex->GetToken();
                   // cout << "after last Rparen Deff: " << lex->GetTokenName(token) << endl;
                    return errors;
                }
                else{
                    errors++;
                    string mess;
                    mess += lex->GetLexeme();
                    mess += " unexpected";
                    lex->ReportError(mess);
                }
            }
        }
        else{
            errors++;
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
        }
    }
    else{
        errors++;
        errors++;
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::stmtList() {
    int errors = 0;
    set<token_type> firsts = {NUMLIT_T, STRLIT_T, SQUOTE_T, IDENT_T, LPAREN_T, RPAREN_T, EOF_T};
    set<token_type> follows = {EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == NUMLIT_T){
        p2file << "Using Rule 5" << endl;
        errors += stmt();
        errors += stmtList();
    }
    else if(token == STRLIT_T){
        p2file << "Using Rule 5" << endl;
        errors += stmt();
        errors += stmtList();
    }
    else if(token == SQUOTE_T){
        p2file << "Using Rule 5" << endl;
        errors += stmt();
        errors += stmtList();
    }
    else if(token == IDENT_T){
        p2file << "Using Rule 5" << endl;
        errors += stmt();
        errors += stmtList();
    }
    else if(token == LPAREN_T){
        p2file << "Using Rule 5" << endl;
        errors += stmt();
        errors += stmtList();
    }
    else if(token == RPAREN_T){
        p2file << "Using Rule 6" << endl;
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors += stmt();
        errors += stmtList();
        errors++;
//        while(follows.find(token) == follows.end()){
//            token = lex->GetToken();
//        }
    }
    return errors;
}

// I THINK STMT SHOULD CALL LITERAL
int SyntacticalAnalyzer::stmt(){
    int errors = 0;
    set<token_type> firsts = {NUMLIT_T, STRLIT_T, SQUOTE_T, IDENT_T, LPAREN_T,EOF_T};
    set<token_type> follows = {RPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, IDENT_T, LPAREN_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }

    if(token == NUMLIT_T){
        p2file << "Using Rule 7" << endl;
        errors += literal();
    }
    else if(token == STRLIT_T){
        p2file << "Using Rule 7" << endl;
        errors += literal();
    }
    else if(token == SQUOTE_T){
        p2file << "Using Rule 7" << endl;
        errors += literal();
    }
    else if(token == IDENT_T){
        p2file << "Using Rule 8" << endl;
        // Token is correctly an IDEN_T so lets consume before moving on
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == LPAREN_T){
        p2file << "Using Rule 9" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += action();
        if(token == RPAREN_T){
            // Its correctly a right paren so consume and get ready to move on.
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
           //  cout << "WE IN IT" << endl;
            token = lex->GetToken();
        }
        else{
            errors++;
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
        }
    }
    else{
        errors++;
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::literal() {
    int errors = 0;
    set<token_type> firsts = {NUMLIT_T, STRLIT_T, SQUOTE_T};
    set<token_type> follows = {RPAREN_T, RBRACK_T, NUMLIT_T, STRLIT_T, SQUOTE_T, IDENT_T, LPAREN_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == NUMLIT_T){
        p2file << "Using Rule 10" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == STRLIT_T){
        p2file << "Using Rule 11" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == SQUOTE_T){
        p2file << "Using Rule 12" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += quotedLit();
    }
    else {
        errors++;
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::quotedLit() {
    int errors = 0;
    set<token_type> firsts=
            {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP1_T, LISTOP2_T, AND_T, OR_T, NOT_T,
             DEFINE_T, LET_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T,
             ROUND_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, SQUOTE_T, COND_T , ELSE_T, EOF_T};
    set<token_type> follows =
            {RPAREN_T, RBRACK_T, NUMLIT_T, STRLIT_T, SQUOTE_T, IDENT_T, LPAREN_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token != RPAREN_T && token != LBRACK_T && token != RBRACK_T && token != ERROR_T && token != EOF_T){
        p2file << "Using Rule 13" << endl;
        errors += anyOtherToken();
    }
    else{
        errors++;
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors += anyOtherToken();
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::moreTokens() {
    int errors = 0;
    //token = lex->GetToken();
    set<token_type> firsts=
            {LPAREN_T, IDENT_T, NUMLIT_T, STRLIT_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP1_T, LISTOP2_T, AND_T, OR_T, NOT_T,
             DEFINE_T, LET_T, NUMBERP_T, LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T,
             ROUND_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, SQUOTE_T, COND_T , ELSE_T, RPAREN_T};
    set<token_type>follows=
            {RPAREN_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token != LBRACK_T && token != RBRACK_T && token != ERROR_T && token != EOF_T){
        if(token == RPAREN_T){
            p2file << "Using Rule 15" << endl;
        }
        else{
            p2file << "Using Rule 14" << endl;
            errors += anyOtherToken();
            errors += moreTokens();
        }
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        errors += anyOtherToken();
        errors += moreTokens();
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::paramList() {
    int errors = 0;
    set<token_type> firsts = {IDENT_T, RPAREN_T, EOF_T};
    set<token_type> follows = {RPAREN_T, EOF_T};
    //cout << "we here5 tok-> " << lex->GetTokenName(token) << endl;
    if(firsts.find(token) == firsts.end()){
        //cout << "we ending up in here?" << endl;
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    //cout << "WE IN PARAM LIST?" << lex->GetTokenName(token) << endl;
    if(token == IDENT_T){
        p2file << "Using Rule 16" << endl;
        // Found correct token so consuming
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += paramList();
    }
    else if(token == RPAREN_T){
        p2file << "Using Rule 17" << endl;
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        errors += paramList();
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::stmtPair(){
    int errors = 0;
    set<token_type> firsts = {LPAREN_T, RPAREN_T, EOF_T};
    set<token_type> follows = {RPAREN_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == LPAREN_T){
        p2file << "Using Rule 20" << endl;
        // Found correct token so consuming
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtPairBody();
    }
    else if(token == RPAREN_T){
        // Found correct token so consuming
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        p2file << "Using Rule 21" << endl;
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

// ***********THIS IS WHERE I STARTED CHANGING WHERE I CALL GET TOKEN ****************************
// This shit seems kinda fucked because you may or may not want to consume a token here, so either
// Need a temp token or a consume flag, prob a flag is the way to go
int SyntacticalAnalyzer::stmtPairBody() {
    int errors = 0;
    set<token_type> firsts = {ELSE_T, IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, EOF_T};
    set<token_type> follows = {RPAREN_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == ELSE_T){
        p2file << "Using Rule 23" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();;
        errors += stmt();
        if(token == RPAREN_T){
            // doo nothing but probably call get token here?
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();;
        }
        else {
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else if(token == IDENT_T){
        p2file << "Using Rule 22" << endl;
        errors += stmt();
        errors += stmt();
        if(token == RPAREN_T){
            token = lex->GetToken();
            errors += stmtPair();
        }
        else{
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else if(token == LPAREN_T){
        p2file << "Using Rule 22" << endl;
        errors += stmt();
        errors += stmt();
        //token = lex->GetToken();
        if(token == RPAREN_T){
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
            errors += stmtPair();
        }
        else{
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else if(token == NUMLIT_T){
        p2file << "Using Rule 22" << endl;
        errors += stmt();
        errors += stmt();
        //token = lex->GetToken();
        if(token == RPAREN_T){
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
            errors += stmtPair();
        }
        else{
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else if(token == STRLIT_T){
        p2file << "Using Rule 22" << endl;
        errors += stmt();
        errors += stmt();
        //token = lex->GetToken();
        if(token == RPAREN_T){
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
            errors += stmtPair();
        }
        else{
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else if(token == SQUOTE_T ){
        p2file << "Using Rule 22" << endl;
        errors += stmt();
        errors += stmt();
        if(token == RPAREN_T){
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
            errors += stmtPair();
        }
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::elsePart() {
    int errors = 0;
    set<token_type> firsts = {IDENT_T, LPAREN_T, NUMLIT_T, STRLIT_T, SQUOTE_T, RPAREN_T, EOF_T};
    set<token_type> follows = {RPAREN_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == IDENT_T){
        p2file << "Using Rule 18" << endl;
        errors += stmt();
    }
    else if(token == LPAREN_T){
        p2file << "Using Rule 18" << endl;
        errors += stmt();
    }
    else if(token == NUMLIT_T){
        p2file << "Using Rule 18" << endl;
        errors += stmt();
    }
    else if(token == STRLIT_T){
        p2file << "Using Rule 18" << endl;
        errors += stmt();
    }
    else if(token == SQUOTE_T){
        p2file << "Using Rule 18" << endl;
        errors += stmt();
    }
    else if(token == RPAREN_T){
        p2file << "Using Rule 19" << endl;
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        errors += stmt();
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::assignsPair() {
    int errors = 0;
    set<token_type> firsts = {LBRACK_T, EOF_T};
    set<token_type> follows = {LBRACK_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == LBRACK_T){
        p2file << "Using Rule 24" << endl;
        token = lex->GetToken();
        if(token == IDENT_T){
            token = lex->GetToken();
            errors += stmt();
            if(token == RBRACK_T){
                token = lex->GetToken();
            }
            else{
                string mess;
                mess += lex->GetLexeme();
                mess += " unexpected";
                lex->ReportError(mess);
                errors++;
            }
        }
        else{
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::moreAssigns() {
    int errors = 0;
    set<token_type> firsts = {LBRACK_T, RPAREN_T, EOF_T};
    set<token_type> follows = {RPAREN_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == LBRACK_T){
        p2file << "Using Rule 25" << endl;
        errors += assignsPair();
        errors += moreAssigns();
    }
    else if(token == RPAREN_T){
        p2file << "Using Rule 26" << endl;
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::action() {
    int errors = 0;
    set<token_type> firsts = {IF_T, COND_T, LET_T, LISTOP2_T, LISTOP1_T, AND_T, OR_T, NOT_T, NUMBERP_T,
                              LISTP_T, ZEROP_T, NULLP_T, STRINGP_T, PLUS_T, MINUS_T, DIV_T, MULT_T, MODULO_T,
                              ROUND_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, IDENT_T, DISPLAY_T, NEWLINE_T, EOF_T};
    set<token_type> follows = {RPAREN_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == IF_T){
        p2file << "Using Rule 27" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
        errors += stmt();
        errors += elsePart();
    }
    else if(token == COND_T){
        p2file << "Using Rule 28" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        //cout << "BEFORE? " << lex->GetTokenName(token) << endl;
        if(token == LPAREN_T){
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
            //cout << "after LPAREN? " << lex->GetTokenName(token) << endl;
            errors += stmtPairBody();
        }
        else{
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else if(token == LET_T){
        p2file << "Using Rule 29" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        if(token == LPAREN_T){
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
            errors += moreAssigns();
            if(token == RPAREN_T){
                //cout << "consuming: " << lex->GetTokenName(token) << endl;
                token = lex->GetToken();
                errors += stmt();
                errors += stmtList();
            }
            else{
                string mess;
                mess += lex->GetLexeme();
                mess += " unexpected";
                lex->ReportError(mess);
                errors++;
            }
        }
        else{
            string mess;
            mess += lex->GetLexeme();
            mess += " unexpected";
            lex->ReportError(mess);
            errors++;
        }
    }
    else if(token == LISTOP1_T){
        p2file << "Using Rule 30" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == LISTOP2_T){
        p2file << "Using Rule 31" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
        errors += stmt();
    }
    else if(token == AND_T){
        p2file << "Using Rule 32" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == OR_T){
        p2file << "Using Rule 33" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == NOT_T){
        p2file << "Using Rule 34" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == NUMBERP_T){
        p2file << "Using Rule 35" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == LISTP_T){
        p2file << "Using Rule 36" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == ZEROP_T){
        p2file << "Using Rule 37" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == NULLP_T){
        p2file << "Using Rule 38" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == STRINGP_T){
        p2file << "Using Rule 39" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == PLUS_T){
        p2file << "Using Rule 40" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == MINUS_T){
        p2file << "Using Rule 41" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
        errors += stmtList();
    }
    else if (token == DIV_T){
        p2file << "Using Rule 42" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
        errors += stmtList();
    }
    else if(token == MULT_T){
        p2file << "Using Rule 43" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == MODULO_T){
        p2file << "Using Rule 44" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
        errors += stmt();
    }
    else if(token == ROUND_T){
        p2file << "Using Rule 45" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == EQUALTO_T){
        p2file << "Using Rule 46" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == GT_T){
        p2file << "Using Rule 47" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == LT_T){
        p2file << "Using Rule 48" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == GTE_T){
        p2file << "Using Rule 49" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == LTE_T){
        p2file << "Using Rule 50" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == IDENT_T){
        p2file << "Using Rule 51" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmtList();
    }
    else if(token == DISPLAY_T){
        p2file << "Using Rule 52" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += stmt();
    }
    else if(token == NEWLINE_T){
        p2file << "Using Rule 53" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
    }
    return errors;
}

int SyntacticalAnalyzer::anyOtherToken() {
    int errors = 0;
    set<token_type> firsts = {LPAREN_T,IDENT_T,NUMLIT_T,STRLIT_T,IF_T,DISPLAY_T,NEWLINE_T,LISTOP1_T,LISTOP2_T,AND_T,OR_T,NOT_T,DEFINE_T,LET_T,
                              NUMBERP_T,LISTP_T,ZEROP_T,NULLP_T,STRINGP_T,PLUS_T,MINUS_T,DIV_T,MULT_T,MODULO_T,ROUND_T,EQUALTO_T,GT_T,LT_T,GTE_T,LTE_T,
                              SQUOTE_T, COND_T, ELSE_T, EOF_T};
    set<token_type> follows = {LPAREN_T,IDENT_T,NUMLIT_T,STRLIT_T,IF_T,DISPLAY_T,NEWLINE_T,LISTOP1_T,LISTOP2_T,AND_T,OR_T,NOT_T,
                               DEFINE_T,LET_T,NUMBERP_T,LISTP_T,ZEROP_T,NULLP_T,STRINGP_T,PLUS_T,MINUS_T,DIV_T,MULT_T,MODULO_T,
                               ROUND_T,EQUALTO_T,GT_T,LT_T,GTE_T,LTE_T,SQUOTE_T,COND_T,ELSE_T,RPAREN_T,RBRACK_T, EOF_T};
    if(firsts.find(token) == firsts.end()){
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        errors++;
        while(firsts.find(token) == firsts.end()){
            token = lex->GetToken();
        }
    }
    if(token == LPAREN_T){
        p2file << "Using Rule 54" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += moreTokens();
        if(token == RPAREN_T){
            //cout << "consuming: " << lex->GetTokenName(token) << endl;
            token = lex->GetToken();
        }
        else{
            errors++;
        }
    }
    else if(token == IDENT_T){
        p2file << "Using Rule 55" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == NUMLIT_T){
        p2file << "Using Rule 56" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == STRLIT_T){
        p2file << "Using Rule 57" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == IF_T){
        p2file << "Using Rule 58" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == DISPLAY_T){
        p2file << "Using Rule 59" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == NEWLINE_T){
        p2file << "Using Rule 60" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == LISTOP1_T){
        p2file << "Using Rule 61" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == LISTOP2_T){
        p2file << "Using Rule 62" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == AND_T){
        p2file << "Using Rule 63" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == OR_T){
        p2file << "Using Rule 64" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == NOT_T){
        p2file << "Using Rule 65" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == DEFINE_T){
        p2file << "Using Rule 66" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == LET_T){
        p2file << "Using Rule 67" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == NUMBERP_T){
        p2file << "Using Rule 68" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == LISTP_T){
        p2file << "Using Rule 69" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == ZEROP_T){
        p2file << "Using Rule 70" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == NULLP_T){
        p2file << "Using Rule 71" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == STRINGP_T){
        p2file << "Using Rule 72" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == PLUS_T){
        p2file << "Using Rule 73" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == MINUS_T){
        p2file << "Using Rule 74" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == DIV_T){
        p2file << "Using Rule 75" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == MULT_T){
        p2file << "Using Rule 76" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == MODULO_T){
        p2file << "Using Rule 77" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == ROUND_T){
        p2file << "Using Rule 78" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == EQUALTO_T){
        p2file << "Using Rule 79" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == GT_T){
        p2file << "Using Rule 80" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == LT_T){
        p2file << "Using Rule 81" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == GTE_T){
        p2file << "Using Rule 82" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == LTE_T){
        p2file << "Using Rule 83" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == SQUOTE_T){
        p2file << "Using Rule 84" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
        errors += anyOtherToken();
    }
    else if(token == COND_T){
        p2file << "Using Rule 85" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else if(token == ELSE_T){
        p2file << "Using Rule 86" << endl;
        //cout << "consuming: " << lex->GetTokenName(token) << endl;
        token = lex->GetToken();
    }
    else{
        string mess;
        mess += lex->GetLexeme();
        mess += " unexpected";
        lex->ReportError(mess);
        while(follows.find(token) == follows.end()){
            token = lex->GetToken();
        }
        errors++;
    }
    return errors;
}
