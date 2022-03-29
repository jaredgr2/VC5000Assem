//
//		Implementation of the Emulator class.
//
#include "stdafx.h"
#include "Emulator.h"


/*
NAME

	insertMemory - inserts an instruction into memory at given location.

SYNOPSIS

	bool insertMemory(int a_location, int a_contents);

DESCRIPTION

	This function will check if the location is valid (returns false otherwise), then store the contents
	of an instruction at the index of m_memory specified by a_location, then return true.
*/
bool Emulator::insertMemory(int a_location,int a_contents) 
{
	if (a_location > 999'999) {
		return false;
	}
	else {
		m_memory[a_location] = a_contents;
		return true;
	}
}

/*
NAME

	runProgram - runs the program given by the machine lang instructions.

SYNOPSIS

	bool runProgram();

DESCRIPTION

	This function will loop through the vector starting at position 100, turn the instruction into a
	string, pull out the op code, register, and address and turn them back into ints, then run the 
	op code through a switch statement that will determine what operation is performed. Returns true 
	and terminates once we reach a halt. If there is an error, we immediately return false and terminate.
*/
bool Emulator::runProgram() {

	cout << "Results of emulating program:" << endl;

	//I used size_t so I could use index notation.
	for (size_t i = 100; i != m_memory.size(); i++) {
		string tempInstr = to_string(m_memory[i]);

		//Fill the instruction string with zeroes where it needs them.

		while (tempInstr.length() != 9) {
			tempInstr.insert(tempInstr.begin(), 1, '0');
		}
		//Create substrings of op code, register, and address.

		string tempOpCodeStr = tempInstr.substr(0, 2);
		string regStr = tempInstr.substr(2, 1);
		string addrStr = tempInstr.substr(3, 6);

		//Turn these substrings back into ints using stoi.

		int tempOpCode = stoi(tempOpCodeStr, nullptr, 10);
		int reg = stoi(regStr, nullptr, 10);
		int addr = stoi(addrStr, nullptr, 10);

		//The big switch statement
		switch (tempOpCode) {
			case 1: //ADD
				m_reg[reg] += m_memory[addr];
				break;
			case 2: //SUB
				m_reg[reg] -= m_memory[addr];
				break;
			case 3: //MULT
				m_reg[reg] *= m_memory[addr];
				break;
			case 4: //DIV
				m_reg[reg] /= m_memory[addr];
				break;
			case 5: //LOAD
				m_reg[reg] = m_memory[addr];
				break;
			case 6: //STORE
				m_memory[addr] = m_reg[reg];
				break;
			case 7: //READ
				int x;
				cout << "? ";

				//Take in the user's input and store it at address.

				cin >> x; 
				m_memory[addr] = x;
				cout << endl;
				break;
			case 8: //WRITE
				cout << m_memory[addr] << endl;
				break;
			case 9: //B
				i = addr -1;
				break;
			case 10: //BN
				if (m_reg[reg] < 0) {
					i = addr - 1;
				}
				break;
			case 11: //BZ
				if (m_reg[reg] == 0) {
					i = addr - 1;
				}
				break;
			case 12: //BP
				if (m_reg[reg] > 0) {
					i = addr - 1;
				}
				break;
			case 13: //HALT
				cout << "End of emulation" << endl;
				return true;
				break;
			case 14: //ERRORS
				cerr << "Error detected. Emulation terminated." << endl;
				return false;
				break;
		}
	}
}