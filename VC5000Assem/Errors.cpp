//
// Implementation of the Errors class.
//
#include"stdafx.h"
#include "Errors.h"

//The vector is static and DECLARED in the header, so we have to DEFINE it here.
vector<string> Errors::m_ErrorMsgs;

/*
NAME

	InitErrorReporting - initializes error reporting.

SYNOPSIS

	void InitErrorReporting();

DESCRIPTION

	This function reads through the file successively, determining the instruction types and
	addning symbols to the SymbolTable object. It returns if we reach an end statement
	or there are no more lines to be read.
*/
void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
}
/*
NAME

	RecordError - pushes a recorded error to the error vector.

SYNOPSIS

	void RecordError(string a_esmg);

DESCRIPTION

	This function takes a string that is an error record and pushes it to the back of the error 
	message vector we defined.
*/
void Errors::RecordError(string a_emsg) {
	m_ErrorMsgs.push_back(a_emsg);
}
/*
NAME

	DisplayErrors - displays all the errors that were detected in Pass II.

SYNOPSIS

	void DisplayErrors();

DESCRIPTION

	This function uses a simple range-based for loop that iterates through the entire
	error vector and displays each error message.
*/
void Errors::DisplayErrors() {
	//Range based for loop (again) that iterates through the vector and displays errors.
	for (auto i : m_ErrorMsgs) {
		cout << i << endl;
	}
}