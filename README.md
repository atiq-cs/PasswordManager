### Password Manager
Password Manager Command Line App, previously named 'CredMan' or 'Credentials Manager'.

### Introduction
The purpose of this software is to encrypt and decrypt confidential user data, efficiently manage storing and retrieval of this information.

#### Data Structure
Current record data structure has following fields,

```cpp
string siteURL;
string tags;
string regEmail;
string userName;
string password;
```

**Encoding**
The program reads the input file in binary mode and writes in bianry mode.

```cpp
ifstream inFile ("pw.bin", ios::binary);
```

Hence, the output file cannot be opened using regular text editor. We could essential implement a text mode writing to help read/write easier as per the trick in Supplement.

**Build**  
General console instructions apply. It is an MBCS project without requiring additional OS Specific libraries to link.

**Current status of this Project**  
- It can encrypt and decrypt confidential data.
- We can enable or disable encryption and decryption as required.
- Modify take user choice function.
  - When a user mistakenly enters a couple of characters a.k.a string instead of a single character as choice. They should be asked to provide correct input.
- Add records
- Show/Search records

Due to modify/delete not being implemented yet we resort to a trick \(mentioned in [Supplement](https://github.com/atiq-cs/PasswordManager/blob/dev/Supplement.md)\) to modify an entire database file.

**Features TODO**  
On top of mind,
- Modify records
- Delete records
- The process of manual modification to database (disabling encryption to get plain text for modifying) should be implemented to be supported via command line.
- Import/export option, backup option.
  - along with export to csv that allows direct import into bitwarden vault.
- Take backup during deleting records till its well tested.
- Security feature: change key and keep only one back up

#### Q&A
Q: Enter key on startup but this can be risky as well. What's best for now?  
Ans: Use SSL and retrieve the key from a secure site.  

Q: If someone gets the software they can easily access the software as mine.  
Ans: Verify PC with MAC access. Update MAC address from time to time.

**Plans Discarded for now**  
- Change on data structure: instead of siteURL there should be two members: site and loginURL. 
- No siteURL stays the same: use loginURL on site when applicable. Put site name on title
- If an account has multiple credential keys store them separating using semicolon
- It should ask for password once in every five minutes; if the call is from powershell probably we ask only once for a session.
  - For some accounts, passwords will always be asked… such as bank accounts and master accounts.

Every month we run this,

    $ PassMan CreateBackup

This should give us a file and a key.

Guidelines
Keys will not reside in plain text files. Backup dbs should be stored in protected vaults…
Backups must not be stored in regular accounts.

Create plain-text database export,

    $ PassMan Export

ref, previous binary name: CredMan

Be cautious aboue the plain text export.

ref, Credentials Manager Software doc, 2014-06


**Refs**  
- Initial driver/demo of Rijndael is in [Supplement](https://github.com/atiq-cs/PasswordManager/blob/dev/Supplement.md).