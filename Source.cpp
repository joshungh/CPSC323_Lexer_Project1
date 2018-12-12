// CS323_ICG.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
//simple lexer assignment
//Derek Dorr, Jonathan Ungheanu, Adam Weesner
//CPSC 323 Fall 2018
//Shohrat Geldiyev
//10/1/18

/*****************************************************************************
To use this program, you just need to put a text file in the project directory
and when the program starts it will ask you to input the file name.
the rest is done automatically!
*****************************************************************************/

#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

enum FSM_TRANSITIONS {
	REJECT = 0,
	STRING,
	SEPARATOR,
	SPACE,
	OPERATOR,
	REAL
};

struct TokenType {
	string token;
	int lexeme;
	string lexemeName;
};

//Function Prototypes
int Get_FSM_Col(char currentChar);
string GetLexemeName(int lexeme);
vector<TokenType> Lexer(string text);
bool isKeyword(string token);
void codeGen(vector<TokenType> tokenVec);


//							   string,    separator,  space,   operator,  real
int stateTable[][6] = { { 0,    STRING,    SEPARATOR,  SPACE,   OPERATOR,  REAL },
{ STRING,    STRING,    REJECT,     REJECT,  REJECT,    REJECT },
{ SEPARATOR, REJECT,    REJECT,     REJECT,  REJECT,    REJECT },
{ SPACE,     REJECT,    REJECT,     REJECT,  REJECT,    REJECT },
{ OPERATOR,  REJECT,    REJECT,     REJECT,  REJECT,    REJECT },
{ REAL,      REJECT,    REJECT,     REJECT,  REJECT,    REAL } };

//DICTIONARY
const int DICSIZE = 10;
string keywd[DICSIZE] = { "while", "get", "int", "put", "if", "else", "endif", "return", "print", "end" };

int quoteCount = 0;

int main() {
	vector<TokenType>tokenVec;
	TokenType tokens;
	fstream inFile;
	string text = "";
	string fileName;

	cout << "Enter your file name: ";
	cin >> fileName;
	cout << endl;

	inFile.open(fileName);
	if (inFile.fail()) {
		cout << "\nUNABLE TO OPEN FILE " << fileName << endl;
		exit(1);
	}
	cout << "TOKEN\t\tLEXEME\n";
	while (getline(inFile, text)) {
		tokenVec = Lexer(text);
		//Cheats the FSM and re-assigns 'strings' to pre defined lexemes
		//maybe if i made the vector global i can do this in get_fsm_col...
		cout << "\nSIZE: " << tokenVec.size() << endl;
		for (unsigned j = 0; j < tokenVec.size(); j++) {

			if (isKeyword(tokenVec[j].token) == true) {
				tokenVec[j].lexemeName = "KEYWORD";
			}//send to find keyword, if not keyword then its an identifier, if not then its a string

			if (tokenVec[j].token[0] == 34) {
				quoteCount++;
			}
			if (quoteCount == 1 || quoteCount == 2) {
				tokenVec[j].lexemeName = "STRING";
				if (quoteCount == 2) {
					quoteCount = 0;
				}
			}


			cout << tokenVec[j].lexemeName << "\t\t" << tokenVec[j].token << endl;
		}
	}
	inFile.close();

	/*Every character read makes the program check to see
	what type of char it is and changes states accordingly*/

	codeGen(tokenVec);

	return 0;
}

//The Lexer, parses thru the text file and finds tokens and updates the state of the FSM
vector<TokenType> Lexer(string text) {
	TokenType access;
	vector<TokenType> myTokens;
	string currentToken = "";
	int currentState = REJECT;
	int prevState = REJECT;
	int col = REJECT;
	char currentChar = ' ';

	for (unsigned i = 0; i < text.size();) {
		currentChar = text[i];

		col = Get_FSM_Col(currentChar);

		currentState = stateTable[currentState][col];

		if (currentState == REJECT) {
			if (prevState != SPACE) {
				access.token = currentToken;
				access.lexeme = prevState;
				access.lexemeName = GetLexemeName(access.lexeme);
				myTokens.push_back(access);
			}
			currentToken = "";
		}
		else {
			currentToken += currentChar;
			++i;
		}
		prevState = currentState;
	}
	if (currentState != SPACE && currentToken != "") {
		access.token = currentToken;
		access.lexeme = currentState;
		access.lexemeName = GetLexemeName(access.lexeme);
		myTokens.push_back(access);
	}

	return myTokens;
}

//recieves each individual character, decides what type it is and returns
//a value that switches the machine's state accordingly
int Get_FSM_Col(char currentChar) {
	int value = currentChar;

	if (isspace(currentChar)) {
		return SPACE;
	}
	else if (isalpha(currentChar) || value == 36) {
		return STRING;
	}
	else if (value == 34 || value == 40 || value == 41 || value == 125 || value == 123 || value == 44 || value == 37 || value == 59 || value == 58 || value == 91 || value == 93) {
		return SEPARATOR;
	}
	else if (value == 60 || value == 61 || value == 62 || value == 42 || value == 43 || value == 45 || value == 47) {
		return OPERATOR;
	}
	else if (isdigit(currentChar)) {
		return REAL;
	}
	else {//may need to remove else statement...
		return REJECT;
	}
}

//Converts the enumerated lexeme into a string then returns the string
string GetLexemeName(int lexeme) {
	switch (lexeme) {
	case STRING://changing this to identifier , will double for loop the vector and chars to find quotes for strings...
		return "IDENTIFIER";
		break;
	case SEPARATOR:
		return "SEPARATOR";
		break;
	case SPACE:
		return "SPACE";
		break;
	case OPERATOR:
		return "OPERATOR";
		break;
	case REAL:
		return "REAL";
		break;
	default:
		return "ERROR";
		break;
	}
}

bool isKeyword(string token) {
	for (int i = 0; i < DICSIZE; i++) {
		if (token == keywd[i]) {
			return true;
		}
	}
	return false;
}
//representing sample code as assembly to outfile
void codeGen(vector<TokenType>tokenVec) {
	ofstream outCode;
	outCode.open("outCode.txt");
	unsigned int j = 0;
	for (unsigned i = 0; i < tokenVec.size(); i++) {
		j++;
		//assigning values to vars
		if (tokenVec[i].lexemeName == "IDENTIFIER") {
			if (tokenVec[j].token == "=") {
				if (tokenVec[j + 1].lexemeName == "IDENTIFIER") {
					if (tokenVec[j + 2].lexemeName == "OPERATOR") {
						if (tokenVec[j + 3].lexemeName == "IDENTIFIER" || tokenVec[j + 3].lexemeName == "REAL") {
							string assTest = tokenVec[j + 2].token;
							//add
							if (assTest == "+") {
								outCode << "MOV ax, " << tokenVec[j + 1].token << "\n";
								outCode << "ADD ax, " << tokenVec[j + 3].token << "\n";
								outCode << "MOV " << tokenVec[i].token << ", ax\n";		//still need to find a way to see if ax,bx,cx,dx are taken.
							}
						}
					}
				}
				else if (tokenVec[i].lexemeName == "IDENTIFIER") {
					if (tokenVec[j].token == "=") {
						if (tokenVec[j + 1].lexemeName == "REAL") {
							if (tokenVec[j + 2].lexemeName == "SEPARATOR") {
								outCode << "->MOV ax, " << tokenVec[j + 1].token << "\n";
								outCode << "->MOV " << tokenVec[i].token << ", ax\n";
							}
						}
					}
				}
			}
		}
		else {
			outCode << "\nERROR\n" << tokenVec[i].token << endl;
		}
	}
	outCode.close();
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
