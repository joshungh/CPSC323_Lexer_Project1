/* CPSC 323 - Lexicon
 * Team: Adam Weesner
 *
 * INSTRUCTIONS:
 * 		Write code on sourceCode.txt
 * 		Use keywords "PRINT" and "END".
 * 		Print allows the user to print an enclosed message to console.
 * 		Message must be surrounded by quotations.
 * 		END ends the input.
 */

#include <iomanip>
#include <iostream>
#include <fstream>
#include <list>
using namespace std;

int main() {

	// Initialization
	char inputChar;
	string inputStr = "";
	string output = "";
	string temp = "";
	bool isLiteral = true;

	// File opening
	ifstream input;
	input.open("sourceCode.txt");

	// Tokens lists
	string keywords[] = { "PRINT", "END" };
	char seperators[] = { '\"' };

	cout << "INSTRUCTIONS:\n\t"
			"Write code on sourceCode.txt\n\t"
			"Use keywords \"PRINT\" and \"END\"\n\t"
			"Print allows the user to print an enclosed message to console.\n\t"
			"Message must be surrounded by quotations.\n\t"
			"END ends the input.\n\n";

	while (getline(input, temp)) {
		for (unsigned int j = 0; j < temp.length(); j++) {

			inputChar = temp[j];
			inputStr += inputChar;

			// Keywords
			for (unsigned int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
				if (inputStr == keywords[i]) {
					output += "Keyword		" + keywords[i] + "\n";
					inputStr = "";
				}
			}

			// Seperators
			for (unsigned int i = 0; i < sizeof(seperators)/sizeof(seperators[0]); i++) {
				// Fist encounter of quotation
				if (isLiteral && inputChar == seperators[i]) {
					output += "Seperators	";
					output += seperators[i];
					output += "\n";
					inputStr = "";
					isLiteral = false;
				} else if (inputChar == seperators[i]) {
					output += "Literal		" + inputStr.substr(0, inputStr.size()-1) + "\nSeperators	" + seperators[i] + "\n";
					cout << "Output: " << inputStr.substr(0, inputStr.size()-1) << "\n\n";
					inputStr = "";
					isLiteral = true;
				}
			}
		}
	}

	cout << "TOKEN		LEXEME\n" << output;

	// File closing
	input.close();

	return 0;
}
