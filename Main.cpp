//
// PURPOSE: Password Manager
//
// Author : Atiq Rahman
// Date   : 05-09-2013
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

  // how does it authenticate?
  // 1. read the password
  // 2. read the key
  // 3. decrypt file and check if file has been decrypted correctly if yes then proceed
  // other key services
  // everytime write is done, key position has to be changed
  //
  // authenticateUser();

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

  // Process command line arguments and display menu depending on that
  while (menu.isActive()) {
    menu.display();
    AvailableOperations choice = menu.getChoice();

    switch (choice) {
    case AddRecord:
      passwordDatabaseObj.addRecord();
      break;
    case ChangeRecord:
      cout<<"we are in change records"<<endl;
      // changeRecord();
      break;
    case DeleteRecord:
      cout<<"we are in delete records"<<endl;
      // deleteRecord();
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

  // Due to some quick pro in manual processing, probably to trigger write after private key change
  //  this segment below is for historical significance
  // #if defined(ENC_ENABLED) && !defined(DEC_ENABLED)
  //     passwordDatabaseObj.saveRecords();
  // #else
  // #endif

  // Below is for manual processing
  #ifdef WRITE_FORCED
    passwordDatabaseObj.saveRecords();
  #else
    if (passwordDatabaseObj.hasChanged())
      passwordDatabaseObj.saveRecords();
    else {
        cout<<endl<<"No change in database, modification ignored. ";
    }
  #endif

  cout<<"Program terminated successfully."<<endl;
    return 0;
}

// Show Title of the Console Program
void displayTitle() {
  cout<<"Credential Manager 1.0 (console version)"<<endl;
  cout<<"========================================"<<endl;
}
