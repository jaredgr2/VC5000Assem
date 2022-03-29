//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation and print
	void PassII(); 

	//Create translated version of instruction to use in Pass II
	int TranslateInst(string,int);

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }

	//Error checking is performed here.
	void ErrorChecker();
    
    // Run emulator on the translation.
	void RunProgramInEmulator() { m_emul.runProgram(); }

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab; // Symbol table object
    Instruction m_inst;	    // Instruction object
    Emulator m_emul;        // Emulator object
	bool m_BadLocFlag;      //Flag that is set if address gives an error.
};

