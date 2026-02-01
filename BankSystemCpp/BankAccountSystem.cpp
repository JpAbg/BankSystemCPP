#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>
#include<ctime>
#include<iomanip>
using namespace std;
//Binary save file BAS.dat
string filename = "BAS.dat";
//BA: Bank Account
class BA{
private:

    string name;
    string accountNumber;
    string pin;
    double balance = 0;
    string email;
    string phoneNumber;

public:

    BA();

    void deposit(double amount){
        balance += amount;
    }

    void withdraw(double amount){
        if(balance >= amount)
            balance -= amount;
        else
            cout << "Insufficient balace!";
    }
   double get_balance(){ return balance; }

    void set_name(string fullName){
        name = fullName;
    }
    string get_name(){ return name; }

    void change_pin(string pinNb, string oldPin){
        if(oldPin == pin)
            pin = pinNb;
    }

    void set_pin(string pinNb){
            pin = pinNb;
    }
    string get_pin(){ return pin; }

    string set_accountNumber(){
        accountNumber.clear();
        bool hasNonZero = false;
        for(int group = 0; group < 4; ++group){
            for(int d = 0; d < 4; ++d){
                char c = char('0' + rand() % 10);
                if(c != '0') 
                    hasNonZero = true;
                accountNumber += c;
            }
            if(group < 3)
                accountNumber += ' ';
        }
        if(!hasNonZero){
            //ensure not all zeros: force first digit to 1-9
            accountNumber[0] = char('1' + rand() % 9);
        }
        return accountNumber;
    }
    string get_accountNumber(){ return accountNumber; }

    void set_accountNumber_fromFile(const string &num) {
        accountNumber = num;
    }

    //PN: Phone Number
    void set_PhoneNb(string PN){
        phoneNumber = PN;
    }
    string get_PhoneNb(){ return phoneNumber; }

    void set_email(string mail){
        email = mail;
    }
    string get_email(){ return email; }

    void display(){
        cout << "Client:" << name << "\n";
        cout << "Account number:" << accountNumber << "\n";
        cout << "Pin:" << pin << "\n";
        cout << "Email:" << email << "\n";
        cout << "Phone number:" << phoneNumber << "\n";
        cout << "Balance:" << balance << "$\n";
    }

    void writeToFile(const string& filename){
        ofstream out(filename, ios::binary | ios::app);
        size_t len;

        //Write name
        len = name.size();
        out.write(reinterpret_cast<char*>(&len), sizeof(len));
        out.write(name.c_str(), len);

        //Write accountNumber
        len = accountNumber.size();
        out.write(reinterpret_cast<char*>(&len), sizeof(len));
        out.write(accountNumber.c_str(), len);

        //Write pin
        len = pin.size();
        out.write(reinterpret_cast<char*>(&len), sizeof(len));
        out.write(pin.c_str(), len);

        //Write balance
        out.write(reinterpret_cast<char*>(&balance), sizeof(balance));

        //Write email
        len = email.size();
        out.write(reinterpret_cast<char*>(&len), sizeof(len));
        out.write(email.c_str(), len);

        //Write phoneNumber
        len = phoneNumber.size();
        out.write(reinterpret_cast<char*>(&len), sizeof(len));
        out.write(phoneNumber.c_str(), len);

        out.close();
    }

    void updateToFile(const string& filename){
        ifstream file(filename, ios::binary);
        if(!file) return;

        ofstream temp("temp.dat", ios::binary);
        bool found = false;

        while(!file.eof()){
            size_t len;
            string tempName, tempAccountNumber, tempPin, tempEmail, tempPhoneNumber;
            double tempBalance;

            if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
            tempName.resize(len);
            file.read(&tempName[0], len);

            if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
            tempAccountNumber.resize(len);
            file.read(&tempAccountNumber[0], len);

            if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
            tempPin.resize(len);
            file.read(&tempPin[0], len);

            if(!file.read(reinterpret_cast<char*>(&tempBalance), sizeof(tempBalance))) break;

            if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
            tempEmail.resize(len);
            file.read(&tempEmail[0], len);

            if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
            tempPhoneNumber.resize(len);
            file.read(&tempPhoneNumber[0], len);

            if(tempAccountNumber == accountNumber){
                found = true;
                //Write updated account
                len = name.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(name.c_str(), len);

                len = accountNumber.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(accountNumber.c_str(), len);

                len = pin.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(pin.c_str(), len);

                temp.write(reinterpret_cast<char*>(&balance), sizeof(balance));

                len = email.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(email.c_str(), len);

                len = phoneNumber.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(phoneNumber.c_str(), len);
            } else {
                //Write unchanged account
                len = tempName.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(tempName.c_str(), len);

                len = tempAccountNumber.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(tempAccountNumber.c_str(), len);

                len = tempPin.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(tempPin.c_str(), len);

                temp.write(reinterpret_cast<char*>(&tempBalance), sizeof(tempBalance));

                len = tempEmail.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(tempEmail.c_str(), len);

                len = tempPhoneNumber.size();
                temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                temp.write(tempPhoneNumber.c_str(), len);
            }
        }
        file.close();
        temp.flush();
        temp.close();

        if(found){
            remove(filename.c_str());
            int renameResult = rename("temp.dat", filename.c_str());
            if(renameResult != 0){
                cerr << "Error updating account file!" << endl;
            }
        } else {
            remove("temp.dat");
        }
    }
};

//Constructor implementation
BA::BA() {
    name = "";
    accountNumber = "";
    pin = "";
    balance = 0;
    email = "";
    phoneNumber = "";
}

//Checks if a string is all numbers
bool isAllDigit(string pin){
    for(char c : pin){
        if(!isdigit(c))
            return false;
    }
    return true;
    
}

//login function for the login page
int login(const string &filename, BA &acc){
    const string ADMIN_ACC = "0000 0000 0000 0000";
    const string ADMIN_PASS = "User";
    ifstream file(filename, ios::binary);
    if(!file){
        cerr << "Failed to open file." << endl;
        return 0;
    }

    string accNum, inputPin;
    cout << "Account Number: ";
    getline(cin, accNum);
    cout << "PIN: ";
    getline(cin, inputPin);

    //Check admin login
    if(accNum == ADMIN_ACC && inputPin == ADMIN_PASS) {
        return 2;
    }

    while(!file.eof()){

        //Read name
        size_t len;
        if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        string name(len, ' ');
        file.read(&name[0], len);

        //Read accountNumber
        if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        string accountNumber(len, ' ');
        file.read(&accountNumber[0], len);

        //Read pin
        if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        string pin(len, ' ');
        file.read(&pin[0], len);

        //Read balance
        double balance;
        if(!file.read(reinterpret_cast<char*>(&balance), sizeof(balance))) break;

        //Read email
        if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        string email(len, ' ');
        file.read(&email[0], len);

        //Read phoneNumber
        if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
        string phoneNumber(len, ' ');
        file.read(&phoneNumber[0], len);

        //Check login
        if(accountNumber == accNum && pin == inputPin) {
            acc.set_name(name);
            acc.set_accountNumber_fromFile(accountNumber);
            acc.set_pin(pin);
            acc.deposit(balance);
            acc.set_email(email);
            acc.set_PhoneNb(phoneNumber);

            file.close();
            return 1;
        }
    }

    file.close();
    cout << "Incorrect account number or PIN." << endl;
    return 0;
}

void accountMenu(BA &acc);
void adminMenu(const string &filename);

//=====Bank System=====//
void loginPage(){

    int choice;
    BA acc;

    do{
        cout << "=============================\n";
        cout << "       Bank System     \n";
        cout << "=============================\n";
        cout << "1. Create new account\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice){

            case 1: {
                string name, email, phoneNb, pin, PIN;

                do{
                    cout << "Enter your full name: ";
                    getline(cin, name);
                    if(name.length() < 5)
                        cout << "Name too short!" << endl;
                }
                while(name.length() < 5);
                acc.set_name(name);

                do{
                    cout << "Enter your email: ";
                    cin >> email;
                    if(email.length() == 0)
                        cout << "Invalid emal!" << endl;
                }
                while(email.length() == 0);
                acc.set_email(email);

                do{
                    cout << "Enter your phone number(no spaces): \n";
                    cout << "+961 ";
                    cin >> phoneNb;
                    if(phoneNb.length() != 8)
                        cout << "Invalid phone number!";
                }
                while(phoneNb.length() != 8);
                acc.set_PhoneNb(phoneNb);

                do{
                    cout << "Enter your pin(4 digits): ";
                    cin >> pin;
                    if(!isAllDigit(pin)){
                        cout << "The pin must contain digits only!\n";
                        continue;
                    }
                    else if(pin.length() != 4){
                        cout << "The pin is too short!\n";
                        continue;
                    }
                    cout << "Confirm your pin: ";
                    cin >> PIN;
                    if(pin == PIN)
                        acc.set_pin(pin);
                    else{
                        cout << "Pins don't match\n";
                        pin = "";
                    }
                }
                while(pin.length() != 4 || !isAllDigit(pin));

                cout << "Account Number: " << acc.set_accountNumber() << "\n";
                cout << "Account succefully created.\n" << endl;
                acc.writeToFile(filename);
                cin.ignore();
                break;
                }

            //login
            case 2: { 
                int loginResult = login(filename, acc); // returns 0=fail, 1=user, 2=admin

                if(loginResult == 1) {
                    cout << "\nUser login successful!\n";
                    accountMenu(acc); //your account menu for user
                } 
                else if(loginResult == 2) {
                    cout << "\nAdmin login successful!\n";
                    adminMenu(filename); //admin menu for managing accounts
                } 
                else {
                    cout << "Login failed.\n";
                }
                break;
            }

            case 3: {
                cout << "Exiting Bank System...\n";
                break;
            }

            default:{
                cout << "Invalid choice. Try again.\n";
                break;
            }

        } 
    } while(choice != 3);
}

//Account Menu//
void accountMenu(BA &acc){

    int choice;
    do{
        cout << "\n=============================\n";
        cout << "       Account Menu     \n";
        cout << "=============================\n";
        cout << "1. Check balance\n";
        cout << "2. Deposit money\n";
        cout << "3. Withdraw money\n";
        cout << "4. Display account information\n";
        cout << "5. Change PIN\n";
        cout << "6. Logout\n";
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;

        switch(choice){

            case 1: {
                cout << "Your balance: " << acc.get_balance() << "$\n";
                break;
            }

            case 2: {
                double amount;
                do{
                    cout << "Enter amount to deposit: ";
                    cin >> amount;
                    if(amount <= 0)
                        cout << "Invalid amount!\n";
                } while(amount <= 0);
                acc.deposit(amount);
                acc.updateToFile(filename);
                cout << "Deposited " << amount << "$ successfully!\n";
                break;

            }

            case 3: {
                double amount;
                do{
                    cout << "Enter amount to withdraw: ";
                    cin >> amount;
                    if(amount <= 0)
                        cout << "Invalid amount!\n";
                    else if(acc.get_balance() < amount){
                        cout << "Insufficient balance!\n";
                    }
                } while(amount <= 0 || acc.get_balance() < amount);
                acc.withdraw(amount);
                acc.updateToFile(filename);
                cout << "Withdrawn " << amount << "$ successfully!\n";
                break;
            }

            case 4: {
                acc.display();
                break;
            }

            case 5: {
                string oldPin, newPin, confirmPin;
                cout << "Enter old PIN: ";
                cin >> oldPin;
                cout << "Enter new PIN: ";
                cin >> newPin;
                cout << "Confirm new PIN: ";
                cin >> confirmPin;
                if(newPin == confirmPin){
                    acc.change_pin(newPin, oldPin);
                    acc.updateToFile(filename);
                    cout << "PIN changed successfully!\n";
                } else
                    cout << "PIN mismatch!\n";
                break;
            }

            case 6: {
                cout << "Logging out...\n";
                break;
            }

            default:
                cout << "Invalid choice!\n";
        }

    } while(choice != 6);
}

//Admin Menu//
void adminMenu(const string &filename){

    int choice;
    do{
        cout << "\n===== ADMIN MENU =====\n";
        cout << "1. Display all accounts\n";
        cout << "2. Search account by number\n";
        cout << "3. Delete an account\n";
        cout << "4. Exit admin menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        cout << endl;

        switch(choice){

            //Display all accounts
            case 1: { 
                ifstream file(filename, ios::binary);
                if(!file) {
                    cerr << "Failed to open file.\n";
                    break;
                }

                cout << "\n===== All Accounts =====\n";
                bool anyAccount = false;
                while(!file.eof()){
                    size_t len;
                    string name, accountNumber, pin, email, phoneNumber;
                    double balance;

                    //Read Name
                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    name.resize(len);
                    file.read(&name[0], len);

                    //Read Account Number
                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    accountNumber.resize(len);
                    file.read(&accountNumber[0], len);

                    //Read PIN
                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    pin.resize(len);
                    file.read(&pin[0], len);

                    //Read balance
                    if(!file.read(reinterpret_cast<char*>(&balance), sizeof(balance))) break;

                    //Read email
                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    email.resize(len);
                    file.read(&email[0], len);

                    //Read phoneNumber
                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    phoneNumber.resize(len);
                    file.read(&phoneNumber[0], len);

                    //Display account info
                    anyAccount = true;
                    cout << "------------------------\n";
                    cout << "Name: " << name << "\n";
                    cout << "Account Number: " << accountNumber << "\n";
                    cout << "PIN: " << pin << "\n";
                    cout << "Balance: " << balance << "$\n";
                    cout << "Email: " << email << "\n";
                    cout << "Phone: " << phoneNumber << "\n";
                }
                if(!anyAccount) cout << "No accounts available. Please create an account to view it here.\n";
                file.close();
                break;
            }

            //Search account by number
            case 2: {
                string searchAcc;
                cout << "Enter account number to search: ";
                getline(cin, searchAcc);

                ifstream file(filename, ios::binary);
                if(!file){ 
                    cerr << "Failed to open file.\n"; 
                    break; 
                }

                bool found = false;

                while(!file.eof()){
                    size_t len;
                    string name, accountNumber, pin, email, phoneNumber;
                    double balance;

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    name.resize(len); file.read(&name[0], len);

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    accountNumber.resize(len); file.read(&accountNumber[0], len);

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    pin.resize(len); file.read(&pin[0], len);

                    if(!file.read(reinterpret_cast<char*>(&balance), sizeof(balance))) break;

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    email.resize(len); file.read(&email[0], len);

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    phoneNumber.resize(len); file.read(&phoneNumber[0], len);

                    if(accountNumber == searchAcc){
                        found = true;
                        cout << "------------------------\n";
                        cout << "Name: " << name << "\n";
                        cout << "Account Number: " << accountNumber << "\n";
                        cout << "PIN: " << pin << "\n";
                        cout << "Balance: " << balance << "$\n";
                        cout << "Email: " << email << "\n";
                        cout << "Phone: " << phoneNumber << "\n";
                        break;
                    }
                }

                if(!found) cout << "Account not found.\n";
                file.close();
                break;
            }

            //Delete account
            case 3: {
                string accToDelete;
                cout << "Enter account number to delete: ";
                getline(cin, accToDelete);

                ifstream file(filename, ios::binary);
                if(!file) { cerr << "Failed to open file.\n"; break; }

                ofstream temp("temp.dat", ios::binary);
                if(!temp) { cerr << "Failed to create temp file.\n"; break; }

                bool found = false;

                while(!file.eof()){
                    size_t len;
                    string name, accountNumber, pin, email, phoneNumber;
                    double balance;

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    name.resize(len); file.read(&name[0], len);

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    accountNumber.resize(len); file.read(&accountNumber[0], len);

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    pin.resize(len); file.read(&pin[0], len);

                    if(!file.read(reinterpret_cast<char*>(&balance), sizeof(balance))) break;

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    email.resize(len); file.read(&email[0], len);

                    if(!file.read(reinterpret_cast<char*>(&len), sizeof(len))) break;
                    phoneNumber.resize(len); file.read(&phoneNumber[0], len);

                    //Skip the account to delete
                    if(accountNumber == accToDelete) { found = true; continue; }

                    //Write other accounts to temp
                    len = name.size(); temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                    temp.write(name.c_str(), len);

                    len = accountNumber.size(); temp.write(reinterpret_cast<char*>(&len), sizeof(len));
                    temp.write(accountNumber.c_str(), len);

                    len = pin.size(); temp.write(reinterpret_cast<char*>(&len), sizeof(len)); 
                    temp.write(pin.c_str(), len);

                    temp.write(reinterpret_cast<char*>(&balance), sizeof(balance));
                    len = email.size(); temp.write(reinterpret_cast<char*>(&len), sizeof(len)); 
                    temp.write(email.c_str(), len);

                    len = phoneNumber.size(); temp.write(reinterpret_cast<char*>(&len), sizeof(len)); 
                    temp.write(phoneNumber.c_str(), len);
                }

                file.close(); 
                temp.close();

                if(found){
                    remove(filename.c_str());
                    rename("temp.dat", filename.c_str());
                    cout << "Account deleted successfully.\n";
                } else {
                    remove("temp.dat");
                    cout << "Account not found.\n";
                }
                break;
            }

            case 4:
                cout << "Exiting admin menu...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while(choice != 4);

}

int main(){

    srand(time(0));

    cout << fixed << setprecision(2);

    int choice;
    BA acc;

    //Call the bank system menu
    loginPage();
    //The login page handles everything

    return 0;

}