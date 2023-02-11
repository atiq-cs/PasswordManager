//
// PURPOSE: Password Manager
//
// Author : Atiq Rahman
// Date   : 08-23-2012
// Status : Good
// Remarks: Credentials Manager Software
//        - Encrypt/decrypt password database,
//        - Retrieves credential and stores them
//


#include "SPMMenu.h"
#include "RecordDatabase.h"
#include <iostream>
//#include <string>
//#include <cstring>
//#include <cctype>


// Displaying Menu & take user input functions
void displayTitle();

int main () {
	displayTitle();

	// authenticateUser();
	/*
		how does it authenticate?
		1. read the password
		2. read the key
		3. decrypt file and check if file has been decrypted correctly if yes then proceed

		other key services
		everytime write is done, key position has to be changed
	*/
	RecordDatabase passwordDatabaseObj;
	SPMMenu menu;

	// add menu items
	menu.addItem(AddRecord, "Add record", 'a');
	menu.addItem(ChangeRecord, "Change record", 'c');
	menu.addItem(DeleteRecord, "Delete record", 'd');
	menu.addItem(SearchRecords, "Search records", 's');
	menu.addItem(ShowRecords, "Show records", 'r');
	menu.addItem(ChangePassword, "Change password of Credential Manager", 'p');
	menu.addItem(QuitApp, "Exit", 'e');

	// process command line arguments and display menu depending on that
	while (menu.isActive()) {
		menu.display();
		AvailableOperations choice = menu.getChoice();

		switch (choice) {
		case AddRecord:
			passwordDatabaseObj.addRecord();
			break;
		case ChangeRecord:
			cout<<"we are in change records"<<endl;
			//passwordDatabaseObj.changeRecord();
			break;
		case DeleteRecord:
			cout<<"we are in delete records"<<endl;
			//deleteRecord();
			break;
		case SearchRecords:
			passwordDatabaseObj.searchRecords();
			break;
		case ShowRecords:
			cout<<"we are in show records"<<endl;
			passwordDatabaseObj.showRecords();
			break;
		case ChangePassword:
			cout<<"we are in change pass"<<endl;
			//changePasswd();
			break;
		case QuitApp:
			menu.disable();
			break;
		default:
			cout<<"Enexpected choice!!"<<endl;
			break;
		}
	}

	if (passwordDatabaseObj.hasChanged())
		passwordDatabaseObj.saveRecords();
	else
		cout<<"No change, write skipped."<<endl;

	cout<<"Program terminated successfully."<<endl;
    return 0;
}

// Show Title of the Console Program
void displayTitle() {
	cout<<"Credential Manager 1.0 (console version)"<<endl;
	cout<<"========================================"<<endl;
}

/*//Function to convert unsigned char to string of length 2
void Char2Hex(unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if(byte[i] >= 0 && byte[i] <= 9)
			szHex[i] = '0' + byte[i];
		else
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

//Function to convert string of length 2 to unsigned char
void Hex2Char(char const* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else
			break;
	}
}    

//Function to convert string of unsigned chars to string of chars
void CharStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		Char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

//Function to convert string of chars to string of unsigned chars
void HexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i;
	unsigned char ch;
	for(i=0; i<iSize; i++)
	{
		Hex2Char(pszHexStr+2*i, ch);
		pucCharStr[i] = ch;
	}
}*/
