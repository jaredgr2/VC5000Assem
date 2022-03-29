//
// Implementation of the Instruction class.
//
#include"stdafx.h"
#include"Instruction.h"
#include"Errors.h"

/*
NAME

	ParseInstruction - parses through the instruction and returns its type.

SYNOPSIS

	InstructionType ParseInstruction(string a_line);

DESCRIPTION

	This function will parse the instruction line and return what type of instruction it is
	based on various factors that we check for.
*/
Instruction::InstructionType Instruction::ParseInstruction(string a_line) {
	m_instruction = a_line; //Stores the original instruction.
	bool fieldsCheck; //Bool value that is true if we don't have extra fields. 


	//THIS CODE WAS MADE POSSIBLE BY : THE HELP SECTION ON PROFESSOR MILLER'S WEBSITE.
    //THANK YOU!
	//We check for any comments and erase them.
	size_t isemi1 = a_line.find(';');
	if (isemi1 != string::npos) {
		a_line.erase(isemi1);
	}

	//If the entire line was erased by the previous code block, it was all
	//a comment. So we return that the instruction type is comment!
	if (a_line.empty()) {
		m_type = ST_Comment;
		return m_type;
	}

	//We use a STL transformation to make the whole line lowercase -
	//this makes things a lot easier - we only have to check for the lower case form of
	//any op code. Nice! 
	transform(a_line.begin(), a_line.end(), a_line.begin(), ::tolower);

	//We call the function to parse the line into the variables
	//that was provided in class, passing in them + the line.
	fieldsCheck=ParseLineIntoFields(a_line, m_Label, m_OpCode,
		m_Operand1, m_Operand2);

	//We set the bool value checking if operand 1 is numeric to true each time 
	//we read in a line - since it retains the value each time once we're done, so
	//we just "reset" it so we don't have it being false when it really isn't.
	m_IsNumericOperand1 = true;

	//Check if operand 1 is empty - if it is, it obviously can't be numeric! (Doy!)
	if (m_Operand1.empty()) {
		m_IsNumericOperand1 = false;
	}

	//Use a range-based for loop (thanks C++ 11!) to check each character of operand 1
	//and if isdigit() returns false for any of them, then we set the bool to false.
	for (char const& ch : m_Operand1) {
		if (isdigit(ch) == 0) {
			m_IsNumericOperand1 = false;
		}
	}

	//We check if the bool value is true - if so, operand 1 value variable gets the numeric value from
	//the string using good ol' stoi from C.
	if (m_IsNumericOperand1) {
		m_Operand1Value = stoi(m_Operand1, nullptr, 10);
	}

	//If there isn't extra data, set what type of instruction we have.
	if (fieldsCheck) {
		if (m_OpCode == "end") {
			m_type = ST_End;
		}
		//Must pass the op code test and these 3 op codes don't count as real machine language instructions.
		else if (OpCodeToNum(m_OpCode) && m_OpCode != "end" && m_OpCode != "ds" && m_OpCode != "org") {
			m_type = ST_MachineLanguage;
		}
		//If it returns false, we still count it as a machine language instruction, but we're gonna replace
		//the op code so translation doesn't stop.
		else if (!(OpCodeToNum(m_OpCode))) {
			m_type = ST_MachineLanguage;
		}
		//If it's not a comment, end statement, or machine lang, it's gotta be an assembler instruction.
		//This is the realm where "end" "ds" and "org" lurk.
		else {
			m_type = ST_AssemblerInstr;
		}
		
	}
	else {
		//Extra fields! We report the error.
		Errors::RecordError("Error: too many fields.");
	}
	return m_type; //Return the instruction type.
} 

/*
NAME

	LocationNextInstruction - calculates the location of the next instruction.

SYNOPSIS

	int LocationNextInstruction(int a_loc);

DESCRIPTION

	This function will check for what op code the instruction has and return the location of 
	the next instruction based on that.
*/
int Instruction::LocationNextInstruction(int a_loc) {
	//We check the op code - only org or ds can affect the location of the next instruction so we check 
	//for those and then calculate the next location
	if (m_OpCode == "org" || m_OpCode == "ds") {
		return a_loc + m_Operand1Value;
	}
	else {
		return a_loc + 1;
	}

}
/*
NAME

	ParseLineIntoFields - parses the string into the data fields.

SYNOPSIS

	bool ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
	string& a_Operand1, string& a_Operand2);

DESCRIPTION

	This function will parse the line given (the instruction) into the data fields,
	getting rid of commas. If there is no extra data, it returns true. Otherwise, false.
*/

//THIS CODE WAS MADE POSSIBLE BY : THE HELP SECTION ON PROFESSOR MILLER'S WEBSITE.
//THANK YOU!
bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
	string& a_Operand1, string& a_Operand2){
	// Get rid of any commas from the line.
	replace(a_line.begin(), a_line.end(), ',', ' ');

	// Get the elements of the line.  That is the label, op code, operand1, and operand2.
	string endStr;
	a_label = a_OpCode = a_Operand1 = a_Operand2 = "";
	istringstream ins(a_line);
	if (a_line[0] == ' ' || a_line[0] == '\t')
	{
		a_label = "";
		ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
	}
	else
	{
		ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
	}
	// If there is extra data, return false.
	return endStr.empty() ? true : false;
}

/*
NAME

	OpCodeToNum - sets the numerical form of the op code.

SYNOPSIS

	bool OpCodeToNum(string m_OpCode);

DESCRIPTION

	This function will check for what op code it is given, set m_NumOpCode to the machine language equivalent
	of the op code (unless it is a assembly code, which makes it 0, or an illegal code, which
	is 14 for the sake of detecting the error without disrupting translation).
*/
bool Instruction::OpCodeToNum(string m_OpCode) {
    //This is certainly an inelegant solution - I probably could've used an array of strings to check against, 
	//but it helps in translation to use the same op code (0) for the 4 assembler op codes.
	if (m_OpCode == "add") {
		m_NumOpCode = 1;
	}
	else if (m_OpCode == "sub"){
		m_NumOpCode = 2;
	}
	else if (m_OpCode == "mult") {
		m_NumOpCode = 3;
	}
	else if (m_OpCode == "div") {
		m_NumOpCode = 4;
	}
	else if (m_OpCode == "load") {
		m_NumOpCode = 5;
	}
	else if (m_OpCode == "store") {
		m_NumOpCode = 6;
	}
	else if (m_OpCode == "read") {
		m_NumOpCode = 7;
	}
	else if (m_OpCode == "write") {
		m_NumOpCode = 8;
	}
	else if (m_OpCode == "b") {
		m_NumOpCode = 9;
	}
	else if (m_OpCode == "bm") {
		m_NumOpCode = 10;
	}
	else if (m_OpCode == "bz") {
		m_NumOpCode = 11;
	}
	else if (m_OpCode == "bp") {
		m_NumOpCode = 12;
	}
	else if (m_OpCode == "halt") {
		m_NumOpCode = 13;
	}
	else if (m_OpCode == "org" ||m_OpCode == "end" || m_OpCode == "ds" ||m_OpCode == "dc") {
	    m_NumOpCode = 0;
	}
	//This is the number we use for the bad, no-good illegal op codes. It helps us in translation
	//because we can replace that 14 with question marks later on. Also, it returns false - 
	//That way we know an illegal op code was used.
	else {
		m_NumOpCode = 14;
		return false;
	}
	return true;
}
