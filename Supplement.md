### Driver Program
Initial driver program for Rijndael,


```cpp
// ref, CredentialsManager.cpp (07-30-2012)

#include "Rijndael.h"
#include <iostream>
#include <fstream>
using namespace std;

//Function to convert unsigned char to string of length 2
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
}

int main () {
	char szHex[65];
	char szDataIn[] = "aaaaaaaabbbbbbbbaaaaaaaabbbbbbbb";
	char szDataOut[33] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	//One block testing
	CRijndael oRijndael;
	ifstream pwdFile ("pw_db.bin", ios::in|ios::binary);
	if (pwdFile.is_open() == false) {
		cout<<"DB File not found! Please check."<<endl;
	}

	try	{
		oRijndael.MakeKey("x9z1v6f7q1l3m8p9f9s2tg3n5c5o5w9r", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", 32, 32);

		// encrypt data and store; only for creating the file first time

		while (pwdFile.good()) {
			getline (myfile,line);
			cout << line << endl;
		}
		myfile.close();

		else cout << "Unable to open file"; 

		oRijndael.EncryptBlock(szDataIn, szDataOut);

		// szHex only for display purpose
		CharStr2HexStr((unsigned char*)szDataIn, szHex, 32);
		cout << szHex << endl;
		CharStr2HexStr((unsigned char*)szDataOut, szHex, 32);
		cout << szHex << endl;

		// Decrypt Block
		memset(szDataIn, 0, 16);
		oRijndael.DecryptBlock(szDataOut, szDataIn);
		CharStr2HexStr((unsigned char*)szDataIn, szHex, 32);
		cout << szHex << endl;
	}
	catch(exception& roException) {
		cout << roException.what() << endl;
	}

    return 0;
}
```
