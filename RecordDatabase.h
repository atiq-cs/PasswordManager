#pragma once

#include <map>
#include <string>
#define DEC_ENABLED	1
#define ENC_ENABLED	1
using namespace std;


// Global Data Variables
const int pwBlockSize = 32;

// Global Data structure
typedef struct DatabaseRecord {
	string siteURL;
	string tags;
	string regEmail;
	string userName;
	string password;
} AccountRecord;


class RecordDatabase {
private:
	bool dbChangeStatus;
	map<string, AccountRecord> accountsMap;
	void loadDatabase();
	void pushRecord(string& record);

public:
	void addRecord();
	void saveRecords();
	void showRecords();
	void searchRecords();
	bool hasChanged();

	// constructor and destructor
	RecordDatabase(void);
	~RecordDatabase(void);
};
