#include "RecordDatabase.h"
#include "Rijndael.h"
#include <fstream>
#include <iostream>

RecordDatabase::RecordDatabase(void)
{
	dbChangeStatus = false;
	// load the database
	loadDatabase();
}

RecordDatabase::~RecordDatabase(void)
{
}
 
void RecordDatabase::loadDatabase() {
	string entireRecords;
	ifstream inFile ("pw.bin", ios::binary);
	char* encBuffer = new char[pwBlockSize+1];
	char plainBuf[pwBlockSize+1];
	streamsize lastReadSize;

	#ifdef DEC_ENABLED
	CRijndael oRijndael;

	try {
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
	size_t found, prefound=13;	// 13 for the string "<beginRecord>"

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

		found = entireRecords.find("<beginRecord>", found+13);		// 13 for "</endRecord>\n"
		if (found == string::npos)		// we have reached end of records
			break;
		prefound = found+13;
	}
}

// push single record into database
void RecordDatabase::pushRecord(string& record) {
	size_t found = record.find('\n'), prefound;
	AccountRecord recSingleInstance;

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

	accountsMap[title] = recSingleInstance;
}

// This functions adds a record into our credential file
void RecordDatabase::addRecord() {
	AccountRecord tempRecord;
	cout<<endl<<"Add record wizard"<<endl;
	cout<<"================="<<endl;

	// Take Title, may contain space
	string siteTitle;
	cout<<"Please enter site Title: ";

	while (true)  {
		getline(cin, siteTitle);

		if (accountsMap.find(siteTitle) == accountsMap.end())
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

	accountsMap[siteTitle] = tempRecord;
	dbChangeStatus = true;
}

void RecordDatabase::saveRecords() {
#ifdef ENC_ENABLED
	CRijndael oRijndael;
#endif
	ofstream outFile ("pw.bin", ofstream::binary);

	int i;
	map<string, AccountRecord>::iterator it;
	AccountRecord *pRecord;
	string entireRecords;

	if (outFile.is_open() == false) {
		cout<<"Cannot open file for write."<<endl;
		return ;
	}

	if (accountsMap.size() == 0) {
		cout<<"No record to save."<<endl;
		outFile.close();
		return ;
	}

	cout<<"Trasnlating to text."<<endl;
	for (it=accountsMap.begin(), i=1; it != accountsMap.end(); ++it, ++i) {
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
			try	{
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

void RecordDatabase::showRecords() {
	int i;
	map<string, AccountRecord>::iterator it;
	AccountRecord *pRecord;

	cout<<endl<<"Display entries"<<endl;
	cout<<"==============="<<endl;

	if (accountsMap.size() == 0) {
		cout<<"No record in database has been entered yet."<<endl;
		return ;
	}

	for (it=accountsMap.begin(), i=1; it != accountsMap.end(); ++it, ++i) {
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

void RecordDatabase::searchRecords() {
	int i;
	map<string, AccountRecord>::iterator it;
	AccountRecord *pRecord;
	string keyword;

	cout<<endl<<"Search entries"<<endl;
	cout<<"==============="<<endl;

	if (accountsMap.size() == 0) {
		cout<<"No record in database has been entered yet."<<endl;
		return ;
	}
	cout<<"Please enter keyword to search: ";
	cin>>keyword;

	cout<<endl<<"Search results on `"<<keyword<<"`:"<<endl;

	for (it=accountsMap.begin(), i=1; it != accountsMap.end(); ++it, ++i) {
		string title = (*it).first;
		pRecord = &(*it).second;
		string tags = pRecord->tags;

		if (string::npos != title.find(keyword) || string::npos != tags.find(keyword)) {
			cout<<"Record index "<<i<<": "<<endl;
			cout<<"  Title:\t"<<title<<endl;
			cout<<"  URL:\t\t"<<pRecord->siteURL<<endl;
			cout<<"  Tags:\t\t"<<tags<<endl;
			cout<<"  Reg Email:\t"<<pRecord->regEmail<<endl;
			cout<<"  User Name:\t"<<pRecord->userName<<endl;
			cout<<"  Password:\t"<<pRecord->password<<endl<<endl;
		}
	}
	cout<<endl;
}


bool RecordDatabase::hasChanged() {
	return dbChangeStatus;
}
