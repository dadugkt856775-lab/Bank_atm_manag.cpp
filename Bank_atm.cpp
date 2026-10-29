
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

class Transaction {
public:
    string type;
    double amount;
    double balanceAfter;

    Transaction(string t, double a, double b) {
        type = t;
        amount = a;
        balanceAfter = b;
    }

    void display() const {
        cout << left << setw(20) << type
             << "Rs. " << setw(12) << fixed
             << setprecision(2) << amount
             << "Balance: Rs. " << balanceAfter << endl;
    }
};

class Account {
public:
    int accountNumber;
    string holderName;
    int pin;
    double balance;
    vector<Transaction> history;

    Account(int number, string name, int accountPin,
            double initialBalance) {
        accountNumber = number;
        holderName = name;
        pin = accountPin;
        balance = initialBalance;
        history.emplace_back("Account Opening",
                             initialBalance, balance);
    }

    void addTransaction(string type, double amount) {
        history.emplace_back(type, amount, balance);
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Enter a valid positive amount!\n";
            return;
        }

        balance += amount;
        addTransaction("Deposit", amount);
        cout << "Deposit successful!\n";
        cout << "New balance: Rs. " << fixed
             << setprecision(2) << balance << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Enter a valid positive amount!\n";
        } else if (amount > balance) {
            cout << "Insufficient balance!\n";
        } else {
            balance -= amount;
            addTransaction("Withdrawal", amount);
            cout << "Please collect your cash.\n";
            cout << "Remaining balance: Rs. "
                 << fixed << setprecision(2)
                 << balance << endl;
        }
    }

    void displayBalance() const {
        cout << "\nAccount Number: " << accountNumber << endl;
        cout << "Account Holder: " << holderName << endl;
        cout << "Available Balance: Rs. "
             << fixed << setprecision(2)
             << balance << endl;
    }

    void displayHistory() const {
        cout << "\n========== TRANSACTION HISTORY ==========\n";

        if (history.empty()) {
            cout << "No transactions available.\n";
            return;
        }

        for (const Transaction& transaction : history)
            transaction.display();
    }
};

class ATMSystem {
private:
    vector<Account> accounts;
    int nextAccountNumber;

    int findAccount(int number) const {
        for (int i = 0; i < (int)accounts.size(); i++) {
            if (accounts[i].accountNumber == number)
                return i;
        }
        return -1;
    }

    int authenticate() const {
        int number, enteredPin;

        cout << "Enter account number: ";
        cin >> number;

        int index = findAccount(number);

        if (index == -1) {
            cout << "Account not found!\n";
            return -1;
        }

        cout << "Enter 4-digit PIN: ";
        cin >> enteredPin;

        if (enteredPin != accounts[index].pin) {
            cout << "Incorrect PIN!\n";
            return -1;
        }

        return index;
    }

public:
    ATMSystem() {
        nextAccountNumber = 10001;
    }

    void createAccount() {
        string name;
        int pin;
        double initialBalance;

        cout << "\nEnter account holder name: ";
        getline(cin >> ws, name);

        cout << "Create a 4-digit PIN: ";
        cin >> pin;

        if (pin < 1000 || pin > 9999) {
            cout << "PIN must contain exactly 4 digits!\n";
            return;
        }

        cout << "Enter initial deposit (Rs.): ";
        cin >> initialBalance;

        if (initialBalance < 0 || cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid initial deposit!\n";
            return;
        }

        accounts.emplace_back(nextAccountNumber++,
                              name, pin, initialBalance);

        cout << "\nAccount created successfully!\n";
        cout << "Your account number is: "
             << accounts.back().accountNumber << endl;
    }

    void depositMoney() {
        int index = authenticate();

        if (index == -1)
            return;

        double amount;
        cout << "Enter deposit amount: Rs. ";
        cin >> amount;

        accounts[index].deposit(amount);
    }

    void withdrawMoney() {
        int index = authenticate();

        if (index == -1)
            return;

        double amount;
        cout << "Enter withdrawal amount: Rs. ";
        cin >> amount;

        accounts[index].withdraw(amount);
    }

    void checkBalance() {
        int index = authenticate();

        if (index != -1)
            accounts[index].displayBalance();
    }

    void transferMoney() {
        int senderIndex = authenticate();

        if (senderIndex == -1)
            return;

        int receiverNumber;
        double amount;

        cout << "Enter receiver account number: ";
        cin >> receiverNumber;

        int receiverIndex = findAccount(receiverNumber);

        if (receiverIndex == -1) {
            cout << "Receiver account not found!\n";
            return;
        }

        if (receiverIndex == senderIndex) {
            cout << "Cannot transfer to the same account!\n";
            return;
        }

        cout << "Enter transfer amount: Rs. ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid transfer amount!\n";
            return;
        }

        if (amount > accounts[senderIndex].balance) {
            cout << "Insufficient balance!\n";
            return;
        }

        accounts[senderIndex].balance -= amount;
        accounts[receiverIndex].balance += amount;

        accounts[senderIndex].addTransaction(
            "Transfer Sent", amount);

        accounts[receiverIndex].addTransaction(
            "Transfer Received", amount);

        cout << "Transfer completed successfully!\n";
    }

    void viewTransactionHistory() {
        int index = authenticate();

        if (index != -1)
            accounts[index].displayHistory();
    }

    void displayAllAccounts() const {
        if (accounts.empty()) {
            cout << "No accounts created yet.\n";
            return;
        }

        cout << "\n========== ALL ACCOUNTS ==========\n";

        for (const Account& account : accounts) {
            cout << "Account Number: " << account.accountNumber
                 << " | Holder: " << account.holderName
                 << " | Balance: Rs. " << fixed
                 << setprecision(2) << account.balance << endl;
        }
    }

    void run() {
        int choice;

        do {
            cout << "\n========== ATM MANAGEMENT SYSTEM ==========\n";
            cout << "1. Create Account\n";
            cout << "2. Deposit Money\n";
            cout << "3. Withdraw Money\n";
            cout << "4. Check Balance\n";
            cout << "5. Transfer Money\n";
            cout << "6. Transaction History\n";
            cout << "7. Display All Accounts (Demo)\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "Invalid input!\n";
                break;
            }

            switch (choice) {
                case 1: createAccount(); break;
                case 2: depositMoney(); break;
                case 3: withdrawMoney(); break;
                case 4: checkBalance(); break;
                case 5: transferMoney(); break;
                case 6: viewTransactionHistory(); break;
                case 7: displayAllAccounts(); break;
                case 0:
                    cout << "Thank you for using the ATM!\n";
                    break;
                default:
                    cout << "Invalid choice! Try again.\n";
            }

        } while (choice != 0);
    }
};

int main() {
    ATMSystem atm;
    atm.run();
    return 0;
}
