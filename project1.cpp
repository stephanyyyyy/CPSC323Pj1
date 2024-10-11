// project1
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <algorithm>
using namespace std;

//function prototypes
void isIdentifier(string currLine, vector<string> &identifierVec, int &totalTokens);
void isKeyword(string currLine, vector<string> &keywordVec, int &totalTokens);
void isOperator(string currLine,  vector<string> &operatorVec, int &totalTokens);
void isSeparator(string currLine, vector<char> &separatorVec, int &totalTokens);
void isLiteralString(string currLine, vector<string> &literalVec, int &totalTokens);
void isNumLiteral(string currLine, vector<string> &literalVec, int &totalTokens);
void isWhitespace(string currLine,  vector<string> &whitespaceVec,  int &totalTokens);
void displayStr(ofstream &table, vector<string> vec);
void displayChar(ofstream &table, vector<char> vec);

// main function
int main()
{
	string				line;
	int					numTokens = 0;
	vector<string>		keywords;
	vector<string>		identifiers;
	vector<char>		separators;
	vector<string>		operators;
	vector<string>		literals;
	vector<string>		whitespace;
	vector<string>		comments;
	ifstream			test;
	ofstream			outputTest;
	ifstream			cleanCode;;
	ofstream			tokenTbl;
	string				codeLine;
	
	// out test file
	test.open("test.txt");

	// check if file is open
	if (test.fail())
	{
		cout << "Error opening file...\n";
		exit(EXIT_FAILURE);
	}

	// open output file
	outputTest.open("outputTest.txt");

	// check if output file is open
	if (outputTest.is_open() == false)
	{
		cout << "Error opening file...\n";
		exit(EXIT_FAILURE);
	}


	while (getline(test, line))
	{
		if ((line.find("//") != string::npos))
		{
			comments.push_back(line);
			continue;
		}

		if(line.empty())
		{
			continue;
		}

		size_t firstChar = line.find_first_not_of(" \t");

		if (firstChar != string::npos)
		{
			line.erase(0, firstChar);
		}
	
		outputTest << line << endl;
	}
	

	test.close();
	outputTest.close();

	cleanCode.open("outputTest.txt");

	// check if file is open
	if (cleanCode.fail())
	{
		cout << "Error opening file...\n";
		exit(EXIT_FAILURE);
	}

		// open output file
	tokenTbl.open("table.txt");

	// check if output file is open
	if (tokenTbl.is_open() == false)
	{
		cout << "Error opening file...\n";
		exit(EXIT_FAILURE);
	}
	
	while (getline(cleanCode, codeLine))
	{
		isKeyword(codeLine, keywords, numTokens);
		isIdentifier(codeLine, identifiers, numTokens);
		isOperator(codeLine, operators, numTokens);
		isSeparator(codeLine, separators, numTokens);
		isLiteralString(codeLine, literals, numTokens);
		isNumLiteral(codeLine, literals, numTokens);
		isWhitespace(codeLine, whitespace, numTokens);
	}

	
	tokenTbl << "Category\t\t\tTokens\n";
	if (!keywords.empty())
	{
		tokenTbl << "Keywords\t\t\t";
		displayStr(tokenTbl, keywords);
	}
	if (!identifiers.empty())
	{
		tokenTbl << "Identifiers\t\t\t";
		displayStr(tokenTbl, identifiers);
	}
	if (!separators.empty())
	{
		tokenTbl << "Separators\t\t\t";
		displayChar(tokenTbl, separators);
	}
	if (!operators.empty())
	{
		tokenTbl << "Operators\t\t\t";
		displayStr(tokenTbl, operators);
	}
	if (!literals.empty())
	{
		tokenTbl << "Literals\t\t\t";
		displayStr(tokenTbl, literals);
	}
	if (!whitespace.empty())
	{
		tokenTbl << "Whitespace\t\t\t";
		displayStr(tokenTbl, whitespace);
	}
	if (!comments.empty())
	{
		tokenTbl << "Comments\t\t\t";
		displayStr(tokenTbl, comments);
	}
	tokenTbl << "\nTotal number of tokens: " << numTokens << endl;


	// close files
	cleanCode.close();
	tokenTbl.close();
	
	
	return 0;
}

void isKeyword(string currLine, vector<string> &keywordVec, int &totalTokens)
{
	vector<string>	keywords = {"int", "char", "float", "double", "string", "for", "if", "else", "cout", 
								"cin", "return", "bool", "while", "do", "true", "false", "void"};
	istringstream	inputstrStream(currLine);
	string			word;

	while (inputstrStream >> word)
	{
		// had to hardcode endl and main
		if (word == "endl;")
		{
			word = word.substr(0, word.size() - 1);
			keywordVec.push_back(word);
			++totalTokens;
		}

		if (word == "main()")
		{
			word = word.substr(0, word.size() - 2);
			keywordVec.push_back(word);
			++totalTokens;
		}

		if (word == "(int")
		{
			word = word.substr(word.size() + 1, word.size());
			keywordVec.push_back(word);
			++totalTokens;
		}
		
		for (unsigned int i = 0; i < keywords.size(); ++i)
		{
			
			if (word == keywords[i])
			{
				keywordVec.push_back(word);
				++totalTokens;
			}

		}
	}
}

	
void isIdentifier(string currLine, vector<string> &identifierVec, int &totalTokens)
{
	vector<string>	keywords = {"int", "char", "float", "double", "string", "for", "if", "else", "main", "cout", 
								"cin", "return", "bool", "while", "do", "true", "false", "endl", "void"};
	vector<char>	separators = {'(', ')', '[', ']', '{', '}',';', ','};

	for (size_t i = 0; i < currLine.size(); ++i)
	{
		char c = currLine[i];

		bool separator = false;

		for (size_t j = 0; j < separators.size(); ++j)
		{
			if (c == separators[j])
			{
				separator = true;
				break;
			}
		}

		if (separator)
		{
			currLine[i] = ' ';
		}
	}
	
	istringstream	inputstrStream(currLine);
	string 			word;
	while(inputstrStream >> word)
	{
		if (find(keywords.begin(), keywords.end(), word) != keywords.end())
		{
			continue;

		}

		if ((isalpha(word[0]) || word[0] == '_')) 
		{
			bool identifier = true;
			for (size_t i = 1; i < word.size(); ++i) 
			{
			 	if (!isalnum(word[i]) && word[i] != '_') 
			 	{
			    	identifier = false;
			        break;
				}
			}

			if (identifier == true) 
			{
                identifierVec.push_back(word);
                ++totalTokens;
            }
         }
	}
}

void isOperator(string currLine,  vector<string> &operatorVec, int &totalTokens)
{
	vector<string> operators = {"+", "-", "*", "/", "%", "++", "--", "<", ">", "<=", ">=", "&&", "||",
								"<<", ">>", "!=", "=", "=="};

	istringstream	inputstrStream(currLine);
	string			word;

	while (inputstrStream >> word)
	{
		for (unsigned int i = 0; i < operators.size(); ++i)
		{
			if (word == operators[i])
			{
				operatorVec.push_back(word);
				++totalTokens;
			}
		}
	}
}

void isSeparator(string currLine, vector<char> &separatorVec, int &totalTokens)
{
	vector<char>	separators = {'(', ')', '[', ']', '{', '}',';', ','};
	istringstream	inputstrStream(currLine);
	char 			nextChar;

	while (inputstrStream.get(nextChar))
	{
		for (unsigned int i = 0; i < separators.size(); ++i)
		{
			if (nextChar == separators[i])
			{
				separatorVec.push_back(nextChar);
				++totalTokens;
			}
		}	
	}
}

void isLiteralString(string currLine, vector<string> &literalVec, int &totalTokens)
{
	size_t firstQuote = 0;
	string statement;

	if ((firstQuote = currLine.find('"')) == string::npos)
	{
		return;
	}
	else
	{
		size_t secQuote = currLine.find('"', firstQuote + 1);
		if (secQuote == string::npos)
		{
			return;
		}
		statement = currLine.substr(firstQuote + 1,  secQuote - firstQuote -1);
		literalVec.push_back(statement);
		++totalTokens;

		firstQuote = secQuote + 1;
	} 
	
}

void isNumLiteral(string currLine, vector<string> &literalVec, int &totalTokens)
{
	istringstream	inputstrStream(currLine);
	string			num;

	while (inputstrStream >> num)
	{
		if (isdigit(num[0]))
		{	
			if (ispunct(num.back()))
			{
				num = num.substr(0, num.size() - 1);
				literalVec.push_back(num);
				++totalTokens;
			}
		}
	}
}

void isWhitespace(string currLine,  vector<string> &whitespaceVec, int &totalTokens)
{
	char 			nextChar;

	for (size_t i = 0; i < currLine.size(); i++)
	{
		nextChar = currLine[i];
		if ((isspace(nextChar)) && (nextChar != ' '))
		{
			whitespaceVec.push_back(string(1,nextChar));
			++totalTokens;
		}
	
	}		
	
}


void displayStr(ofstream &table, vector<string> vec)
{
	unsigned int i;

	for (i = 0; i < vec.size(); ++i)
	{
		if (i == vec.size() - 1)
		{
			table << vec[i] << endl;
		}
		else
		{
			table << vec[i] << ", ";
		}
	}

}

void displayChar(ofstream &table, vector<char> vec)
{
    unsigned int i;

	for (i = 0; i < vec.size(); ++i)
	{
		if (i == vec.size() - 1)
		{
			table << vec[i] << endl;;
		}
		else
		{
			table << vec[i] << ", ";
		}
	}
}
