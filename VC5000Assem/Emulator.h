//
//		Emulator class - supports the emulation of VC1620 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class Emulator {

public:

    const static int MEMSZ = 1000000;	// The size of the memory of the VC5000.
    Emulator() {

         m_memory.resize(MEMSZ, 0);
    }
    // Records instructions and data into VC5000 memory.
    bool insertMemory( int a_location, int a_contents );
    
    // Runs the VC5000 program recorded in memory.
    bool runProgram( );

private:

    vector<int> m_memory;         // Memory for the VC5000
    int m_reg[10];                        // Registers for the VC5000
};

#endif

