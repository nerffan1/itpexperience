%{
#include "Node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the program
NProgram* gProgram = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* You should not need to change the union */
%union {
    Node* node;
	NProgram* program;
	NData* data;
	NDecl* decl;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NExpr* expr;
	NComparison* comparison;
	std::string* string;
	int token;
}

%error-verbose

/* Tokens/Terminal symbols */
%token <token> TDATA TMAIN TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TLBRACKET TRBRACKET TINC TDEC TEQUALS
%token <token> TADD TSUB TMUL TDIV
%token <token> TLESS TISEQUAL
%token <token> TVAR TARRAY
%token <token> TIF TELSE TWHILE
%token <token> TCOMMA TPENUP TPENDOWN TSETPOS TSETCOLOR TFWD TBACK TROT
%token <string> TINTEGER TIDENTIFIER

/* Types/non-terminal symbols */
%type <program> program
%type <block> main block 
%type <statement> statement
%type <comparison> comparison
%type <data> data decls
%type <decl> decl;
%type <numeric> numeric
%type <node> node
%type <expr> expr

/* Operator precedence */
%left TADD TSUB
%left TMUL TDIV

%%

program		: data main
				{
					gProgram = new NProgram($1, $2);
				}
;

block		: statement 
				{ 
					$$ = new NBlock();
					$$->AddStatement($1);
				}
			| block statement 
				{ 
					$$->AddStatement($2); 
				}
;

statement	: TIDENTIFIER TEQUALS expr TSEMI 
				{
					$$ = new NAssignVarStmt(*($1),$3);
					std::cout << "Var assignment " << *($1) <<'\n';
				}
			| TIDENTIFIER TLBRACKET expr TRBRACKET TEQUALS expr TSEMI 
				{
					$$ = new NAssignArrayStmt(*($1),$3,$6);
				}
			| TINC TIDENTIFIER TSEMI 
				{
					$$ = new NIncStmt(*($2));
					std::cout << "Increment "<< *($2) << '\n';
				}
			| TDEC TIDENTIFIER TSEMI 
				{
					$$ = new NDecStmt(*($2));
					std::cout << "Decrement "<< *($2) << '\n';
				}
			| TIF comparison TLBRACE block TRBRACE
				{
					$$ = new NIfStmt($2, $4, nullptr);
					std::cout << "If statement " << '\n' ;	
				}
			| TIF comparison TLBRACE block TRBRACE TELSE TLBRACE block TRBRACE
				{
					$$ = new NIfStmt($2, $4, $8);
					std::cout << "If, Else statement " << '\n' ;	
				}
			| TWHILE comparison TLBRACE block TRBRACE
				{
					$$ = new NWhileStmt($2,$4);
					std::cout << "While loop" << '\n' ;	
				}
			| TPENUP TLPAREN TRPAREN TSEMI
				{
					$$ = new NPenUpStmt();
					std::cout << "PenUp() " << '\n' ;	
				}
			| TPENDOWN TLPAREN TRPAREN TSEMI
				{
					$$ = new NPenDownStmt();
					std::cout << "PenDown() " << '\n' ;	
				}
			| TSETPOS TLPAREN expr TCOMMA expr TRPAREN TSEMI
				{
					$$ =  new NSetPosStmt($3,$5);
					std::cout << "SetPos() " << '\n' ;	
				}
			| TSETCOLOR	TLPAREN expr TRPAREN TSEMI
				{
					$$ = new NSetColorStmt($3);
					std::cout << "SetPos() " << '\n' ;	
				}
			| TFWD TLPAREN expr TRPAREN TSEMI
				{
					$$ = new NFwdStmt($3);
					std::cout << "Forward() " << '\n' ;	
				}
			| TBACK TLPAREN expr TRPAREN TSEMI
				{
					$$ = new NBackStmt($3);
					std::cout << "Forward() " << '\n' ;	
				}
			| TROT TLPAREN expr TRPAREN TSEMI
				{
					$$ = new NRotStmt($3);
					std::cout << "Rotate() " << '\n' ;	
				}
;

comparison	: expr TISEQUAL expr 
				{
					$$ = new NComparison($1,$2,$3);
					std::cout << "Comparison" << '\n';
				}
			| expr TLESS expr 
				{
					$$ = new NComparison($1,$2,$3);
					std::cout << "Less than" << '\n';
				}
;

data		: TDATA TLBRACE TRBRACE
				{
					$$ = new NData();
					std::cout << "Data (no decls)\n";
				}
			| TDATA TLBRACE decls TRBRACE
				{
					$$ = $3;
					std::cout << "Data (decls) \n";
				}
;

decls		: decl
				{
					$$ = new NData();
					$$->AddDecl($1);
					std::cout << "Single decl\n";
				}
			| decls decl
				{
					$$->AddDecl($2);
					std::cout << "Multiple decls\n";
				}
;

decl		: TARRAY TIDENTIFIER TLBRACKET numeric TRBRACKET TSEMI
				{
					$$ = new NArrayDecl(*($2), $4);
					std::cout << "Array declaration " << *($2) << '\n';
				}
			| TVAR TIDENTIFIER TSEMI
				{
					$$ = new NVarDecl(*($2));
					std::cout << "Variable Declaration " << *($2) << '\n';
				}
;

main		: TMAIN TLBRACE TRBRACE
				{
					$$ = new NBlock();
					std::cout << "Main (no stmts)\n";
				}
			| TMAIN TLBRACE block TRBRACE
				{
					$$ = $3 ;
					std::cout << "Main (Block)\n";
				}
;

expr		: numeric
				{
					$$ = new NNumericExpr($1);
					std::cout << "Numeric expression\n"; 
				}
			| TIDENTIFIER
				{
					$$ = new NVarExpr(*($1));
				}
			| expr TADD expr 
				{ 
					$$ = new NBinaryExpr($1, $2, $3);
					std:: cout << "Binary Addition" << '\n'; 
				}
			| expr TSUB expr 
				{ 
					$$ = new NBinaryExpr($1, $2, $3);
					std:: cout << "Binary Addition" << '\n'; 
				}
			| expr TMUL expr 
				{ 
					$$ = new NBinaryExpr($1, $2, $3);
					std:: cout << "Binary Addition" << '\n'; 
				}
			| expr TDIV expr 
				{ 
					$$ = new NBinaryExpr($1, $2, $3);
					std:: cout << "Binary Addition" << '\n'; 
				}
			| TLPAREN expr TRPAREN 
				{ 
					$$ = $2;
				}
			| TIDENTIFIER TLBRACKET expr TRBRACKET  
				{
					$$ = new NArrayExpr(*($1), $3);
					std::cout << "Array Access" << '\n';
				}
;

numeric		: TINTEGER
				{
					$$ = new NNumeric(*($1));
					std::cout << "Numeric value of " << *($1) << '\n';
				}
;

node		: program 
				{
					gProgram = new NProgram($1);
				}
			| data  
				{
					gProgram = new NProgram($1, nullptr);
				}
			| main 
				{
					gProgram = new NProgram(nullptr, $1);
				}
;

%%
