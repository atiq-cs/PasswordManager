## Supplement
This doc contains the driver program and early instructions when we didn't have options to modify records via menu.


### Early Instructions
Manually modifying password database file of Password Manager when modifying records via app wasn't available. Essentially, we would modify the plain text database file back, modify the records manually exiting the text file and encrypt it back.

In instructions below,
- disabling a macro means setting its value to 0.
- enabling a macro means setting its value to 1. For example,

```cpp
#define ENC_ENABLED  1
```

These macros are found in 'RecordDatabase.h'.

**Getting plain text file back from the encrypted file**  
After getting the app code, set correct private key. Let's follow steps below,

- First, let's disable `ENC_ENABLED` going to .h file.
- Enable `WRITE_FORCED` and Run program. Or, add an entry, save it.
 - that will do the trick for encryption disabled writing
 - first try to find duplicate for this entry using 'Passwords.txt', remember that search using the menu option is case sensitive.
- Now, we disable `DEC_ENABLED` before running the program next time.

**Modify the password db**  
Now we can use it to add entries running the program,
- Length should be divisible by 32, if the length is less than 32 please append few 'Z' to make it 32.

**Getting back encryption database file**  
Getting back encryption database from the plain text db we retained above,

- First, let's enalbe `ENC_ENABLED` going to .h file. It's a good idea to back up the database file.
- Now, let's enable `WRITE_FORCED`, run the program which gives us the encrypted database.
- Afterwards, disable `WRITE_FORCED` and run the program with `DEC_ENABLED` to check (by showing records or by searching).
- Disable `DEC_ENABLED` before running the program next time.
- Modify to dissembled key (probably for safety, not keeping the original private key in code).

ref, *CredentialsManager_Inst.txt*


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

  try  {
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
