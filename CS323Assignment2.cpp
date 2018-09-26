//simple lexer assignment 
//Derek Dorr
//CPSC 323 Fall 2018
//Shohrat Geldiyev
//9/11/18

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


//							   string,    separator,  space,   operator,  real
int stateTable[][6] = { {0,    STRING,    SEPARATOR,  SPACE,   OPERATOR,  REAL},
				   {STRING,    STRING,    REJECT,     REJECT,  REJECT,    REJECT},
				   {SEPARATOR, REJECT,    REJECT,     REJECT,  REJECT,    REJECT},
				   {SPACE,     REJECT,    REJECT,     REJECT,  REJECT,    REJECT},
				   {OPERATOR,  REJECT,    REJECT,     REJECT,  REJECT,    REJECT},
				   {REAL,      REJECT,    REAL,     REJECT,  REJECT,      REAL } };


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
		for (unsigned j = 0; j < tokenVec.size(); j++) {
			if (tokenVec[j].token == "while") {
				tokenVec[j].lexemeName = "KEYWORD";
			}
			else if (tokenVec[j].token == "fahr" || tokenVec[j].token == "upper" || tokenVec[j].token == "a") {
				tokenVec[j].lexemeName == "IDENTIFIER";
			}
			cout << tokenVec[j].lexemeName << "\t\t" << tokenVec[j].token << endl;
		}
	}
	inFile.close();

	/*Every character read makes the program check to see
	what type of char it is and changes states accordingly*/

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
	else if (isalpha(currentChar)) {
		return STRING;
	}
	else if (value == 40 || value == 41 || value == 46) {
		return SEPARATOR;
	}
	else if (value == 60 || value == 61) {
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
	case STRING:
		return "STRING";
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
