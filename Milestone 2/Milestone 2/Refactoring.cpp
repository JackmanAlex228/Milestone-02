// Refactoring.cpp : Defines the entry point for the console application.
/*
I/O:
READ        10    read into memory
Write       11
Load/store operations
LOAD        20    load from memory location into accumulator
STORE       21
Arithmetic operation
ADD         30    number in memory location + number in accumulator (keep result in accumulator)
SUB         31
DIV         32
MULT        33
Control operations
BRANCH      40    branch into specific location in memory
BRANCHNEG   41
BRANCHZERO  42
HALT        43
Debugging opreations
MEMDUMP            print the memory image on the screen
BREAK
CONTINUE
Terminate        -99999
*/

//You may have to comment this line if you're using MacOS.
#include "stdafx.h"

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <array>
#include <iomanip>
#include <cstdlib>


using namespace std;

#define MIM_DIGITS 4
#define MAX_DIGITS 6
#define NUM_ZERO 0
#define MEMORY_SIZE 1000
#define TERMINATE -99999


int calculateOperations(int command, int location);
string fixMyString(string rawValue);
string fixMyString(int rawValue);
int instructCount = 0;
int accumulator = 0;

int instructRegisterDisplay = 0;
int operationCodeDisplay = 0;
int operandDisplay = 0;


//Menu Object for outputting the options available, 
//Tyler could add the Exponential/Remainder operations as Menu Objects -Scott's take on this method
class Menu
{
private:
	int code;
	string name;
public:
	Menu(int c, string n) : code(c), name(n) {};
	Menu(const Menu &m) : code(m.code), name(m.name) { }

	//The numerical operation code (i.e. 10)
	void setCode(int c) {
		this->code = c;
	}

	//The name of the operation (i.e. STORE)
	void setName(string n) {
		this->name = n;
	}

	//The numerical operation code (i.e. 10)
	int getCode() {
		return code;
	}

	//The name of the operation (i.e. STORE)
	string getName() {
		return name;
	}
};

//Startup to output the possible operations available 
//for end user to use.-Scott's take on this method
void showMenu(vector<Menu> &v)
{
	for (int i = 0; i < v.size(); i++)
	{
		cout << v.at(i).getCode() << " - " << v.at(i).getName() << endl;
	}

}

//Class to build Instruction objects, which contain an operation and an operand, 
//also a Value for outputting at the end -Scott's take on this method
class Instruction
{
private:
	int operation = 0;
	int operand = 0;
	int value = 0;
public:
	Instruction(int opt, int opn) : operation(opt), operand(opn) {};
	Instruction(int opt, int opn, int v) : operation(opt), operand(opn), value(v) {};
	Instruction(const Instruction &i) : operation(i.operation), operand(i.operand), value(i.value) { }

	void setOperation(int opt) {
		this->operation = opt;
	}

	void setOperand(int opn) {
		this->operand = opn;
	}

	void setValue(int v) {
		this->value = v;
	}

	int getOperation() {
		return this->operation;
	}

	int getOperand() {
		return this->operand;
	}

	int getValue() {
		return this->value;
	}
};

//Output that shows the instruction count, most recent memory value, its instructions, and all 100 memory locations. 
void memoryDump(vector<Instruction> &memory)
{
	string operationDisplayCheck = to_string(operationCodeDisplay);
	if (operationDisplayCheck.length() < 2)
	{
		operationDisplayCheck = "0" + operationDisplayCheck;
	}

	string operandDisplayCheck = to_string(operandDisplay);
	if (operandDisplayCheck.length() < 3)
	{
		if (operandDisplayCheck.length() < 2)
		{
			operandDisplayCheck = "00" + operandDisplayCheck;
		}
		else
		{
			operandDisplayCheck = "0" + operandDisplayCheck;
		}

	}
	string operationOperandCombined = operationDisplayCheck + operandDisplayCheck;

	cout << "*** PROGRAM COMPLETED ***" << endl;

	// Print out all the basic information summary
	cout << "\n \n REGISTERS:" << endl
		<< setw(20) << left << "Accumulator:" << setw(10) << right << accumulator << endl
		<< setw(20) << left << "InstructionCounter:" << setw(10) << right << instructCount << endl
		<< setw(20) << left << "InstructionRegister:" << setw(10) << right << operationOperandCombined << endl
		<< setw(20) << left << "OperationCode:" << setw(10) << right << operationCodeDisplay << endl
		<< setw(20) << left << "Operand:" << setw(10) << right << operandDisplayCheck << endl << endl;

	// Print all the memory headers
	cout << "\n\nMEMORY:\n";
	cout << setw(9.5) << "00" << setw(9.5) << "01" << setw(9.5) << "02" << setw(9.5)
		<< "03" << setw(9.5) << "04" << setw(9.5) << "05" << setw(9.5) << "06"
		<< setw(9.5) << "07" << setw(9.5) << "08" << setw(9.5) << "09" << endl;

	//Print each row, pulling the data from main memory, checking format, and outputting it to the screen.	
	for (int column = 0; column < MEMORY_SIZE; column++)
	{
		cout << setfill('0') << setw(3) << column << setfill(' ') << setw(9.5);
		for (int row = 0; row < 10; row++)
		{
			if (memory.at(row + column).getValue() == 0)
			{
				if (memory.at(row + column).getOperation() != 0 || memory.at(row + column).getOperand() != 0)
				{
					string operationMemDumpDisplayCheck = to_string(memory.at(row + column).getOperation());
					if (operationMemDumpDisplayCheck.length() < 2)
					{
						operationMemDumpDisplayCheck = "0" + operationMemDumpDisplayCheck;
					}

					string operandMemDumpDisplayCheck = to_string(memory.at(row + column).getOperand());
					if (operandMemDumpDisplayCheck.length() < 3)
					{
						if (operandMemDumpDisplayCheck.length() < 2)
						{
							operandMemDumpDisplayCheck = "00" + operandMemDumpDisplayCheck;
						}
						else
						{
							operandMemDumpDisplayCheck = "0" + operandMemDumpDisplayCheck;
						}

					}
					string operationOperandMemDumpCombined = operationMemDumpDisplayCheck + operandMemDumpDisplayCheck;
					cout << operationOperandMemDumpCombined << setw(9.5);
				}
				else
				{
					cout << "00000" << setw(9.5);
				}
			}
			else
			{
				string memoryValueString = fixMyString(memory.at(row + column).getValue());
				cout << memoryValueString << setw(9.5);
			}
		}
		cout << endl;
		column += 9;
	}
}

//Checks to see if the input from the keyboard was 4 digits? 
//(what do we do with the + and - signs besides on -99999?)-Scott's take on this method
void parseInput(string &input, vector<Menu> &vMenu, vector<Instruction> &vMemory, bool &menuQuit)
{
	// operation is the command and operand is the location
	int operation;
	int operand;

	// checking if the input is 0, greater than 0, or if it is terminate command
	if (stoi(input) >= NUM_ZERO)
	{
		// checking if the input has 5 digits
		if (input.length() > MIM_DIGITS && input.length() < MAX_DIGITS) {

			//For when users want to intentionally make a memory instruction 
			//spot blank(usually that spot is used for storing data later)
			if (stoi(input) == NUM_ZERO)
			{
				vMemory.push_back(Instruction(0, 0));
			}
			else
			{
				operation = stoi(input.substr(0, 2));
				operand = stoi(input.substr(2, 3));

				//Checking if the command (variable operation) exists in the Menu vector,
				//If it exists, store in mainMemory and flag it as a good instruction.
				bool isInstructionValid = false;
				for (int menuCheck = 0; menuCheck < vMenu.size(); menuCheck++)
				{
					if (vMenu.at(menuCheck).getCode() == operation)
					{
						if (operand >= NUM_ZERO && operand < MEMORY_SIZE) {
							//std::cout << "Instruction entered is valid." << endl;
							vMemory.push_back(Instruction(operation, operand));
							isInstructionValid = true;
							instructCount++;
						}
					}
				}
				if (!isInstructionValid)
				{
					cout << "Invalid instruction. Please try again." << endl;

				}
			}

		}
		else {
			cout << "Invalid instruction length. Please try again." << endl;
		}

	}
	else if (stoi(input) == TERMINATE) { // -99999
		std::cout << "Instruction input complete.\n";
		menuQuit = true;
	}
	//else if (stoi(input) == PROGRAMEND) { // SOMETHING
	//	std::cout << "Program instruction input complete.\n";
	//	menuQuit = true;
	//}
	else
	{
		cout << "Invalid input. Please try again." << endl;
	}
}

//Initial loop thru our memory array.
//Calls overloaded method to determine and execute the instructions.
void Calculate_Operations(vector<Instruction> &memory) {

	string input;

	//Initial loop through memory array.
	for (int index = 0; index < memory.size(); index++) {

		int command = memory.at(index).getOperation();
		int memoryLocation = memory.at(index).getOperand();

		//For Display purposes in memoryDump() 
		operationCodeDisplay = command;
		operandDisplay = memoryLocation;

		//To determine which Command has been sent, and execute calculations. -Scott
		switch (command) {

			// Read a word from the keyboard into a specific location in memory.
		case 10: {

			//Input loop, if they give a value bigger than 4 digits or leave it blank
			do {
				cout << "Please enter a number between 1 and 99999: ";
				cin >> input;

			} while (input.length() > 5 || input.length() == 0);

			memory.at(memoryLocation).setValue(stoi(input));
			//cout << "Saving to Memory Location: " << memoryLocation << endl;
			break;
		}

				 // Write output to screen.
		case 11: {

			//Need to work on how to display negative values.
			string outputStyle = fixMyString(memory.at(memoryLocation).getValue());
			cout << "Contents of Memory Location " << fixMyString(memoryLocation) << ": " << outputStyle << endl;
			break;
		}

				 //Load a word from a specific location in memory into the accumulator.
		case 20: {

			accumulator = memory.at(memoryLocation).getValue();
			//cout << "Loaded " << memory.at(memoryLocation).getValue() << " from Memory Location " << memoryLocation << " into the Accumulator." << endl;
			break;
		}

				 //Store from accumulator to main memory
		case 21: {

			memory.at(memoryLocation).setValue(accumulator);
			//cout << accumulator << " loaded from Accumulator to Memory Location " << memoryLocation << endl;
			break;
		}

				 //Addition: Add a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator).
		case 30: {

			accumulator += memory.at(memoryLocation).getValue();
			//cout << "Addition Complete" << endl;
			break;
		}

				 //Subtraction: Subtract a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator).
		case 31: {

			accumulator = accumulator - memory.at(memoryLocation).getValue();
			//cout << "Subtraction Complete" << endl;
			break;
		}

				 //Division: Divide the word in the accumulator by the word from a specific location in memory (leave the result in the accumulator).
		case 32: {

			//Divisor can't be 0
			int denominator = memory.at(memoryLocation).getValue();
			if (denominator != 0) {
				accumulator = accumulator / denominator;
			}
			else {
				cout << "CRITICAL ERROR: Cannot Divide by 0." << endl;
				memoryDump(memory);
			}
			break;
		}

				 //Multiplication: Multiply a word from a specific location in memory to the word in the accumulator (leave the result in the accumulator).
		case 33: {

			int multiplier = memory.at(memoryLocation).getValue();
			if (accumulator * multiplier <= 99999) {
				accumulator *= multiplier;
				//cout << "Multiplication Complete " << accumulator << endl;
			}
			else
			{
				cout << "CRITICAL ERROR: Multiplication operation bigger than 99999." << endl;
				memoryDump(memory);
			}
			break;
		}

				 //Branch to a specific location in memory.
		case 40: {

			/* Memory Locations relate to the for-loop position in the vector,
			simply setting the index value in our for loop to match(- 1
			so it'll run it) to simulate branching -Scott */
			//cout << "Branching to Memory Location: " << memoryLocation << endl;
			index = memoryLocation - 1;
			break;
		}

				 //BranchNeg: Branch to a specific location in memory if result from input(s) is Negative.
		case 41: {

			if (accumulator < 0)
			{
				//cout << "BranchNeg - Branching to Memory Location: " << memoryLocation << endl;
				index = memoryLocation - 1;
			}
			break;
		}

				 //BranchZero: Branch to a specific location in memory if result from input(s) is 0.
		case 42: {

			if (accumulator == 0)
			{
				//cout << "BranchZero - Branching to Memory Location: " << memoryLocation << endl;
				index = memoryLocation - 1;
			}
			break;
		}

				 // Halt: Stop program completely and exit.
		case 43: {
			memoryDump(memory);
			break;
		}
		}
	}
}


/*
Takes in a string and checks the length of the string.
If it's length 1, then it'll add 3 zeros to the front.
2 zeros for length 2, etc.
Parameters: string
Returns: string
*/
string fixMyString(string rawValue) {

	string value = rawValue;
	if (rawValue.length() < 5) {
		if (value.length() == 1) {
			value.insert(0, "0000");
		}
		else if (rawValue.length() == 2) {
			value.insert(0, "000");
		}
		else if (rawValue.length() == 3) {
			value.insert(0, "00");
		}
		else if (rawValue.length() == 4) {
			value.insert(0, "0");
		}
	}
	return value;
}

/*
Takes in an int, converts it to a string, and checks
the length of the string. If it's length 1, then
it'll add 3 zeros to the front. 2 zeros for length 2, etc.
Parameters: int
Returns: string
*/
string fixMyString(int rawValue) {

	string value = to_string(rawValue);
	if (value.length() < 5) {
		if (value.length() == 1) {
			value.insert(0, "0000");
		}
		else if (value.length() == 2) {
			value.insert(0, "000");
		}
		else if (value.length() == 3) {
			value.insert(0, "00");
		}
		else if (value.length() == 4) {
			value.insert(0, "0");
		}
	}
	return value;
}

int main(int argc, const char * argv[])
{
	// Initializing mainMemory and menu(vecptr)
	vector<Instruction> *mainMemory = new vector<Instruction>;
	vector<Menu> *vecptr = new vector<Menu>;

	string input;
	bool menuQuit = false;

	// Operations that are available in the program
	vecptr->push_back(Menu(10, "READ"));
	vecptr->push_back(Menu(11, "WRITE"));
	vecptr->push_back(Menu(20, "LOAD"));
	vecptr->push_back(Menu(21, "STORE"));
	vecptr->push_back(Menu(30, "ADD"));
	vecptr->push_back(Menu(31, "SUB"));
	vecptr->push_back(Menu(32, "DIV"));
	vecptr->push_back(Menu(33, "MULT"));
	vecptr->push_back(Menu(40, "BRANCH"));
	vecptr->push_back(Menu(41, "BRANCHNEG"));
	vecptr->push_back(Menu(42, "BRANCHZERO"));
	vecptr->push_back(Menu(43, "HALT"));
	vecptr->push_back(Menu(-99999, "Terminate"));

	//Future use to establish one program has finished, but there's more to go.
	//vecptr->push_back(Menu(SOMETHING, "Program End"));

	cout << "What action would you like to take?\n" << endl;

	//Displaying the menu with the options
	showMenu(*vecptr);

	// Loop for entering instructions from the user
	while (menuQuit == false)
	{
		cin >> input;
		// Parsing the input
		parseInput(input, *vecptr, *mainMemory, menuQuit);
	}

	//Initialize the remainder of the 100 "data slots" to be blank for ease-of-use in outputting all the data.
	int amtToFill = MEMORY_SIZE - mainMemory->size();
	for (int initialize = 0; initialize < amtToFill; initialize++)
	{
		mainMemory->push_back(Instruction(0, 0));
	}

	//Process the Instructions
	Calculate_Operations(*mainMemory);

	// Displaying Main Memory -- If the code reaches here
	//The instructions didn't have a halt code, so it finished.
	memoryDump(*mainMemory);

	//M2 Test Cases:
	/*

	Test case #1
	10007
	10008
	20007
	30008
	21009
	11009
	43000
	00000
	00000
	00000
	-99999

	Test case #2
	10009
	10010
	20009
	31010
	41007
	11009
	43000
	11010
	43000
	00000
	00000
	-99999


	Test Case #3
	(For loop that finds the lowest
	value in 10 numbers)
	10068
	10069
	10070
	10071
	10072
	10073
	10074
	10075
	10076
	10077

	20068
	21078
	20069
	31078
	41041
	20070
	31078
	41044
	20071
	31078

	41047
	20072
	31078
	41050
	20073
	31078
	41053
	20074
	31078
	41056

	20075
	31078
	41059
	20076
	31078
	41062
	20077
	31078
	41065
	11078

	43000
	20069
	21078
	40012
	20070
	21078
	40012
	20071
	21078
	40012

	20072
	21078
	40012
	20073
	21078
	40012
	20074
	21078
	40012
	20075

	21078
	40012
	20076
	21078
	40012
	20077
	21078
	40012
	00000
	00000

	00000
	00000
	00000
	00000
	00000
	00000
	00000
	00000
	00000
	-99999

	*/

	//M1 Test Cases:
	/*
	Test case #1
	1007
	1008
	2007
	3008
	2109
	1109
	4300
	0000
	0000
	0000
	-99999

	Test case #2
	1009
	1010
	2009
	3110
	4107
	1109
	4300
	1110
	4300
	0000
	0000
	-99999


	Test Case #3
	1068
	1069
	1070
	1071
	1072
	1073
	1074
	1075
	1076
	1077

	2068
	2178
	2069
	3178
	4141
	2070
	3178
	4144
	2071
	3178

	4147
	2072
	3178
	4150
	2073
	3178
	4153
	2074
	3178
	4156

	2075
	3178
	4159
	2076
	3178
	4162
	2077
	3178
	4165
	1178

	4300
	2069
	2178
	4012
	2070
	2178
	4012
	2071
	2178
	4012

	2072
	2178
	4012
	2073
	2178
	4012
	2074
	2178
	4012
	2075

	2178
	4012
	2076
	2178
	4012
	2077
	2178
	4012
	0000
	0000

	0000
	0000
	0000
	0000
	0000
	0000
	0000
	0000
	0000
	-99999

	*/
	return 0;
}

