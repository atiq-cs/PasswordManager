//
// PURPOSE: SPM Menu Header File
//
// Author : Atiq Rahman
// Date   : 02-28-2013
// Remarks: This header file contains the Menu class and necessary function definitions
//     Additionaly, it declares an enum which specifies what types of operations are available for
//     the menu and database
//

#pragma once

#include <vector>
#include <string>
using namespace std;


typedef struct ItemType {
  short int id;    // id should be unique
  char hotkey;    // a single character hotkey along with numeric, numeric should be the index value
  string text;    // The entry text
} MenuItem;

//typedef struct ItemType MenuItem;

enum AvailableOperations {
  InvalidInput=0,
  AddRecord,    // by default starts from zero, we want to start from non-zero
  ChangeRecord,
  DeleteRecord,
  SearchRecords,
  ShowRecords,
  ChangePassword,
  QuitApp,
  NotSingleCharInput
};


class SPMMenu {
private:
  vector<MenuItem> itemList;
  bool activeFlag;

  // private functions
  char takeUserChoiceMainMenu();
  AvailableOperations translateChoice(char ch);

public:
  void addItem(short int id, const char* text, char hotkey);
  bool isActive(void);
  void disable(void);
  void display(void);
  AvailableOperations getChoice(void);

  // constructor and destructor
  SPMMenu(void);
  ~SPMMenu(void);
};
