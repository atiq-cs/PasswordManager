#pragma once

#include <map>
#include <string>

#define WRITE_FORCED 1
#define DEC_ENABLED  1
#define ENC_ENABLED  1


// Global Data Variables
const int pwBlockSize = 32;

// Global Data structure
typedef struct DatabaseRecord {
  std::string siteURL;
  std::string tags;
  std::string regEmail;
  std::string userName;
  std::string password;
} AccountRecord;


class RecordDatabase {
private:
  bool dbChangeStatus;
  std::map<std::string, AccountRecord> accountsMap;
  void loadDatabase();
  void pushRecord(std::string& record);

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
