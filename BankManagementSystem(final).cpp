#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;
const int MAX_ACCOUNTS = 100; // Maximum number of accounts

// Class to represent a transaction
class Transaction {
public:
    string type;
    double amount;

    Transaction(string t = "", double a = 0.0) : type(t), amount(a) {}
};

// Class to represent a bank account
class BankAccount {
private:
    string accountNumber;
    string owner;
    double balance;
    Transaction transactions[MAX_ACCOUNTS];
    int transactionCount;

public:
    BankAccount(string accNum = "", string own = "", double bal = 0.0)
        : accountNumber(accNum), owner(own), balance(bal), transactionCount(0) {}

    void deposit(double amount) {
        balance += amount;
        if (transactionCount < MAX_ACCOUNTS) {
            transactions[transactionCount++] = Transaction("Deposit", amount);
        }
        displayBalance(); // Show balance after deposit
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            if (transactionCount < MAX_ACCOUNTS) {
                transactions[transactionCount++] = Transaction("Withdrawal", amount);
            }
            displayBalance(); // Show balance after withdrawal
        } else {
            throw runtime_error("Insufficient funds!");
        }
    }

    void display() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Owner: " << owner << endl;
        cout << "Balance: " << balance << endl;
    }

    void displayBalance() const {
        cout << "Current Balance: " << balance << endl;
    }

    string getAccountNumber() const { return accountNumber; }
    string getOwner() const { return owner; }
    double getBalance() const { return balance; }
};

// Class to handle file operations
class FileHandler {
public:
    static void saveAccount(const BankAccount& account) {
        ofstream file("accounts.txt", ios::app);
        if (file.is_open()) {
            file << account.getAccountNumber() << " "
                 << account.getOwner() << " "
                 << account.getBalance() << endl;
            file.close();
        } else {
            throw runtime_error("Unable to open file for writing.");
        }
    }

    static void loadAccounts(BankAccount accounts[], int& accountCount) {
        ifstream file("accounts.txt");
        if (file.is_open()) {
            string accNum, owner;
            double balance;
            while (file >> accNum >> owner >> balance) {
                if (accountCount < MAX_ACCOUNTS) {
                    accounts[accountCount++] = BankAccount(accNum, owner, balance);
                }
            }
            file.close();
        } else {
            throw runtime_error("Unable to open file for reading.");
        }
    }
};

// Function to create a new bank account
void createAccount(BankAccount accounts[], int& accountCount) {
    string accNum, owner;
    double balance;
    cout << "Enter account number: ";
    cin >> accNum;
    cin.ignore(); // Ignore the newline character left in the buffer
    cout << "Enter owner name: ";
    getline(cin, owner); // Use getline to read the entire line
    cout << "Enter initial balance: ";
    cin >> balance;
    if (accountCount < MAX_ACCOUNTS) {
        accounts[accountCount] = BankAccount(accNum, owner, balance);
        FileHandler::saveAccount(accounts[accountCount]);
        accountCount++;
        cout << "Account created successfully!" << endl;
    } else {
        cout << "Maximum account limit reached!" << endl;
    }
}

// Function to deposit money into an account
void depositToAccount(BankAccount accounts[], int accountCount) {
    string accNum;
    double amount;
    cout << "Enter account number: ";
    cin >> accNum;
    cout << "Enter amount to deposit: ";
    cin >> amount;

    // Find the account
    for (int i = 0; i < accountCount; ++i) {
        if (accounts[i].getAccountNumber() == accNum) {
            accounts[i].deposit(amount);
            cout << "Deposit successful!" << endl;
            return;
        }
    }
    cout << "Account not found!" << endl;
}

// Function to withdraw money from an account
void withdrawFromAccount(BankAccount accounts[], int accountCount) {
    string accNum;
    double amount;
    cout << "Enter account number: ";
    cin >> accNum;
    cout << "Enter amount to withdraw: ";
    cin >> amount;

    // Find the account
    for (int i = 0; i < accountCount; ++i) {
        if (accounts[i].getAccountNumber() == accNum) {
            try {
                accounts[i].withdraw(amount);
                cout << "Withdrawal successful!" << endl;
            } catch (const runtime_error& e) {
                cout << e.what() << endl;
            }
            return;
        }
    }
    cout << "Account not found!" << endl;
}

// Function to display an account
void displayAccount(BankAccount accounts[], int accountCount) {
    string accNum;
    cout << "Enter account number: ";
    cin >> accNum;

    // Find the account
    for (int i = 0; i < accountCount; ++i) {
        if (accounts[i].getAccountNumber() == accNum) {
            accounts[i].display();
            return;
        }
    }
    cout << "Account not found!" << endl;
}

// Class to handle admin operations
class Admin {
public:
    bool isAlphaString(const string &str) {
        for (size_t i = 0; i < str.length(); ++i) {
            if (!isalpha(str[i])) {
                return false;
            }
        }
        return true;
    }

    bool isDigitString(const string &str) {
        for (size_t i = 0; i < str.length(); ++i) {
            if (!isdigit(str[i])) {
                return false;
            }
        }
        return true;
    }

    void login(BankAccount accounts[], int& accountCount) {
        cout << "------------------------------------Admin Login-------------------------------" << endl;
        string user_name;
        string pass_str; // change pass to string to check if it contains only digits
        const string correct_Name = "Admin";
        cout << "Enter User Name: ";
        cin >> user_name;
        if (user_name == correct_Name) {
            cout << endl << "Enter Password: ";
            cin >> pass_str;
            // validation for pass_str using isDigitString
            if (isDigitString(pass_str)) {
                int pass;
                stringstream(pass_str) >> pass; // convert pass_str to an integer
                if (pass == 123) {
                    cout << "*******************WELCOME ADMIN********************" << endl;
                    showMenu(accounts, accountCount);
                } else {
                    cout << "Incorrect Password" << endl;
                }
            } else {
                cout << "Password should contain only digits" << endl;
            }
        } else {
            cout << "Incorrect User Name, enter the correct one" << endl;
        }
    }

    void showMenu(BankAccount accounts[], int& accountCount) {
        bool condition = true;
        while (condition) {
            int choice;
            cout << "1. Create Account" << endl;
            cout << "2. Deposit" << endl;
            cout << "3. Withdraw" << endl;
            cout << "4. Display Account" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    createAccount(accounts, accountCount);
                    break;
                case 2:
                    depositToAccount(accounts, accountCount);
                    break;
                case 3:
                    withdrawFromAccount(accounts, accountCount);
                    break;
                case 4:
                    displayAccount(accounts, accountCount);
                    break;
                case 5:
                    condition = false;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        }
    }
};

// Class to handle user operations
class User {
public:
    void showMenu(BankAccount accounts[], int accountCount) {
        bool condition = true;
        while (condition) {
            int choice;
            cout << "1. Display Balance" << endl;
            cout << "2. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    displayAccount(accounts, accountCount);
                    break;
                case 2:
                    cout << "Exiting..." << endl;
                    condition = false;
                    break;
                default:
                    cout << "Invalid choice!" << endl;
            }
        }
    }
};

int main() {
    BankAccount accounts[MAX_ACCOUNTS];
    int accountCount = 0;
    Admin admin;
    User user;

    bool condition = true;
    while (condition) {
        int ch;
        cout << "****************WELCOME TO BANK MANAGEMENT SYSTEM****************************************" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. User Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> ch;
        switch (ch) {
            case 1:
                admin.login(accounts, accountCount);
                break;
            case 2:
                user.showMenu(accounts, accountCount);
                break;
            case 3: condition=false;
            break;
            default:cout<<"invalid option"<<endl;
            break;
        }
    }
    return 0;
}
               

