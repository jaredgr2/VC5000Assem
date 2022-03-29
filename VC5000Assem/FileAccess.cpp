//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"

/*
NAME

	FileAccess - constructor of FileAccess object.

SYNOPSIS

	FileAccess(int argc, char *argv[]);

DESCRIPTION

	This function is the constructor of the FileAccess class, opening the file 
	that the runtime parameter specifies. If the file can't be opened, throw an
	error and exit with code 1.
*/
FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file.  One might question if this is the best place to open the file.
    // One might also question whether we need a file access class.
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}
/*
NAME

	~FileAccess - destructor of the FileAccess class

SYNOPSIS

	~FileAccess();

DESCRIPTION

	This function is the destructor of the FileAccess class and closes the file.
*/
FileAccess::~FileAccess( )
{
    // Not that necessary in that the file will be closed when the program terminates, but good form.
    m_sfile.close( );
}
/*
NAME

	GetNextLine - gets the next line of the file.

SYNOPSIS

	bool GetNextLine(string &a_line);

DESCRIPTION

	This function gets the next line of the file and returns false if there is no more data
	- otherwise, it returns true.
*/
bool FileAccess::GetNextLine( string &a_line )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_line );
    
    // Return indicating success.
    return true;
}
/*
NAME

	rewind - returns the file back to the beginning.

SYNOPSIS

	void rewind();

DESCRIPTION

	This function returns the file back to the beginning and clears any flags that may have been set.
*/
void FileAccess::rewind( )
{
    // Clean all file flags and go back to the beginning of the file.
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
    
