#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	nulsym = 1, identSym, numberSym, plusSym, minusSym, multSym, slashSym, oddSym, eqlSym, neqSym, lesSym, leqSym, 
	gtrSym, geqSym, lparentSym, rparentSym, commaSym, semicolonSym, periodSym, becomesSym, beginSym, endSym, ifSym, thenSym,
	whileSym, doSym, callSym, constSym, varSym, procSym, writeSym, readSym , elseSym
}Lex;

typedef enum
{
	LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, OUT, IN
}OP;

typedef enum
{
	RET = 0, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
}M;



typedef symbol
{
	int type; //const = 1, var = 2, procedure = 3
	int level;
	int value;
	int address; //M
	char name[12];
	
}symbol;

typedef struct
{
	int op;
	int l;
	int m;
} instruction;

//Globals
int token = 0;
FILE* lexemeListFile = fopen ("lexemelist.txt", "r");
FILE* mCode = fopen ("mCode.txt", "w");

symbol symbolTable[100];
int symbolTableIndex = 0

instruction code[2000];
int codeIndex = 0;

int level = -1;

//declarations
void statement();
void getToken();
void genCode(int op, int l, int m);
void block();
void program();
void error(int error);
void condition();
void expression();
void term();
void factor();
void genCode(int op, int l, int m);
void addSymbol(int type, int level, int value, int address, char name[12]);
int findSymbol(char name[12]);


//fetches next token
void getToken()
{
	fscanf(lexemeListFile, "%d", CurrentToken)
}

//adds a line of machine code
void genCode(int op, int l, int m)
{
	code[codeIndex].op = op;
	code[codeIndex].l = l;
	code[codeIndex].m = m;
	codeIndex++;
}

//adds a symbol to the table
void addSymbol(int type, int level, int value, int address, char name[12])
{
	symbolTable[symbolTableIndex].type = type;
	symbolTable[symbolTableIndex].level = level;
	symbolTable[symbolTableIndex].value = value;
	symbolTable[symbolTableIndex].address = address;
	symbolTable[symbolTableIndex].name = name;
	symbolTableIndex++;
}

//returns the index of a symbol from the table
int findSymbol(char name[12])
{
	int i = 0;
	for(i=0; i<=symbolTableIndex; i++)
	{
		if(strcmp(symbolTable[i].name,name) == 0)
			return(i);
	}
	error(15);
}



void factor()
{
	if(token == identSym)
	{
		getToken();
	}
	else if(token == numberSym)
	{
		getToken();
	}
	else if(token == lparentSym)
	{
		getToken();
		expression();
		if(token != rparentSym) error(22);
		getToken();
	}
	else
	{
		error(21);
	}
}

void term()
{
	factor();
	while(token == multSym || token == slashSym)
	{
		getToken();
		factor();
	}
}

void expression()
{
	if(token == plusSym || token == minusSym)
	{
		getToken();
		term();
	}
	else
	{
		term();
	}
	while(token == plusSym || token == minusSym)
	{
		getToken();
		term();
	}
}

void condition()
{
	if(token == oddSym)
	{
		getToken();
		expression();
	}
	else
	{
		expression();
		if(Token != eqlSym || Token != neqSym || Token != lssSym || Token != leqSym || Token != gtrSym || Token != geqSym ||) error(20);
		getToken();
		expression();
	}
}

void statement()
{
	if(token == identSym)
	{
		getToken();
		if(token != becomesSym) error(3);
		getToken();
		expression();
	}
	
	else if(token == callSym)
	{
		getToken();
		if(token != identSym) error(14);
		getToken();
	}
	
	else if(token == beginSym)
	{
		getToken();
		statement();
		while(token = semicolonSym)
		{
			getToken();
			statement();
		}
		if(token != endSym) error(8);
		getToken();
	}
	
	else if(token == whileSym)
	{
		getToken();
		condition();
		if(token != doSym) error(18);
		getToken();
		statement();
	}
	
	else if(token == writeSym)
	{
		getToken();
		if(token != identSym) error(11);
		getToken();
	}
	
	else if(token == readSym)
	{
		getToken();
		if(token != identSym) error(11);
		getToken();
	}
}

void block()
{
	int ident, val, type;
	int varCount = 0;
	int constCount = 0;
	int procCount = 0;
	
	level ++;
	
	if(token == constSym)
	{
		type = 1;
		do
		{
			getToken();
			if(token != identSym) error(4);
			getToken();
			if(token != eqlSym) error(3);
			getToken()
			if(token != numberSym) error(2);
			//put into symbol table
			constCount++;
		}
		while(token != commaSym);
			
		if(token != semicolonSym) error(5);
		getToken();
	}
	
	if(token == varSym)
	{
		type = 2;
		do
		{
			getToken();
			if(token != identSym) error(4);
			//put into symbol table
			varCount++;
			getToken();
		}
		while(token != commaSym);
		
		if(token != semicolonSym) error(5);
		getToken();
	}
	
	// add code for LIT M = varCount + constCount
	
	while(token == procSym)
	{
		type = 3;
		getToken();
		if(token != identSym) error(4);
		getToken();
		if(token != semicolonSym) error(5);
		getToken();
		block();
		if(token != semicolonSym) error(5);
		//put into symbol table
		procCount++;
	}
	statement();
}

void program()
{
	getToken();
	block();
	if(token != periodSym)
	{
		error(9);
	}
	else
	{
		genCode(0,0,3); //halt program
	}
}



//prints errors
void error(int error)
{
	printf("Error ");
	switch (error)
	{	
		case 1:
			printf("1. Use = instead of :=.");
			break;
		case 2:
			printf("2. = must be followed by a number.");
			break;
		case 3:
			printf("3. Identifier must be followed by =.");
			break;
		case 4:
			printf("4. const, var, procedure must be followed by identifier.");
			break;
		case 5:
			printf("5. Semicolon or comma missing.");
			break;
		case 6:
			printf("6. Incorrect symbol after procedure declaration.");
			break;
		case 7:
			printf("7. Statement expected.");
			break;
		case 8:
			printf("8. Incorrect symbol after statement part in block.");
			break;
		case 9:
			printf("9. Period expected.");
			break;
		case 10:
			printf("10. Semicolon between statements missing.");
			break;
		case 11:
			printf("11. Undeclared identifier.");
			break;
		case 12:
			printf("12. Assignment to constant or procedure is not allowed.");
			break;
		case 13:
			printf("13. Assignment operator expected.");
			break;
		case 14:
			printf("14. call must be followed by an identifier.");
			break;
		case 15:
			printf("15. Call of a constant or variable is meaningless.");
			break;
		case 16:
			printf("16. then expected.");
			break;
		case 17:
			printf("17. Semicolon or } expected.");
			break;
		case 18:
			printf("18. do expected.");
			break;
		case 19:
			printf("19. Incorrect symbol following statement.");
			break;
		case 20:
			printf("20. Relational operator expected.");
			break;
		case 21:
			printf("21. Expression must not contain a procedure identifier.");
			break;
		case 22:
			printf("22. Right parenthesis missing.");
			break;
		case 23:
			printf("23. The preceding factor cannot begin with this symbol.");
			break;
		case 24:
			printf("24. An expression cannot begin with this symbol.");
			break;
		case 25:
			printf("25. This number is too large.");
			break;
	}
	 printf( "\n" );
	exit(0);
} 


int main()
{
	
}

