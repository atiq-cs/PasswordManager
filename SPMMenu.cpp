#include "SPMMenu.h"
#include <iostream>


SPMMenu::SPMMenu(void) {
  activeFlag = true;
}


SPMMenu::~SPMMenu(void) {
}

// check whether menu is active
bool SPMMenu::isActive(void)
{
  return activeFlag;
}

// set menu to disabled state
void SPMMenu::disable(void)
{
  activeFlag = false;
}

// Add single item into the menu
void SPMMenu::addItem(short int id, const char* text, char hotkey)
{
  MenuItem newItem;
  newItem.id = id;
  newItem.text = text;
  newItem.hotkey = hotkey;
  itemList.push_back(newItem);
}

// Display the menu
void SPMMenu::display(void)
{
  vector<MenuItem>::iterator menuIt;
  cout<<"Password Manager Menu"<<endl;

  for (menuIt=itemList.begin(); menuIt<itemList.end(); menuIt++ )
    cout<<(*menuIt).id<<". "<<(*menuIt).text<<" ("<<(*menuIt).hotkey<<")"<<endl;
}

AvailableOperations SPMMenu::getChoice(void) {
  AvailableOperations choice;

  while (true) {
    // if input is wrong then choice will be 0
    choice = translateChoice(takeUserChoiceMainMenu());
    // cout<<"debug, choice = "<<(int)choice<<endl;

    // verify choice
    if (choice == InvalidInput)
      cout<<"You have entered a wrong choice! Please select again."<<endl;
    if (choice == NotSingleCharInput)
      cout<<"You have entered multiple characters! Please select with a single letter or digit."<<endl;
    else
      break;
  }
  return choice;
}

char SPMMenu::takeUserChoiceMainMenu() {
  char inputChar='\n';
  short char_count=0;
  cout<<endl<<"Please enter a choice: ";

  // ignore newline and spaces until a char is found; at the end char is stored in 'inputChar' variable
  while (inputChar == '\n' || inputChar == ' ')
    inputChar = cin.get();
  char_count++;

  // continue input until newline is not found
  while (cin.get() != '\n')
    char_count++;

  if (char_count > 1)
    return NotSingleCharInput;
  // take out the extra garbage newline which comes after the character, otherwise it makes problem in later processing
  // cout<<"returning "<<(int) inputChar<<endl;
  return inputChar;
}

// translates choice and returns an integer value within range 0 to 6 where 0 means a wrong choice
AvailableOperations SPMMenu::translateChoice(char ch) {
  short int choice;

  // if digit check it it's in range
  if (ch == NotSingleCharInput)
    return NotSingleCharInput;
  else if (isdigit(ch)) {
    choice = ch-'0';
    // correct
    if (choice > 0 && (int) choice <= (int) itemList.size())  // cast to avoid sign mismatch
      return (AvailableOperations) choice;
  }
  // if character (do a tolower) check if it matches with item hotkeys
  else if (isalpha(ch)) {
    vector<MenuItem>::iterator menuIt;
    char temp;

    if (isupper(ch))
      ch = tolower(ch);

    for (menuIt=itemList.begin(); menuIt<itemList.end(); menuIt++ ) {
      temp = (*menuIt).hotkey;
      if (temp == ch)
        return (AvailableOperations) (*menuIt).id;
    }
  }

  return InvalidInput;
}
