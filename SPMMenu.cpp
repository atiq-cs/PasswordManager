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
void SPMMenu::addItem(short int id, char* text, char hotkey)
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
		if (choice != InvalidInput)
			break;
		cout<<"You have entered a wrong choice! Please select again."<<endl;
	}
	return choice;
}

char SPMMenu::takeUserChoiceMainMenu() {
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
AvailableOperations SPMMenu::translateChoice(char ch) {
	short int choice;

	// if digit check it it's in range
	if (isdigit(ch)) {
		choice = ch-'0';
		// correct
		if (choice > 0 && (int) choice <= (int) itemList.size())	// sign mismatch
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
