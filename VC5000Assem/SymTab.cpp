//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"
#include "Errors.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( const string &a_symbol, int a_loc );

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void SymbolTable::AddSymbol( const string &a_symbol, int a_loc ){
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {
		Errors::RecordError("Error: Symbol is multiply defined");
        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

/*
NAME

	DisplaySymbolTable - iterates through and displays the symbol table.

SYNOPSIS

	void DisplaySymbolTable();

DESCRIPTION

	This function will use a range-based for loop to iterate through the Symbol Table 
	and display the key and value at each location.
*/

void SymbolTable::DisplaySymbolTable() {
	cout << "Symbol Table:" << endl;
	cout << "Symbol #" << "   " << "Symbol" << "    " << "Location" << endl;

	int idx = 0;
	//We use an range based for loop (we don't really need the auto here but it's good form)
	//to go through the entire table and display the key (the symbol) then the 
	//value (the location).
	for (auto iter: m_symbolTable) {
		cout << idx <<"           " <<  iter.first << "          " << iter.second << endl;
		idx++;
	}
}

/*
NAME

	LookupSymbol - looks through the Symbol Table to see if a symbol is there.

SYNOPSIS

	bool LookupSymbol(const string& a_symbol);

DESCRIPTION

	This function will use an iterator to find the symbol supplied in
	the symbol table - if it's found, it returns true, otherwise false.
*/

bool SymbolTable::LookupSymbol(const string& a_symbol) {
	//Use an iterator to find where the symbol is.
	map<string, int>::iterator it = m_symbolTable.find(a_symbol);
	if (it != m_symbolTable.end()) {
		return true;
	}
	return false; //We couldn't find the symbol - return false
}

/*
NAME

	FindSymbolLoc - looks through the Symbol Table to get a symbol's location

SYNOPSIS

	int FindSymbolLoc(const string& a_symbol);

DESCRIPTION

	This function will use an iterator to find the symbol supplied in
	the symbol table and if found returns the value there (the location)
	otherwise returns null.
*/
int SymbolTable::FindSymbolLoc(const string& a_symbol) {
	//Use an iterator to find where the symbol is.
	map<string, int>::iterator it = m_symbolTable.find(a_symbol);
	if (it == m_symbolTable.end()) {
		return NULL;
	}
	return m_symbolTable[a_symbol]; //Return the value of the key (the location)
}
