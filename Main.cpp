//
// PURPOSE: Password Manager
//
// Author : Atiq Rahman
// Date   : 08-23-2012
// Status : Good
// Remarks: Credentials Manager Software
//        - Encrypt/decrypt password database,
//        - Retrieves credential, stores
//


#include "Rijndael.h"
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstring>
#include <cctype>
#define DEC_ENABLED  1
#define ENC_ENABLED  1
using namespace std;

// Function to convert unsigned char to string of length 2
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

// Function to convert string of length 2 to unsigned char
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

// Function to convert string of unsigned chars to string of chars
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

// Function to convert string of chars to string of unsigned chars
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

// Global Data structure
struct credDbRecord {
  string siteURL;
  string tags;
  string regEmail;
  string userName;
  string password;
};

typedef struct credDbRecord acRecord;

map<string, acRecord> credDatabase;
const int pwBlockSize = 32;


// Displaying Menu & take user input functions
void displayTitle();
void displayMainMenu();
short int getVerifiedChoice();
char takeUserChoiceMainMenu();
short int translateChoice(char ch);

// Record functions
void loadDatabase();
void addRecord();
void pushRecord(string& record);
void saveRecords();
void showRecords();


int main () {
  displayTitle();
  // authenticateUser();
  loadDatabase();

  // process command line arguments and display menu depending on that
  while (true) {
    displayMainMenu();

    short int choice = getVerifiedChoice();

    switch (choice) {
    case 1:
      addRecord();
      break;
    /*case 2:
      changeRecord();
      break;
    case 3:
      deleteRecord();
      break;*/
    case 4:
      showRecords();
      break;
    /*case 5:
      changePasswd();
      break;*/
    case 6:
      saveRecords();
      return 0;
    default:
      cout<<"Enexpected choice!!"<<endl;
      break;
    }
  }
    return 0;
}


// Show Title of the Console Program
void displayTitle() {
  cout<<"Credential Manager 1.0 (console version)"<<endl;
  cout<<"========================================"<<endl;
}

void loadDatabase() {
  string entireRecords;
  ifstream inFile ("pw_db.bin", ios::binary);
  char* encBuffer = new char[pwBlockSize+1];
  char plainBuf[pwBlockSize+1];
  streamsize lastReadSize;

  #ifdef DEC_ENABLED
  CRijndael oRijndael;

  try {
    // Dummy key
    oRijndael.MakeKey("x9z1v6f7q1l3m8p9f9s2tg3n5c5o5w9r", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", pwBlockSize, pwBlockSize);
  }
  catch(exception& roException) {
    cout << roException.what() << endl;
  }
  #endif

  // readfile and store text into string entireRecords
  if (inFile.is_open()) {
    while (inFile.good()) {
      inFile.read(encBuffer, pwBlockSize);
      lastReadSize = inFile.gcount();
      cout<<"lastReadSize: "<<lastReadSize<<endl;

      #ifdef DEC_ENABLED
      memset(plainBuf, 0, pwBlockSize);
      oRijndael.DecryptBlock(encBuffer, plainBuf);
      plainBuf[lastReadSize] = '\0';
      entireRecords += plainBuf;
      #else
      encBuffer[lastReadSize] = '\0';
      //string tmp(buffer);
      //cout<<"buffer: "<<tmp<<endl;
      entireRecords += encBuffer;
      #endif
    }
    inFile.close();
  }
  else
    cout << "Unable to open input/output file"<<endl;

  cout<<"Record string: "<<entireRecords<<endl;

  // parse the string and load into map
  //entireRecords.find("");
  size_t found, prefound=13;  // 13 for the string "<beginRecord>"

  while (true) {
    // assuming "</endRecord>" will be found within next 1000 chars
    found = entireRecords.find("</endRecord>", prefound);
    //found = entireRecords.find("google.com", prefound);
    if (found == string::npos)
      break;

    string singleRecord = entireRecords.substr(prefound, found-prefound);
    //cout<<"Single record: "<<singleRecord<<endl;

    // push the record into database
    pushRecord(singleRecord);

    found = entireRecords.find("<beginRecord>", found+13);    // 13 for "</endRecord>\n"
    if (found == string::npos)    // we have reached end of records
      break;
    prefound = found+13;
  }
}

// push single record into database
void pushRecord(string& record) {
  size_t found = record.find('\n'), prefound;
  acRecord recSingleInstance;

  //cout<<"new line found in position: "<<found<<endl;

  string title = record.substr(0, found);
  cout<<"site title: "<<title<<endl;

  prefound = found+1;
  found = record.find('\n', prefound);
  string field = record.substr(prefound, found-prefound);
  cout<<"site URL: "<<field<<endl;
  recSingleInstance.siteURL = field;

  prefound = found+1;
  found = record.find('\n', prefound);
  field = record.substr(prefound, found-prefound);
  cout<<"tags: "<<field<<endl;
  recSingleInstance.tags = field;

  prefound = found+1;
  found = record.find('\n', prefound);
  field = record.substr(prefound, found-prefound);
  cout<<"site reg email: "<<field<<endl;
  recSingleInstance.regEmail = field;

  prefound = found+1;
  found = record.find('\n', prefound);
  field = record.substr(prefound, found-prefound);
  cout<<"user name: "<<field<<endl;
  recSingleInstance.userName = field;

  prefound = found+1;
  found = record.find('\n', prefound);
  field = record.substr(prefound, found-prefound);
  cout<<"password: "<<field<<endl<<endl;
  recSingleInstance.password = field;

  credDatabase[title] = recSingleInstance;
}

// Display Menu
void displayMainMenu() {
  cout<<"Password Manager Menu"<<endl;
  cout<<"1. Add record (a)"<<endl;
  cout<<"2. Change record (c)"<<endl;
  cout<<"3. Delete record (d)"<<endl;
  cout<<"4. Show records (s)"<<endl;
  cout<<"5. Change password of Credential Manager (p)"<<endl;
  cout<<"6. Exit (e)"<<endl;
}

short int getVerifiedChoice() {
  short int choice;

  while (1) {
    choice = translateChoice(takeUserChoiceMainMenu());

    // verify choice
    if (choice > 0 && choice < 7)
      break;
    cout<<"You have entered a wrong choice! Please select again."<<endl;
  }
  return choice;
}

char takeUserChoiceMainMenu() {
  char choice='\n';
  cout<<endl<<"Please enter choice: ";

  // check if character is newline or space
  while (choice == '\n' || choice == ' ')
    choice = cin.get();

  // take out the extra garbage newline comes after the character, otherwise it makes problem in later processing
  cin.get();

  return choice;
}

// translates choice and returns an integer value within range 0 to 6 where 0 means a wrong choice
short int translateChoice(char ch) {
  if (isdigit(ch))
    return (ch-'0');

  switch(ch) {
  case 'a': case 'A':
    return 1;
  case 'c': case 'C':
    return 2;
  case 'd': case 'D':
    return 3;
  case 's': case 'S':
    return 4;
  case 'p': case 'P':
    return 5;
  case 'e': case 'E':
    return 6;
  default:
    return 0;
  }
}

// This functions adds a record into our credential file
void addRecord() {
  acRecord tempRecord;
  cout<<endl<<"Add record wizard"<<endl;
  cout<<"================="<<endl;

  // Take Title, may contain space
  string siteTitle;
  cout<<"Please enter site Title: ";

  while (true)  {
    getline(cin, siteTitle);

    if (credDatabase.find(siteTitle) == credDatabase.end())
      break;
    else
      cout<<endl<<"Record with this title already exists. Please enter different site title: ";
  }

  // Take URL, may contain space but cannot be empty
  string url;

  while (true) {
    cout<<"Please enter site URL: ";
    getline(cin, url);
    if (url.size())
      break;
    cout<<"URL cannot be empty string. ";
  }


  // remove http:// from beginning of the string
  if (!strncmp(url.c_str(), "http://", 7)) {
    url.erase(0,7);
  }

  tempRecord.siteURL = url;

  // Take tags, may contain space
  string tags;
  cout<<"Enter tags (if any): ";
  getline(cin, tags);
  tempRecord.tags = tags;

  // Take registration email address, can not contain space
  string regEmail;
  cout<<"Enter email address used for registration: ";
  cin>>regEmail;
  tempRecord.regEmail = regEmail;

  // Take username, can not contain space
  string userName;
  cout<<"Enter user name: ";
  cin>>userName;
  tempRecord.userName = userName;

  // Take passwd, can not contain space
  string passwd;
  cout<<"Enter password: ";
  cin>>passwd;
  tempRecord.password = passwd;

  credDatabase[siteTitle] = tempRecord;
}

void saveRecords() {
#ifdef ENC_ENABLED
  CRijndael oRijndael;
#endif
  ofstream outFile ("pw_db.bin", ofstream::binary);

  int i;
  map<string, acRecord>::iterator it;
  acRecord *pRecord;
  string entireRecords;

  if (outFile.is_open() == false) {
    cout<<"Cannot open file for write."<<endl;
    return ;
  }

  if (credDatabase.size() == 0) {
    cout<<"No record to save."<<endl;
    outFile.close();
    return ;
  }

  cout<<"Trasnlating to text."<<endl;
  for (it=credDatabase.begin(), i=1; it != credDatabase.end(); ++it, ++i) {
    pRecord = &(*it).second;
    entireRecords += "<beginRecord>";
    entireRecords += (*it).first+"\n";
    entireRecords += pRecord->siteURL+"\n";
    entireRecords += pRecord->tags+"\n";
    entireRecords += pRecord->regEmail+"\n";
    entireRecords += pRecord->userName+"\n";
    entireRecords += pRecord->password+"</endRecord>\n";
  }

  cout<<"Writing text to file."<<endl;
  if (outFile.is_open()) {
    const char *startPos = entireRecords.c_str();;
    char* inBuffer= (char *) startPos;
    int len = entireRecords.length();
    int lengthToWrite;
    char outBuffer[pwBlockSize+1] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

    #ifdef ENC_ENABLED
    try {
      // TODO: Store the key in a global var
      oRijndael.MakeKey("x9z1v6f7q1l3m8p9f9s2tg3n5c5o5w9r", "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", pwBlockSize, pwBlockSize);
    }
    catch(exception& roException) {
      cout << roException.what() << endl;
    }
    #endif

    bool notEndBlockFlag = true;
    char* tmpBuffer = new char[pwBlockSize+1];

    while (outFile.good() && (inBuffer-startPos<len) && notEndBlockFlag) {
      if (inBuffer+pwBlockSize > startPos+len) {
        lengthToWrite = startPos+len-inBuffer;

        // technique to complete the block, we change the pointer inBuffer to tmpBuffer because inBuffer is pointing to a constant string
        memcpy(tmpBuffer, inBuffer, lengthToWrite);

        // put Z on rest of the block
        for (int i=lengthToWrite; i<pwBlockSize; i++)
          tmpBuffer[i] = 'Z';

        inBuffer = tmpBuffer;
        notEndBlockFlag = false;
      }
      //else
        //lengthToWrite = pwBlockSize;

      #ifdef ENC_ENABLED
      // start here encryption
      try  {
        oRijndael.EncryptBlock(inBuffer, outBuffer);
      }
      catch(exception& roException) {
        cout << roException.what() << endl;
      }
      outFile.write(outBuffer, pwBlockSize);
      #else
      outFile.write(inBuffer, pwBlockSize);
      #endif
      // end here encryption


      inBuffer+=pwBlockSize;
      cout << "Writing block.."<<endl;
      //buffer[pwBlockSize]=0;
      //CharStr2HexStr((unsigned char*)szDataIn, szHex, 32);
      //cout << szHex << endl;
    }
    delete tmpBuffer;
    cout << "Write done."<<endl;
    outFile.close();
  }
  else
    cout << "Unable to open file."<<endl;
}

void showRecords() {
  int i;
  map<string, acRecord>::iterator it;
  acRecord *pRecord;

  cout<<endl<<"Display entries"<<endl;
  cout<<"==============="<<endl;

  if (credDatabase.size() == 0) {
    cout<<"No record in database has been entered yet."<<endl;
    return ;
  }

  for (it=credDatabase.begin(), i=1; it != credDatabase.end(); ++it, ++i) {
    pRecord = &(*it).second;
    cout<<"Record "<<i<<": "<<endl;
    cout<<"  Title:\t"<<(*it).first<<endl;
    cout<<"  URL:\t\t"<<pRecord->siteURL<<endl;
    cout<<"  Tags:\t\t"<<pRecord->tags<<endl;
    cout<<"  Reg Email:\t"<<pRecord->regEmail<<endl;
    cout<<"  User Name:\t"<<pRecord->userName<<endl;
    cout<<"  Password:\t"<<pRecord->password<<endl<<endl;
  }
  cout<<endl;
}
