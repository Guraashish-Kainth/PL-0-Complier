/*
	This is the lex.c file for the UCF Fall 2021 Systems Software Project.
	For HW2, you must implement the function lexeme *lexanalyzer(char *input).
	You may add as many constants, global variables, and support functions
	as you desire.

	If you choose to alter the printing functions or delete list or lex_index,
	you MUST make a note of that in you readme file, otherwise you will lose
	5 points.
*/


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 500
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

lexeme *list;
int lex_index;

void printlexerror(int type);
void printtokens();

lexeme checkType(char * inputString);
void checkError(lexeme token);

lexeme checkSpecial(char *input, int current, int tokenCounter);

lexeme *lexanalyzer(char *input)
{
    list = malloc(sizeof(lexeme)*MAX_NUMBER_TOKENS);
    //Preparing all of the counters
    int inputCounter = 0;
    int stringCounter = 0;
    int tokenCounter = 0;
    char *inputString = malloc(sizeof(char)*15);
    int inputLength = strlen(input);
    bool isNumber = false;
    bool isString = false;

    //While loop to run through the entire char input array
    while(inputCounter < inputLength){
        if(isdigit(input[inputCounter]) && stringCounter == 0){
            isNumber = true;
        }
        if(isNumber == true){
            if(stringCounter > 5){
                printlexerror(3);
                exit(0);
            }
            if(isalpha(input[inputCounter])){
                printlexerror(2);
                exit(0);
            }
        }

        if(isalpha(input[inputCounter]) && stringCounter == 0){
            isString = true;
        }

        if(isString == true){
            if(stringCounter > 11){
                printlexerror(4);
                exit(0);
            }
        }
        //Check to see if the next char is a space, semicolon, comma, period or newline
        if(input[inputCounter] == ' ' || input[inputCounter] == '\n' || iscntrl(input[inputCounter])){

            //Checking if it is a random space or if it is the indication of a completed string
            if(stringCounter == 0){
                inputCounter++;
            }
            else{
                lexeme newToken;
				newToken = checkType(inputString);
				checkError(newToken);
				list[tokenCounter] = newToken;
				tokenCounter++;
				memset(inputString, '\0', 15);
				stringCounter = 0;
				isString = false;
				isNumber = false;
            }
        }


        //Checking if the next char indicates a comment, skipping to the end of the comment in that case
        else if(input[inputCounter] == '/' && input[inputCounter + 1] == '/'){
            while(!iscntrl(input[inputCounter])){
                inputCounter++;
            }
        }

        else if(!isalpha(input[inputCounter]) && !isdigit(input[inputCounter])){
            if(stringCounter != 0){
                lexeme newToken;
				newToken = checkType(inputString);
				checkError(newToken);
				list[tokenCounter] = newToken;
				tokenCounter++;
				memset(inputString, '\0', 15);
				stringCounter = 0;
				isString = false;
				isNumber = false;
            }
			list[tokenCounter] = checkSpecial(input, inputCounter, tokenCounter);
			inputCounter = inputCounter + strlen(list[tokenCounter].name);
			tokenCounter++;
        }

		//Adding the next char into the string
        else{
            inputString[stringCounter] = input[inputCounter];
            stringCounter++;
            inputCounter++;
        }
        if(inputCounter == inputLength){
            if(stringCounter != 0){
                lexeme newToken;
				newToken = checkType(inputString);
				checkError(newToken);
				list[tokenCounter] = newToken;
				tokenCounter++;
				memset(inputString, '\0', 15);
				stringCounter = 0;
				isString = false;
				isNumber = false;
            }
        }
    }

	//remove later
	lex_index = tokenCounter;
	printtokens();
	return list;
}

lexeme checkType(char *inputString)
{
	lexeme token;
	int alphaCounter = 0;
	strcpy(token.name, inputString);
	if(!strcmp(inputString, "const"))
	{
		token.type = constsym;
		token.value = 1;
	}
	else if(!strcmp(inputString, "var"))
	{
		token.type = varsym;
		token.value = 2;
	}
	else if(!strcmp(inputString, "procedure"))
	{
		token.type = procsym;
		token.value = 3;
	}
	else if(!strcmp(inputString, "begin"))
	{
		token.type = beginsym;
		token.value = 4;
	}
	else if(!strcmp(inputString, "end"))
	{
		token.type = endsym;
		token.value = 5;
	}
	else if(!strcmp(inputString, "while"))
	{
		token.type = whilesym;
		token.value = 6;
	}
	else if(!strcmp(inputString, "do"))
	{
		token.type = dosym;
		token.value = 7;
	}
	else if(!strcmp(inputString, "if"))
	{
		token.type = ifsym;
		token.value = 8;
	}
	else if(!strcmp(inputString, "then"))
	{
		token.type = thensym;
		token.value = 9;
	}
	else if(!strcmp(inputString, "else"))
	{
		token.type = elsesym;
		token.value = 10;
	}
	else if(!strcmp(inputString, "call"))
	{
		token.type = callsym;
		token.value = 11;
	}
	else if(!strcmp(inputString, "write"))
	{
		token.type = writesym;
		token.value = 12;
	}
	else if(!strcmp(inputString, "read"))
	{
		token.type = readsym;
		token.value = 13;
	}
	else if(!strcmp(inputString, "odd"))
	{
		token.type = oddsym;
		token.value = 28;
	}
	else
	{
		if(isalpha(token.name[0])){
            token.type = identsym;
            token.value = 14;
		}
		else{
            token.type = numbersym;
            token.value = 15;
		}
	}
	return token;
}

void checkError(lexeme token)
{
    int length = strlen(token.name);
    if(token.type == identsym)
    {
        if(isalpha(token.name[0]))
        {
            if(length > 11)
            {
                printlexerror(4);
                exit(0);
            }
        }

        else
        {
            printlexerror(2);
            exit(0);
        }
    }
    else if(token.type == numbersym)
    {
        if(length > 5)
        {
            printlexerror(3);
            exit(0);
        }

        for (int i = 0; i < length; i++)
        {
            if(isalpha(token.name[i]))
            {
                printlexerror(2);
                exit(0);
            }
        }
    }
}

void printtokens()
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++)
	{
		switch (list[i].type)
		{
			case oddsym:
				printf("%11s\t%d", "odd", oddsym);
				break;
				case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case modsym:
				printf("%11s\t%d", "%", modsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case addsym:
				printf("%11s\t%d", "+", addsym);
				break;
			case subsym:
				printf("%11s\t%d", "-", subsym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11s\t%d", list[i].name, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 0; i < lex_index; i++)
	{
		if (list[i].type == numbersym)
			printf("%d %s ", numbersym, list[i].name);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
	list[lex_index++].type = -1;
}

void printlexerror(int type)
{
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Excessive Number Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Excessive Identifier Length\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");

	free(list);
	return;
}

lexeme checkSpecial(char *input, int current, int tokenCounter){
    lexeme newToken;
    if(input[current] == '='){
        if(input[current + 1] == '='){
            strcpy(newToken.name, "==");
            newToken.type = eqlsym;
            newToken.value = 22;
            return newToken;
        }
        else{
            printlexerror(1);
            exit(0);
        }
    }

    else if(input[current] == '!'){
        if(input[current + 1] == '='){
            strcpy(newToken.name, "!=");
            newToken.type = neqsym;
            newToken.value = 23;
            return newToken;
        }
        else{
            printlexerror(1);
            exit(0);
        }
    }

  else if(input[current] == ':'){
            if(input[current + 1] == '='){
                strcpy(newToken.name, ":=");
                newToken.type = assignsym;
                newToken.value = 16;
                return newToken;
            }
            else{
                printlexerror(1);
                exit(0);
            }
  }
  else if(input[current] == '+'){
            strcpy(newToken.name, "+");
            newToken.type = addsym;
            newToken.value = 17;
            return newToken;
   }

  else if(input[current] == '-'){
            strcpy(newToken.name, "-");
            newToken.type = subsym;
            newToken.value = 18;
            return newToken;
   }

  else if(input[current] == '*'){

            strcpy(newToken.name, "*");
            newToken.type = multsym;
            newToken.value = 19;
            return newToken;

   }

  else if(input[current] == '/'){
            strcpy(newToken.name, "/");
            newToken.type = divsym;
            newToken.value = 20;
            return newToken;
   }

  else if(input[current] == '%'){
            strcpy(newToken.name, "%");
            newToken.type = modsym;
            newToken.value = 21;
            return newToken;
   }

  else if(input[current] == '<'){
            if(input[current + 1] == '='){
            	strcpy(newToken.name, "<=");
            	newToken.type = leqsym;
            	newToken.value = 25;
            	return newToken;
	    }
	    else{
                strcpy(newToken.name, "<");
            	newToken.type = lsssym;
            	newToken.value = 24;
            	return newToken;
	    }
   }

   else if(input[current] == '>'){
            if(input[current + 1] == '='){
            	strcpy(newToken.name, ">=");
            	newToken.type = geqsym;
            	newToken.value = 27;
            	return newToken;
            }
        else{
	    strcpy(newToken.name, ">");
            newToken.type = gtrsym;
            newToken.value = 26;
            return newToken;
        }
   }

  else if(input[current] == '('){
            strcpy(newToken.name, "(");
            newToken.type = lparensym;
            newToken.value = 29;
            return newToken;
        }

   else if(input[current] == ')'){
            strcpy(newToken.name, ")");
            newToken.type = rparensym;
            newToken.value = 30;
            return newToken;
   }


   else if(input[current] == ','){
            strcpy(newToken.name, ",");
            newToken.type = commasym;
            newToken.value = 31;
            return newToken;
        }

  else if(input[current] == '.'){
            strcpy(newToken.name, ".");
            newToken.type = periodsym;
            newToken.value = 32;
            return newToken;
        }

  else if(input[current] == ';'){
            strcpy(newToken.name, ";");
            newToken.type = semicolonsym;
            newToken.value = 33;
            return newToken;
        }

  else {
    printlexerror(1);
    exit(-1);
  }

}
