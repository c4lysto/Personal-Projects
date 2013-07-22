#include <string>
#include <conio.h>
#include <fstream>
#include <iostream>
using namespace std;

int main(void)
{
	cout << "Begin Trivial Text File Parsing:\n"; 

	string token;
	ifstream objFile;
	objFile.open("Cube.obj");
	
	while(objFile.good())
	{
		objFile >> token;
		cout << token << "\n";
	}

	cout << "\nPress Any Key to Continue.";
	_getch();
	return 0;
}