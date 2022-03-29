//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction( ) { };
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End                   		// end instruction.
    };
    // Parse the Instruction.
	InstructionType ParseInstruction(string a_line);

    // Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

	//Parse the line into our data fields. Returns true for a machine lang instruction
	//and false otherwise.
	bool ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
		string& a_Operand1, string& a_Operand2);

	//Creates and stores the numeric version of the op code to use in the translated instruction.
	//Returns true if the op code is machine language, otherwise returns false.
	bool OpCodeToNum(string m_OpCode);


    // To access the label
    inline string &GetLabel( ) { 
		return m_Label;
	};

	//To access the opCode
	inline string& GetOpCode() {
		return m_OpCode;
	};

	//To access numeric opCode
	inline int& GetNumOpCode() {
		return m_NumOpCode;
	}

	//To access operand 1
	inline string& GetOperand1() {
		return m_Operand1;
	}

	//To access operand 2
	inline string& GetOperand2() {
		return m_Operand2;
	}

	//To access original instruction
	inline string& GetOrgInstruct() {
		return m_instruction;
	}

	//To access if operand 1 is numeric
	inline bool& GetOperand1Numeric() {
		return m_IsNumericOperand1;
	}

	//To access operand 1 value
	inline int& GetOperand1Value() {
		return m_Operand1Value;
	}

    // To determine if a label is blank.
    inline bool isLabel( ) {
		return ! m_Label.empty();
	};


private:


    // The elemements of a instruction
    string m_Label;            // The label.
    string m_OpCode;       // The symbolic op code.
    string m_Operand1;     // The first operand. 
    string m_Operand2;     // The second operand.


    string m_instruction;    // The original instruction.

    // Derived values.
    int m_NumOpCode;     // The numerical value of the op code for machine language equivalents.
    InstructionType m_type; // The type of instruction.

	bool m_IsNumericOperand1;// == true if the operand is numeric.
    int m_Operand1Value;   // The value of the operand if it is numeric.
};


