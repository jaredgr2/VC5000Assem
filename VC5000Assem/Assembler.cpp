//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

/*
NAME

	Assembler - constructor of Assembler object.

SYNOPSIS

	Assembler(int argc, char *argv[]);

DESCRIPTION

	This function is the constructor of the Assembler object, taking in
	the runtime parameters (using argc and argv[]) then passing them to
	the FileAccess object within Assembler class.
*/
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{
    // Nothing else to do here at this point.
}
/*
NAME

	~Assembler - constructor of Assembler object.

SYNOPSIS

	~Assembler;

DESCRIPTION

	This function is the destructor of the Assembler object. It doesn't do much
	but there will be linker errors if we get rid of it, so it stays.
*/
Assembler::~Assembler( )
{
}
/*
NAME

	Pass I - first pass of the Assembler.

SYNOPSIS

	void PassI();

DESCRIPTION

	This function reads through the file successively, determining the instruction types and
	addning symbols to the SymbolTable object. It returns if we reach an end statement
	or there are no more lines to be read. 
*/
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.  Currently this is only comments.
        if( st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr ) 
        {
        	continue;
	    }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) {

            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}

/*
NAME

	Pass II - second pass of the Assembler.

SYNOPSIS

	void PassII();

DESCRIPTION

	This function reads through the file successively, determining the instruction types,
	creating the translation of the contents of a Machine Language instruction by calling 
	TranslateInst, then printing out the locations, contents, and original statements.
*/
void Assembler::PassII( )
{
	int loc = 0;        // Tracks the location of the instructions to be generated.

	m_facc.rewind();    //Rewind the file so we can start reading again from the top.
	
	//Header - for fanciness
	cout << "Translation of Program:" << endl;
	cout << "Location" << "  " << "Contents" << "     " << "Original Statement" << endl;

	// Successively process each line of source code.
	for (; ; ) {

		// Read the next line from the source file.
		string line;
		if (!m_facc.GetNextLine(line)) {
			//If no more lines without an end statement, we record the error and return.
			Errors::RecordError("Error: No end statement");
			return;
		}

		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		//Skip comments, but print them out first
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr && st!= Instruction::ST_End)
		{
			cout << "                   " << m_inst.GetOrgInstruct() << endl;
			continue;
		}
		
		//Here we check what instruction type we have. This will determine what we end up printing.
		if (st == Instruction::ST_MachineLanguage) {
			//Create string form of the translated contents returned by the function
			//translateInt.
			string machineInstruct = to_string(TranslateInst(line,loc));

			//Fill the string with zeroes till it reaches 9 digits.
			while (machineInstruct.length() != 9) {
				machineInstruct.insert(machineInstruct.begin(),1, '0');
			}

			//Check if the op code is bad and replace with question marks.
			if (machineInstruct.substr(0, 2) == "14") {
				Errors::RecordError("Error: Illegal op code");
				machineInstruct.replace(0, 2, "??");
			}

			//Check if the address is bad and replace with question marks.
			if (m_BadLocFlag) {
				machineInstruct.replace(3, 6, "??????");
			}

			//Print out the location, contents, and original line.
			cout << to_string(loc) <<  "  " << machineInstruct << "     " << line << endl;
		}
		else if (st == Instruction::ST_End) {
			//If end statement, print the line and leave Pass II. We're done here

			cout << "                   " << line << endl;
			Errors::DisplayErrors();
			return;
		}
		//If comment or blank line, print the original line
		else if (st == Instruction::ST_Comment || line.empty()) {
			cout << m_inst.GetOrgInstruct() << endl;
		}
		//If not either, just print the location and original line.
		else {
			cout << to_string(loc) << "                " << line << endl;
		}

		// Compute the location of the next instruction. 
		loc = m_inst.LocationNextInstruction(loc);
	}
}
/*
NAME

	TranslateInst - returns the machine language instruction contents.

SYNOPSIS

	int TranslateInst(string a_line, int a_loc);

DESCRIPTION

	This function adds to the machine language variable based on various conditions
	of the instruction. It will still translate the instruction the best it can 
	even if there is an error, which we check for. It also pushes the int version
	to the memory of the emulator at the location passed in.
*/
int Assembler::TranslateInst(string a_line, int a_loc) {
	int machineLang = 0;  //This will store the int format of our translated contents. 
	bool opCodeTrue = m_inst.OpCodeToNum(m_inst.GetOpCode()); //Set to true if we have a machine language op code.
	m_BadLocFlag = false; //The bad location flag is set to false unless otherwise proven by the error checker.

	ErrorChecker(); 

	//Calculate the machine langauge op code.
	machineLang += (10'000'000 * m_inst.GetNumOpCode());

	//If we have a bad location flag, translate the register and then return. We will replace the address in Pass II
	if (m_BadLocFlag) {
		machineLang += (1'000'000 * stoi(m_inst.GetOperand1(), nullptr, 10));
		m_emul.insertMemory(a_loc, machineLang);
		return machineLang;
	}

    //If we have a halt instruction, add register 9 to the contents and return the instruction.
     if (m_inst.GetOpCode() == "halt") {
		machineLang += 9'000'000;
		m_emul.insertMemory(a_loc, machineLang);
		return machineLang;
	 }

	//If we have a write instruction, return the instruction.
	if (m_inst.GetOpCode()=="write" && m_inst.GetOperand2() == "") {
		m_emul.insertMemory(a_loc, machineLang);
		return machineLang +=m_symtab.FindSymbolLoc(m_inst.GetOperand1());
	}

	//If we have a read instruction, add register 9 to the contents as well as the location of operand 1 and return.
	if (m_inst.GetOpCode() == "read") {
		machineLang += (9'000'000 + m_symtab.FindSymbolLoc(m_inst.GetOperand1()));
		m_emul.insertMemory(a_loc, machineLang);
		return machineLang;
	}

	//If we have defined a constant, add the location of the constant by getting operand 1's value and then return.
	else if (m_inst.GetOpCode() == "dc") {
		machineLang += stoi(m_inst.GetOperand1(), nullptr, 10);
		m_emul.insertMemory(a_loc, machineLang);
		return machineLang;
	}

	//If previous cases aren't met, add the register value and the location of operand 2, then return.
	else {
		machineLang += (1'000'000 * stoi(m_inst.GetOperand1(), nullptr, 10)) + m_symtab.FindSymbolLoc(m_inst.GetOperand2());
		m_emul.insertMemory(a_loc, machineLang);
		return machineLang;
	}
}
/*
NAME

	ErrorChecker - checks and records various common errors.

SYNOPSIS

	void ErrorChecker();

DESCRIPTION

	This function uses a big switch statement based on the numerical op code returned
	to check for errors dependent on which operation is being performed. For certain
	address-related errors, it will set a m_BadLocFlag to being true.
*/
void Assembler::ErrorChecker() {

	//This checks the op code to see what kind of errors we might have and record them.

	switch (m_inst.GetNumOpCode()) {
	//In the case of "ds" "dc" "end" and "org"

	case 0:
		if (m_inst.GetOperand2() != ""){ //We never want a 2nd operand for these 
			Errors::RecordError("Error: Number of args exceeds defined operation");
		}
		if (m_inst.GetOpCode() != "end") {//"ds" "dc" and "org" cases
			if (!(m_inst.GetOperand1Numeric())) { 
				Errors::RecordError("Error: Operand 1 must be numeric"); 
			}
			if (m_inst.GetOpCode() != "dc") { //"ds" and "org" cases
				if (m_inst.GetOperand1Value() > 999) {
					Errors::RecordError("Error: Memory location exceeds allocated memory");
				}
			}
			else { //"dc" case
				if (m_inst.GetOperand1Value() > 999'999) {
					Errors::RecordError("Error: Address location exceeds allocated addresses");
				}
			}
		}
		break;

    //These are our "read" and "write" cases

	case 7:
	case 8:
		if (m_inst.GetOperand2() == "") { //For the form of the operation with no register

			//Lookup symbol and if we can't find in table, record error and set flag

			bool SymbolCheck = m_symtab.LookupSymbol(m_inst.GetOperand1());
			if (!(SymbolCheck)) {
				Errors::RecordError("Error: Missing symbolic operand");
				m_BadLocFlag = true;
			}
		}
		else {
			//Lookup symbol and if we can't find in table, record error and set flag

			if (!(m_symtab.LookupSymbol(m_inst.GetOperand2()))) {
				Errors::RecordError("Error: Missing symbolic operand");
				m_BadLocFlag = true;
			}
		}
		break;

    //"halt" cases
	case 13:
		if (m_inst.GetOperand1() != "") { //We don't want more than just the "halt" code
			Errors::RecordError("Error: Number of args exceeds defined operation");
		}
		break;

    //All other cases

    default:
		if (m_inst.GetOperand1Value() < 0 || m_inst.GetOperand1Value() > 9) {
			Errors::RecordError("Error: Invalid register");
		}
		if (!(m_symtab.LookupSymbol(m_inst.GetOperand2()))) {
			Errors::RecordError("Error: Missing symbolic operand");
			m_BadLocFlag = true;
		}
		break;
	}
}