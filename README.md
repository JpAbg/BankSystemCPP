Bank Account System (C++)

Author: Jean Paul Abou Gharib

Description:
This is a console-based bank account system written in C++.
The program uses classes and binary files to store bank accounts.
Data is saved in a file so it is not lost when the program closes.

Features:
- Create a new bank account
- Login using account number and PIN
- Check account balance
- Deposit money
- Withdraw money
- Display account information
- Change account PIN
- Logout

Admin Features:
- Admin login
- Display all accounts
- Search for an account by account number
- Delete an account

File Storage:
- Accounts are stored in a binary file named: BAS.dat
- The file is created automatically if it does not exist

Admin Login:
- Account Number: 0000 0000 0000 0000
- PIN: User

Compilation:
Compile using a C++ compiler such as g++

Example:
g++ main.cpp -o bank
./bank

Notes:
- PIN must be 4 digits
- Deposit and withdrawal amounts must be positive
- Withdrawals check for sufficient balance
- This project is for learning purposes (OOP and file handling)
