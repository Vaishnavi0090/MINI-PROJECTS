#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cmath>
#include <chrono>

class Book;
class User;
class Student;
class Faculty;
class Librarian;
class Account;
class Library;
class Transaction;
class Date;

class Date {
private:
    int day;
    int month;
    int year;

public:
    Date() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        day = ltm->tm_mday;
        month = 1 + ltm->tm_mon;
        year = 1900 + ltm->tm_year;
    }

    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    Date(const std::string& dateStr) {
        std::istringstream ss(dateStr);
        char delimiter;
        ss >> day >> delimiter >> month >> delimiter >> year;
    }

    std::string toString() const {
        std::ostringstream ss;
        ss << std::setfill('0') << std::setw(2) << day << "/"
           << std::setfill('0') << std::setw(2) << month << "/"
           << year;
        return ss.str();
    }

    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator==(const Date& other) const {
        return (day == other.day && month == other.month && year == other.year);
    }

   
int daysBetweenStdLib(const Date& other) const {
    std::tm startTm = {};
    startTm.tm_year = year - 1900;
    startTm.tm_mon = month - 1;
    startTm.tm_mday = day;

    std::tm endTm = {};
    endTm.tm_year = other.year - 1900;
    endTm.tm_mon = other.month - 1;
    endTm.tm_mday = other.day;

    time_t startTime = std::mktime(&startTm);
    time_t endTime = std::mktime(&endTm);

    return static_cast<int>(std::difftime(endTime, startTime) / 86400);
}

    static Date fromString(const std::string& dateStr) {
        std::istringstream ss(dateStr);
        int d, m, y;
        char delimiter;
        ss >> d >> delimiter >> m >> delimiter >> y;
        return Date(d, m, y);
    }

    Date addDays(int days) const {
        struct tm timeinfo = {};
        timeinfo.tm_mday = day;
        timeinfo.tm_mon = month - 1;
        timeinfo.tm_year = year - 1900;
        
        time_t rawtime = mktime(&timeinfo);

        if (rawtime == -1) {
            std::cerr << "Error: Failed to compute time." << std::endl;
            return *this; // Return the same date if conversion fails
        }
        
        rawtime += days * 24 * 60 * 60;
        
        struct tm* tm_info = localtime(&rawtime);
        
        return Date(tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900);
    }

    void setDay(int d) { day = d; }
    void setMonth(int m) { month = m; }
    void setYear(int y) { year = y; }

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    void printDate() const {
        std::cout << toString() << std::endl;
    }
};

class Book {
private:
    std::string title;
    std::string author;
    std::string publisher;
    int year;
    std::string ISBN;
    std::string status; // "Available", "Borrowed", or "Reserved"
    int bookId;

public:
    Book() : year(0), bookId(0), status("Available") {}

    Book(int id, const std::string& t, const std::string& a, const std::string& p,
         int y, const std::string& isbn)
        : bookId(id), title(t), author(a), publisher(p), year(y), ISBN(isbn), status("Available") {}

    int getBookId() const { return bookId; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getPublisher() const { return publisher; }
    int getYear() const { return year; }
    std::string getISBN() const { return ISBN; }
    std::string getStatus() const { return status; }

    void setBookId(int id) { bookId = id; }
    void setTitle(const std::string& t) { title = t; }
    void setAuthor(const std::string& a) { author = a; }
    void setPublisher(const std::string& p) { publisher = p; }
    void setYear(int y) { year = y; }
    void setISBN(const std::string& isbn) { ISBN = isbn; }
    void setStatus(const std::string& s) { status = s; }

    void displayBookId() const { std::cout << "Book ID: " << bookId << std::endl; }
    void displayTitle() const { std::cout << "Title: " << title << std::endl; }
    void displayAuthor() const { std::cout << "Author: " << author << std::endl; }
    void displayPublisher() const { std::cout << "Publisher: " << publisher << std::endl; }
    void displayYear() const { std::cout << "Year: " << year << std::endl; }
    void displayISBN() const { std::cout << "ISBN: " << ISBN << std::endl; }
    void displayStatus() const { std::cout << "Status: " << status << std::endl; }

    void displayDetails() const {
        std::cout << "\n----- Book Details -----" << std::endl;
        displayBookId();
        displayTitle();
        displayAuthor();
        displayPublisher();
        displayYear();
        displayISBN();
        displayStatus();
        std::cout << "------------------------\n" << std::endl;
    }

    std::string toString() const {
        std::ostringstream ss;
        ss << bookId << "|" << title << "|" << author << "|"
           << publisher << "|" << year << "|" << ISBN << "|"
           << status;
        return ss.str();
    }

    static Book fromString(const std::string& str) {
        std::istringstream ss(str);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 7) {
            Book book;
            book.setBookId(std::stoi(tokens[0]));
            book.setTitle(tokens[1]);
            book.setAuthor(tokens[2]);
            book.setPublisher(tokens[3]);
            book.setYear(std::stoi(tokens[4]));
            book.setISBN(tokens[5]);
            book.setStatus(tokens[6]);
            return book;
        }

        return Book(); // Return default book if parsing fails
    }
};

class Transaction {
private:
    int transactionId;
    int userId;
    int bookId;
    Date borrowDate;
    Date dueDate;
    Date returnDate;
    bool isReturned;
    double fine;

public:
    Transaction() : transactionId(0), userId(0), bookId(0), isReturned(false), fine(0) {}

    Transaction(int tid, int uid, int bid, const Date& bDate, const Date& dDate)
        : transactionId(tid), userId(uid), bookId(bid), borrowDate(bDate), dueDate(dDate),
          isReturned(false), fine(0) {}

    int getTransactionId() const { return transactionId; }
    int getUserId() const { return userId; }
    int getBookId() const { return bookId; }
    Date getBorrowDate() const { return borrowDate; }
    Date getDueDate() const { return dueDate; }
    Date getReturnDate() const { return returnDate; }
    bool getIsReturned() const { return isReturned; }
    double getFine() const { return fine; }

    void setTransactionId(int id) { transactionId = id; }
    void setUserId(int id) { userId = id; }
    void setBookId(int id) { bookId = id; }
    void setBorrowDate(const Date& date) { borrowDate = date; }
    void setDueDate(const Date& date) { dueDate = date; }
    void setReturnDate(const Date& date) { returnDate = date; }
    void setIsReturned(bool returned) { isReturned = returned; }
    void setFine(double f) { fine = f; }

    void displayTransactionId() const { std::cout << "Transaction ID: " << transactionId << std::endl; }
    void displayUserId() const { std::cout << "User ID: " << userId << std::endl; }
    void displayBookId() const { std::cout << "Book ID: " << bookId << std::endl; }
    void displayBorrowDate() const {
        std::cout << "Borrow Date: ";
        borrowDate.printDate();
    }
    void displayDueDate() const {
        std::cout << "Due Date: ";
        dueDate.printDate();
    }
    void displayReturnDate() const {
        if (isReturned) {
            std::cout << "Return Date: ";
            returnDate.printDate();
        } else {
            std::cout << "Not yet returned" << std::endl;
        }
    }
    void displayIsReturned() const { std::cout << "Returned: " << (isReturned ? "Yes" : "No") << std::endl; }
    void displayFine() const { std::cout << "Fine: ₹" << fine << std::endl; }

    void displayDetails() const {
        std::cout << "\n----- Transaction Details -----" << std::endl;
        displayTransactionId();
        displayUserId();
        displayBookId();
        displayBorrowDate();
        displayDueDate();
        displayIsReturned();
        if (isReturned) {
            displayReturnDate();
        }
        displayFine();
        std::cout << "-------------------------------\n" << std::endl;
    }

    std::string toString() const {
        std::ostringstream ss;
        ss << transactionId << "|" << userId << "|" << bookId << "|"
           << borrowDate.toString() << "|" << dueDate.toString() << "|";

        if (isReturned) {
            ss << returnDate.toString();
        } else {
            ss << "NA";
        }

        ss << "|" << (isReturned ? "1" : "0") << "|" << fine;
        return ss.str();
    }

    static Transaction fromString(const std::string& str) {
        std::istringstream ss(str);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 8) {
            Transaction transaction;
            transaction.setTransactionId(std::stoi(tokens[0]));
            transaction.setUserId(std::stoi(tokens[1]));
            transaction.setBookId(std::stoi(tokens[2]));
            transaction.setBorrowDate(Date::fromString(tokens[3]));
            transaction.setDueDate(Date::fromString(tokens[4]));

            if (tokens[5] != "NA") {
                transaction.setReturnDate(Date::fromString(tokens[5]));
            }

            transaction.setIsReturned(tokens[6] == "1");
            transaction.setFine(std::stod(tokens[7]));
            return transaction;
        }

        return Transaction(); // Return default transaction if parsing fails
    }
};

class Student;
class Faculty;
class Librarian;

class User {
protected:
    int userId;
    std::string name;
    std::string email;
    std::string password;
    std::string role; // "Student", "Faculty", or "Librarian"

public:
    User() : userId(0) {}

    User(int id, const std::string& n, const std::string& e, const std::string& p, const std::string& r)
        : userId(id), name(n), email(e), password(p), role(r) {}


    int getUserId() const { return userId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPassword() const { return password; }
    std::string getRole() const { return role; }

    void setUserId(int id) { userId = id; }
    void setName(const std::string& n) { name = n; }
    void setEmail(const std::string& e) { email = e; }
    void setPassword(const std::string& p) { password = p; }
    void setRole(const std::string& r) { role = r; }

    void displayUserId() const { std::cout << "User ID: " << userId << std::endl; }
    void displayName() const { std::cout << "Name: " << name << std::endl; }
    void displayEmail() const { std::cout << "Email: " << email << std::endl; }
    void displayRole() const { std::cout << "Role: " << role << std::endl; }

    virtual int getMaxBooks() const = 0;
    virtual int getMaxBorrowDays() const = 0;
    virtual double calculateFine(const Date& dueDate, const Date& returnDate) const = 0;
    virtual bool canBorrow(int currentBorrowed, double fineAmount, const std::vector<Transaction>& transactions) const = 0;

    virtual void displayDetails() const {
        std::cout << "\n----- User Details -----" << std::endl;
        displayUserId();
        displayName();
        displayEmail();
        displayRole();
        std::cout << "-----------------------\n" << std::endl;
    }

    virtual std::string toString() const {
        std::ostringstream ss;
        ss << userId << "|" << name << "|" << email << "|"
           << password << "|" << role;
        return ss.str();
    }

    static User* fromString(const std::string& str);
    
    };
   

class Student : public User {
public:
    Student() { role = "Student"; }

    Student(int id, const std::string& n, const std::string& e, const std::string& p)
        : User(id, n, e, p, "Student") {}

    int getMaxBooks() const override { return 3; }
    int getMaxBorrowDays() const override { return 15; }

    double calculateFine(const Date& dueDate, const Date& returnDate) const override {
        if (returnDate < dueDate) return 0;
        int daysLate = dueDate.daysBetweenStdLib(returnDate);
        return daysLate * 10.0; // 10 rupees per day
    }

    bool canBorrow(int currentBorrowed, double fineAmount, const std::vector<Transaction>& transactions) const override {
        return currentBorrowed < getMaxBooks() && fineAmount <= 0;
    }

    void displayDetails() const override {
        User::displayDetails();
        std::cout << "Maximum books allowed: " << getMaxBooks() << std::endl;
        std::cout << "Maximum borrowing period: " << getMaxBorrowDays() << " days" << std::endl;
        std::cout << "Fine rate: ₹10 per day for overdue books" << std::endl;
    }
    
    
};

class Faculty : public User {
public:
    Faculty() { role = "Faculty"; }

    Faculty(int id, const std::string& n, const std::string& e, const std::string& p)
        : User(id, n, e, p, "Faculty") {}

    int getMaxBooks() const override { return 5; }
    int getMaxBorrowDays() const override { return 30; }

    double calculateFine(const Date& dueDate, const Date& returnDate) const override {
        return 0.0; // No fine for faculty
    }

    bool canBorrow(int currentBorrowed, double fineAmount, const std::vector<Transaction>& transactions) const override {
        if (currentBorrowed >= getMaxBooks()) {
            return false;
        }

        Date currentDate;
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) {
                Date dueDate = transaction.getDueDate();
                int daysOverdue = dueDate.daysBetweenStdLib(currentDate);
                if (daysOverdue > 60) {
                    return false;
                }
            }
        }
        
        return true;
    }

    void displayDetails() const override {
        User::displayDetails();
        std::cout << "Maximum books allowed: " << getMaxBooks() << std::endl;
        std::cout << "Maximum borrowing period: " << getMaxBorrowDays() << " days" << std::endl;
        std::cout << "Fine rate: No fine for overdue books" << std::endl;
        std::cout << "Note: Cannot borrow books if any book is overdue by more than 60 days" << std::endl;
    }
    
    
};

class Librarian : public User {
public:
    Librarian() { role = "Librarian"; }

    Librarian(int id, const std::string& n, const std::string& e, const std::string& p)
        : User(id, n, e, p, "Librarian") {}

    int getMaxBooks() const override { return 0; } // Librarians cannot borrow books
    int getMaxBorrowDays() const override { return 0; }

    double calculateFine(const Date& dueDate, const Date& returnDate) const override {
        return 0.0; // Not applicable for librarians
    }

    bool canBorrow(int currentBorrowed, double fineAmount, const std::vector<Transaction>& transactions) const override {
        return false; // Librarians cannot borrow books
    }

    void displayDetails() const override {
        User::displayDetails();
        std::cout << "Librarians can manage the library and cannot borrow books." << std::endl;
    }
    
};    
    
    User* User::fromString(const std::string& str) {
    if (str.empty()) {
        return nullptr;
    }

    std::istringstream ss(str);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(ss, token, '|')) {
        token.erase(std::remove(token.begin(), token.end(), '\r'), token.end());
        token.erase(std::remove(token.begin(), token.end(), '\n'), token.end());
        tokens.push_back(token);
    }

    if (tokens.size() < 5) {
        return nullptr;
    }

    int id = std::stoi(tokens[0]);
    std::string name = tokens[1];
    std::string email = tokens[2];
    std::string password = tokens[3];
    std::string role = tokens[4];

    if (role == "Student") {
        return new Student(id, name, email, password);
    } else if (role == "Faculty") {
        return new Faculty(id, name, email, password);
    } else if (role == "Librarian") {
        return new Librarian(id, name, email, password);
    }

    return nullptr; // If the role doesn't match
}


class Account {
private:
    int userId;
    std::vector<Transaction> transactions;
    double totalFine;

public:
    Account() : userId(0), totalFine(0) {}
    Account(int uid) : userId(uid), totalFine(0) {}

    int getUserId() const { return userId; }
    double getTotalFine() const { return totalFine; }
    const std::vector<Transaction>& getTransactions() const { return transactions; }

    void setUserId(int id) { userId = id; }
    void setTotalFine(double fine) { totalFine = fine; }

    void addTransaction(const Transaction& transaction) {
        transactions.push_back(transaction);
        if (!transaction.getIsReturned()) {
            totalFine += transaction.getFine();
        }
    }

    void updateTransaction(const Transaction& transaction) {
        for (auto& t : transactions) {
            if (t.getTransactionId() == transaction.getTransactionId()) {
                t = transaction;
                break;
            }
        }
        recalculateFine();
    }

    void recalculateFine() {
        totalFine = 0;
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) {
                totalFine += transaction.getFine();
            }
        }
    }

    bool payFine(double amount) {
        if (amount < totalFine) {
            std::cout << "You must pay the full fine amount of ₹" << totalFine << " to clear your dues.\n";
            return false;
        }
        if (amount == totalFine) {
            totalFine = 0;
            std::cout << "Fine cleared successfully!\n";
            return true;
        }
        std::cout << "Invalid payment amount.\n";
        return false;
    }

    std::vector<int> getOverdueBooks() const {
        std::vector<int> overdueBookIds;
        Date currentDate;

        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) {
                Date dueDate = transaction.getDueDate();
                if (dueDate < currentDate) {
                    overdueBookIds.push_back(transaction.getBookId());
                }
            }
        }
        return overdueBookIds;
    }

    std::vector<int> getCurrentlyBorrowedBooks() const {
        std::vector<int> bookIds;
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) {
                bookIds.push_back(transaction.getBookId());
            }
        }
        return bookIds;
    }

    int getOverduebooksCount() const {
        int count = 0;
        Date currentDate;
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) {
                Date dueDate = transaction.getDueDate();
                if (dueDate < currentDate) {
                  count++;
                }
            }
        }
        return count;
    }


    int getCurrentlyBorrowedCount() const {
        int count = 0;
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) {
                count++;
            }
        }
        return count;
    }

    std::vector<Transaction> getBorrowingHistory() const {
        std::vector<Transaction> history;
        for (const auto& transaction : transactions) {
            if (transaction.getIsReturned()) {
                history.push_back(transaction);
            }
        }
        return history;
    }

    std::vector<Transaction> getActiveTransactions() const {
        std::vector<Transaction> active;
        for (const auto& transaction : transactions) {
            if (!transaction.getIsReturned()) {
                active.push_back(transaction);
            }
        }
        return active;
    }

    void displayUserId() const { std::cout << "User ID: " << userId << std::endl; }
    void displayTotalFine() const { std::cout << "Total Fine: ₹" << totalFine << std::endl; }
    void displayOverdueBooks() const {
        std::vector<int> overdueBooks = getOverdueBooks();

        std::cout << "\n----- Overdue Books -----" << std::endl;
        if (overdueBooks.empty()) {
            std::cout << "No overdue books!" << std::endl;
        } else {
            std::cout << "Overdue Book IDs: ";
            for (size_t i = 0; i < overdueBooks.size(); ++i) {
                std::cout << overdueBooks[i];
                if (i < overdueBooks.size() - 1) std::cout << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << "--------------------------\n" << std::endl;
    }
    void displayCurrentlyBorrowed() const {
        std::cout << "Currently Borrowed Books: ";
        auto bookIds = getCurrentlyBorrowedBooks();
        if (bookIds.empty()) {
            std::cout << "None" << std::endl;
        } else {
            for (size_t i = 0; i < bookIds.size(); ++i) {
                std::cout << bookIds[i];
                if (i < bookIds.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << std::endl;
        }
    }

    void displayDetails() const {
        std::cout << "\n----- Account Details -----" << std::endl;
        displayUserId();
        displayTotalFine();
        displayOverdueBooks();
        displayCurrentlyBorrowed();
        std::cout << "Overdue books: " << getOverduebooksCount() << std::endl;
        std::cout << "Currently borrowed books: " << getCurrentlyBorrowedCount() << std::endl;
        std::cout << "---------------------------\n" << std::endl;
    }

    void displayTransactionHistory() const {
        std::cout << "\n----- Transaction History -----" << std::endl;
        if (transactions.empty()) {
            std::cout << "No transactions found." << std::endl;
        } else {
            for (const auto& transaction : transactions) {
                transaction.displayDetails();
            }
        }
        std::cout << "-------------------------------\n" << std::endl;
    }

    std::string toString() const {
        std::ostringstream ss;
        ss << userId << "|" << totalFine << "|" << transactions.size();
        return ss.str();
    }

    std::vector<std::string> getTransactionsAsStrings() const {
        std::vector<std::string> transactionStrings;
        for (const auto& transaction : transactions) {
            transactionStrings.push_back(transaction.toString());
        }
        return transactionStrings;
    }

    static Account fromString(const std::string& str) {
        std::istringstream ss(str);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 3) {
            Account account;
            account.setUserId(std::stoi(tokens[0]));
            account.setTotalFine(std::stod(tokens[1]));
            return account;
        }

        return Account(); // Return default account if parsing fails
    }
};

class Library {
private:
    std::vector<Book> books;
    std::vector<User*> users;
    std::map<int, Account> accounts;
    int nextBookId;
    int nextUserId;
    int nextTransactionId;
    User* currentUser;

    const std::string BOOKS_FILE = "books.txt";
    const std::string USERS_FILE = "users.txt";
    const std::string ACCOUNTS_FILE = "accounts.txt";
    const std::string TRANSACTIONS_FILE = "transactions.txt";

public:
   Library() : nextBookId(1), nextUserId(1), nextTransactionId(1), currentUser(nullptr) {
    std::cout << "Loading library data..." << std::endl;
    try {
        loadData();
        std::cout << "Loaded " << books.size() << " books, " << users.size() << " users, " 
                 << accounts.size() << " accounts." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error during data loading: " << e.what() << std::endl;
        std::cerr << "Starting with empty library." << std::endl;
    }
}

    ~Library() {
        saveData();
        for (auto user : users) {
            delete user;
        }
    }

    bool isUserLoggedIn() const {
        return currentUser != nullptr;
    }

    User* getCurrentUser() const {
        return currentUser;
    }

    void loadData() {
        loadBooks();
        loadUsers();
        loadAccounts();
        loadTransactions();
    }

    void saveData() {
        saveBooks();
        saveUsers();
        saveAccounts();
        saveTransactions();
        std::cout << "All data saved successfully." << std::endl;
    } 

    void loadBooks() {
    std::ifstream file(BOOKS_FILE);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << BOOKS_FILE << std::endl;
        return;
    }

    books.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Book book = Book::fromString(line);

            if (book.getStatus() == "Not Available") {
                book.setStatus("Not Available"); // Keep it as Not Available if it was borrowed
            } else {
                book.setStatus("Available"); // Else mark it as Available
            }

            books.push_back(book);
            nextBookId = std::max(nextBookId, book.getBookId() + 1);
        }
    }
    file.close();
}

    void loadUsers() {
    std::ifstream file(USERS_FILE);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << USERS_FILE << std::endl;
        return;
    }

    users.clear(); // Use the correct variable name

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            // Try parsing the user data
            User* user = User::fromString(line);
            if (user) {
                users.push_back(user);
                nextUserId = std::max(nextUserId, user->getUserId() + 1);
            } else {
                std::cerr << "Failed to parse user from line: " << line << std::endl;
            }
        }
    }

    file.close();
}

    void loadAccounts() {
        std::ifstream file(ACCOUNTS_FILE);
        if (!file.is_open()) {
        }

        accounts.clear();
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                Account account = Account::fromString(line);
                accounts[account.getUserId()] = account;
            }
        }
        file.close();
    }
    void loadTransactions() {
        std::ifstream file(TRANSACTIONS_FILE);
        if (!file.is_open()) {
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                Transaction transaction = Transaction::fromString(line);
                int userId = transaction.getUserId();

                if (accounts.find(userId) != accounts.end()) {
                    accounts[userId].addTransaction(transaction);
                }

                nextTransactionId = std::max(nextTransactionId, transaction.getTransactionId() + 1);
            }
        }
        file.close();

        for (auto& pair : accounts) {
            pair.second.recalculateFine();
        }
    }

   void saveBooks() {
    std::ofstream file(BOOKS_FILE);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open " << BOOKS_FILE << " for writing." << std::endl;
        return;
    }

    bool success = true;
    for (const auto& book : books) {
        file << book.toString() << std::endl;
        if (file.fail()) {
            std::cerr << "Error: Failed while writing book data." << std::endl;
            success = false;
            break;
        }
    }

    file.close();
    
    if (success) {
        std::cout << "Books data saved successfully." << std::endl;
    } else {
        std::cerr << "Error: Failed to save all book data." << std::endl;
    }
}

    void saveUsers() {
    std::ofstream file(USERS_FILE);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open " << USERS_FILE << " for writing." << std::endl;
        return;
    }

    for (const auto& user : users) {  
        file << user->toString() << std::endl;  
    }

    if (file.fail()) {
        std::cerr << "Error: Failed to write to " << USERS_FILE << std::endl;
    } else {
        std::cout << "Users data saved successfully." << std::endl;
    }

    file.close();
}

    void saveAccounts() {
    std::ofstream file(ACCOUNTS_FILE);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open " << ACCOUNTS_FILE << " for writing." << std::endl;
        return;
    }

    for (const auto& pair : accounts) {
        file << pair.second.toString() << std::endl;
    }

    if (file.fail()) {
        std::cerr << "Error: Failed to write to " << ACCOUNTS_FILE << std::endl;
    } else {
        std::cout << "Accounts data saved successfully." << std::endl;
    }

    file.close();
}

    void saveTransactions() {
    std::ofstream file(TRANSACTIONS_FILE);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open " << TRANSACTIONS_FILE << " for writing." << std::endl;
        return;
    }

    for (const auto& pair : accounts) {
        for (const auto& transactionStr : pair.second.getTransactionsAsStrings()) {
            file << transactionStr << std::endl;
        }
    }

    if (file.fail()) {
        std::cerr << "Error: Failed to write to " << TRANSACTIONS_FILE << std::endl;
    } else {
        std::cout << "Transactions data saved successfully." << std::endl;
    }

    file.close();
}

int getBooksCount() const {
    return books.size();
}

int getUsersCount() const {
    return users.size();
}

Book* findBook(int bookId) {
    for (auto& book : books) {
        if (book.getBookId() == bookId) {
            return &book;
        }
    }
    return nullptr;
}

bool addBook(const std::string& title, const std::string& author, const std::string& publisher, 
             int year, const std::string& isbn) {
    if (!isUserLoggedIn() || currentUser->getRole() != "Librarian") {
        std::cout << "Only librarians can add books.\n";
        return false;
    }
    
    Book newBook(nextBookId++, title, author, publisher, year, isbn);
    books.push_back(newBook);
    std::cout << "Book added successfully.\n";
    return true;
}

bool removeBook(int bookId) {
    if (!isUserLoggedIn() || currentUser->getRole() != "Librarian") {
        std::cout << "Only librarians can remove books.\n";
        return false;
    }
    
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getBookId() == bookId) {
            if (it->getStatus() != "Available") {
                std::cout << "Cannot remove a book that is currently borrowed.\n";
                return false;
            }
            books.erase(it);
            std::cout << "Book removed successfully.\n";
            return true;
        }
    }
    
    std::cout << "Book not found.\n";
    return false;
}


bool addUser(const std::string& name, const std::string& email, const std::string& password, const std::string& role) {
    if (users.empty()) {
        if (role != "Librarian") {
            std::cout << "Error: The first user must be a librarian.\n";
            return false;
        }
    } else if (currentUser == nullptr || currentUser->getRole() != "Librarian") {
        std::cout << "Only librarians can add users.\n";
        return false;
    }

    User* newUser = nullptr;
    int newUserId = nextUserId++;  

    if (role == "Student") {
        newUser = new Student(newUserId, name, email, password);  
    } else if (role == "Faculty") {
        newUser = new Faculty(newUserId, name, email, password);
    } else if (role == "Librarian") {
        newUser = new Librarian(newUserId, name, email, password);
    } else {
        std::cout << "Error: Invalid role specified.\n";
        return false;
    }

    users.push_back(newUser);
    std::cout << "User added successfully: " << name << " (" << email << ")" << std::endl;
    return true;
}


bool removeUser(int userId) {
    if (!isUserLoggedIn() || currentUser->getRole() != "Librarian") {
        std::cout << "Only librarians can remove users.\n";
        return false;
    }

    if (currentUser->getUserId() == userId) {
        std::cout << "Cannot remove yourself.\n";
        return false;
    }

    // Ensure at least one librarian remains
    int librarianCount = 0;
    for (const auto& user : users) {
        if (user->getRole() == "Librarian") {
            librarianCount++;
        }
    }

    for (auto it = users.begin(); it != users.end(); ) {  
        if ((*it)->getUserId() == userId) {
            if ((*it)->getRole() == "Librarian" && librarianCount == 1) {
                std::cout << "Cannot remove the last librarian.\n";
                return false;
            }

            auto accountIt = accounts.find(userId);
            if (accountIt != accounts.end() && accountIt->second.getCurrentlyBorrowedCount() > 0) {
                std::cout << "Cannot remove a user who has borrowed books.\n";
                return false;
            }

            delete *it;
            it = users.erase(it);  

            if (accountIt != accounts.end()) {
                accounts.erase(accountIt);
            }

            std::cout << "User removed successfully.\n";
            return true;
        } else {
            ++it;
        }
    }

    std::cout << "User not found.\n";
    return false;
}

bool login(const std::string& email, const std::string& password) {
    for (auto user : users) {
        if (user->getEmail() == email && user->getPassword() == password) {
            currentUser = user;
            
            int userId = user->getUserId();

            if (accounts.find(userId) == accounts.end()) {
                // 🚀 **No account exists -> Create a new account for this user**
                Account newAccount(userId);
                accounts[userId] = newAccount;
                std::cout << "New account created for: " << user->getName() << std::endl;
            } else {
                // 🚀 **Account exists -> Retrieve the existing account**
                std::cout << "Welcome back, " << user->getName() << "!" << std::endl;
                std::cout << " Retrieving your borrowed books and transactions...\n";
                accounts[userId].displayDetails();
            }

            return true;
        }
    }
    return false;
}


void logout() {
    currentUser = nullptr;
}

void displayAllBooks() const {
    std::cout << "\n===== All Books =====" << std::endl;
    if (books.empty()) {
        std::cout << "No books in the library." << std::endl;
    } else {
        for (const auto& book : books) {
            book.displayDetails();
        }
    }
}

void displayAvailableBooks() const {
    std::cout << "\n===== Available Books =====" << std::endl;
    bool found = false;
    
    for (const auto& book : books) {
        if (book.getStatus() == "Available") {
            book.displayDetails();
            found = true;
        }
    }
    
    if (!found) {
        std::cout << "No available books in the library." << std::endl;
    }
}

void displayAllUsers() const {
    if (!isUserLoggedIn() || currentUser->getRole() != "Librarian") {
        std::cout << "Only librarians can view all users.\n";
        return;
    }
    
    std::cout << "\n===== All Users =====" << std::endl;
    if (users.empty()) {
        std::cout << "No users in the system." << std::endl;
    } else {
        for (const auto& user : users) {
            user->displayDetails();
        }
    }
}

bool borrowBook(int bookId) {
    if (!isUserLoggedIn()){
        std::cout << "user did not get logged in .\n";
        return false;
    }
    
    if (currentUser->getRole() != "Student" && currentUser->getRole() != "Faculty") {
        std::cout << "Only students and faculty can borrow books.\n";
        return false;
    }

    Book* book = findBook(bookId);
    if (!book) {
        std::cout << "Book not found.\n";
        return false;
    }

    if (book->getStatus() != "Available") {
        std::cout << "This book is not available for borrowing.\n";
        return false;
    }
   
    int userId = currentUser->getUserId();
    if (accounts.find(userId) == accounts.end()) {
        std::cout << "User account not found.\n";
        return false;
    }
    
    Account& account = accounts[userId];
    
    int currentlyBorrowed = account.getCurrentlyBorrowedCount();
    int maxBooks = currentUser->getMaxBooks();
    
    if (currentlyBorrowed >= maxBooks) {
        std::cout << "You have reached your maximum borrowing limit of " << maxBooks << " books.\n";
        return false;
    }

    double fineAmount = account.getTotalFine();
    if (fineAmount > 0) {
        std::cout << "You have unpaid fines of ₹" << fineAmount << ". Please clear your dues before borrowing.\n";
        return false;
    }
    if (!currentUser->canBorrow(currentlyBorrowed, fineAmount, account.getTransactions())) {
        if (currentUser->getRole() == "Faculty") {
            std::cout << "You cannot borrow more books as you have books overdue by more than 60 days.\n";
        } else {
            std::cout << "You are not eligible to borrow books at this time.\n";
        }
        return false;
    }

    Date borrowDate;
    Date dueDate = borrowDate.addDays(currentUser->getMaxBorrowDays());
    
    Transaction transaction(nextTransactionId++, userId, bookId, borrowDate, dueDate);
    account.addTransaction(transaction);

    book->setStatus("Borrowed");
    
    std::cout << "Book borrowed successfully. Due date: ";
    dueDate.printDate();
    
    return true;
}

bool returnBook(int bookId) {
   
    if (!isUserLoggedIn()){
        std::cout << "user did not get logged in .\n";
        return false;
    }
    
    if (currentUser->getRole() != "Student" && currentUser->getRole() != "Faculty") {
        std::cout << "Only students and faculty can return books.\n";
        return false;
    }
    
    Book* book = findBook(bookId);
    if (!book) {
        std::cout << "Book not found.\n";
        return false;
    }
    
    int userId = currentUser->getUserId();
    if (accounts.find(userId) == accounts.end()) {
        std::cout << "User account not found.\n";
        return false;
    }
    
    Account& account = accounts[userId];
    
    std::vector<Transaction> activeTransactions = account.getActiveTransactions();
    Transaction* transaction = nullptr;
    
    for (auto& t : activeTransactions) {
        if (t.getBookId() == bookId) {
            transaction = &t;
            break;
        }
    }
    
    if (!transaction) {
        std::cout << "You have not borrowed this book.\n";
        return false;
    }

    Date returnDate;
    transaction->setReturnDate(returnDate);
    transaction->setIsReturned(true);

    Date dueDate = transaction->getDueDate();
    double fine = 0.0;
    
    if (returnDate < dueDate) {
        fine = 0.0; // Book returned on time
    } else {
        fine = currentUser->calculateFine(dueDate, returnDate);
        if (fine > 0) {
            std::cout << "Late return! Fine: ₹" << fine << std::endl;
        }
    }
    
    transaction->setFine(fine);
    account.updateTransaction(*transaction);
 
    book->setStatus("Available");
    
    std::cout << "Book returned successfully.\n";
    
    return true;
}

void displayCurrentlyBorrowedBooks() const {
    if (!isUserLoggedIn()) {
        std::cout << "No user is logged in.\n";
        return;
    }
    
    int userId = currentUser->getUserId();
    if (accounts.find(userId) == accounts.end()) {
        std::cout << "User account not found.\n";
        return;
    }
    
    const Account& account = accounts.at(userId);
    std::vector<int> borrowedBookIds = account.getCurrentlyBorrowedBooks();
    
    std::cout << "\n===== Your Currently Borrowed Books =====" << std::endl;
    if (borrowedBookIds.empty()) {
        std::cout << "You have not borrowed any books.\n";
    } else {
        for (int bookId : borrowedBookIds) {
            for (const auto& book : books) {
                if (book.getBookId() == bookId) {
                    book.displayDetails();
                    break;
                }
            }
        }
    }
}

void displayBorrowingHistory() const {
    if (!isUserLoggedIn()) {
        std::cout << "No user is logged in.\n";
        return;
    }
    
    int userId = currentUser->getUserId();
    if (accounts.find(userId) == accounts.end()) {
        std::cout << "User account not found.\n";
        return;
    }
    
    const Account& account = accounts.at(userId);
    std::vector<Transaction> history = account.getBorrowingHistory();
    
    std::cout << "\n===== Your Borrowing History =====" << std::endl;
    if (history.empty()) {
        std::cout << "You have no borrowing history.\n";
    } else {
        for (const auto& transaction : history) {
            std::cout << "Transaction ID: " << transaction.getTransactionId() << std::endl;

            int bookId = transaction.getBookId();
            for (const auto& book : books) {
                if (book.getBookId() == bookId) {
                    std::cout << "Book: " << book.getTitle() << " by " << book.getAuthor() << std::endl;
                    break;
                }
            }
            
            std::cout << "Borrowed on: ";
            transaction.getBorrowDate().printDate();
            
            std::cout << "Returned on: ";
            transaction.getReturnDate().printDate();
            
            std::cout << "Fine: ₹" << transaction.getFine() << "\n" << std::endl;
        }
    }
}

void displayUserAccount() const {
    if (!isUserLoggedIn()) {
        std::cout << "No user is logged in.\n";
        return;
    }
    
    int userId = currentUser->getUserId();
    displayUserAccountById(userId);
}

void displayUserAccountById(int userId) const {
    if (!isUserLoggedIn()) {
        std::cout << "No user is logged in.\n";
        return;
    }
    
    if (currentUser->getUserId() != userId && currentUser->getRole() != "Librarian") {
        std::cout << "You don't have permission to view this account.\n";
        return;
    }
    
    User* user = nullptr;
    for (auto u : users) {
        if (u->getUserId() == userId) {
            user = u;
            break;
        }
    }
    
    if (!user) {
        std::cout << "User not found.\n";
        return;
    }
 
    user->displayDetails();

    if (accounts.find(userId) != accounts.end()) {
        const Account& account = accounts.at(userId);
        account.displayDetails();
    } else {
        std::cout << "Account not found for this user.\n";
    }
}

Account& getAccount(int userId) {
    if (accounts.find(userId) == accounts.end()) {
        accounts[userId] = Account(userId);
    }
    return accounts[userId];
}

void displayAllTransactions() const {
    if (!isUserLoggedIn() || currentUser->getRole() != "Librarian") {
        std::cout << "Only librarians can view all transactions.\n";
        return;
    }
    
    std::cout << "\n===== All Transactions =====" << std::endl;
    bool found = false;
    
    for (const auto& accountPair : accounts) {
        const std::vector<Transaction>& transactions = accountPair.second.getTransactions();
        for (const auto& transaction : transactions) {
            found = true;
            transaction.displayDetails();
         
            for (const auto& user : users) {
                if (user->getUserId() == transaction.getUserId()) {
                    std::cout << "User: " << user->getName() << std::endl;
                    break;
                }
            }
            
            for (const auto& book : books) {
                if (book.getBookId() == transaction.getBookId()) {
                    std::cout << "Book: " << book.getTitle() << std::endl;
                    break;
                }
            }
            
            std::cout << "------------------------\n";
        }
    }
    
    if (!found) {
        std::cout << "No transactions found.\n";
    }
  }    
};

int main() {
    Library librarySystem;
    
    bool running = true;
    while (running) {
        if (librarySystem.isUserLoggedIn()) {
            User* currentUser = librarySystem.getCurrentUser();
            std::string role = currentUser->getRole();
            
            std::cout << "\n=== LIBRARY MANAGEMENT SYSTEM ===\n";
            std::cout << "Welcome, " << currentUser->getName() << " (" << role << ")\n\n";
            
            if (role == "Student" || role == "Faculty") {
                std::cout << "1. View available books\n";
                std::cout << "2. Borrow a book\n";
                std::cout << "3. Return a book\n";
                std::cout << "4. View my borrowed books\n";
                std::cout << "5. View my returned history\n";
                std::cout << "6. View my account details\n";
                std::cout << "7. Pay fine\n";
                std::cout << "8. Logout\n";
                std::cout << "9. Exit\n";
                std::cout << "Enter your choice: ";
                
                int choice;
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                switch (choice) {
                    case 1:
                        librarySystem.displayAvailableBooks();
                        break;
                    case 2:
                        {
                            std::cout << "Enter Book ID to borrow: ";
                            int bookId;
                            std::cin >> bookId;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            librarySystem.borrowBook(bookId);
                        }
                        break;
                    case 3:
                        {
                            std::cout << "Enter Book ID to return: ";
                            int bookId;
                            std::cin >> bookId;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid book ID. Please enter a valid number.\n";
                            break;
                          }
                            librarySystem.returnBook(bookId);
                        }
                        break;
                    case 4:
                        librarySystem.displayCurrentlyBorrowedBooks();
                        break;
                    case 5:
                        librarySystem.displayBorrowingHistory();
                        break;
                    case 6:
                        librarySystem.displayUserAccount();
                        break;
                    case 7:
                        {
                            Account& account = librarySystem.getAccount(currentUser->getUserId());
                            double fineAmount = account.getTotalFine();
                        
                            if (fineAmount > 0) {
                                std::cout << "Current fine amount: ₹" << fineAmount << std::endl;
                        
                                double payAmount;
                                do {
                                    std::cout << "Enter amount to pay (full amount required): ₹";
                                    std::cin >> payAmount;
                                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        
                                    if (payAmount != fineAmount) {
                                        std::cout << "Error: You must pay the full fine amount of ₹" << fineAmount << ". Try again.\n";
                                    }
                                } while (payAmount != fineAmount);
                        
                                account.payFine(payAmount);
                            } else {
                                std::cout << "You have no outstanding fines.\n";
                            }
                        }
                        break;
                        
                    case 8:
                        librarySystem.logout();
                        std::cout << "Logged out successfully.\n";
                        break;
                    case 9:
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                        break;  
                }
                    
            } else if (role == "Librarian") {
                std::cout << "1. View all books\n";
                std::cout << "2. Add a book\n";
                std::cout << "3. Update a book\n";
                std::cout << "4. Remove a book\n";
                std::cout << "5. View all users\n";
                std::cout << "6. Add a user\n";
                std::cout << "7. Remove a user\n";
                std::cout << "8. View user account details\n";
                std::cout << "9. View all transactions\n";
                std::cout << "10. Logout\n";
                std::cout << "11. Exit\n";
                std::cout << "Enter your choice: ";
                
                int choice;
                std::cin >> choice;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                
                switch (choice) {
                    case 1:
                        librarySystem.displayAllBooks();
                        break;
                    case 2:
                        {
                            std::string title, author, publisher, isbn;
                            int year;
                            
                            std::cout << "Enter book title: ";
                            std::getline(std::cin, title);
                            
                            std::cout << "Enter author: ";
                            std::getline(std::cin, author);
                            
                            std::cout << "Enter publisher: ";
                            std::getline(std::cin, publisher);
                            
                            std::cout << "Enter publication year: ";
                            std::cin >> year;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            
                            std::cout << "Enter ISBN: ";
                            std::getline(std::cin, isbn);
                            
                            librarySystem.addBook(title, author, publisher, year, isbn);
                            std::cout << "Book added successfully!\n";
                        }
                        break;
                    case 3:
                        {
                            std::cout << "Enter Book ID to update: ";
                            int bookId;
                            std::cin >> bookId;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            
                            Book* book = librarySystem.findBook(bookId);
                            if (book) {
                                std::cout << "Current book details:\n";
                                book->displayDetails();
                                
                                std::string title, author, publisher, isbn;
                                int year;
                                
                                std::cout << "Enter new title (or press Enter to keep current): ";
                                std::getline(std::cin, title);
                                if (!title.empty()) book->setTitle(title);
                                
                                std::cout << "Enter new author (or press Enter to keep current): ";
                                std::getline(std::cin, author);
                                if (!author.empty()) book->setAuthor(author);
                                
                                std::cout << "Enter new publisher (or press Enter to keep current): ";
                                std::getline(std::cin, publisher);
                                if (!publisher.empty()) book->setPublisher(publisher);
                                
                                std::cout << "Enter new publication year (or 0 to keep current): ";
                                std::cin >> year;
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                if (year != 0) book->setYear(year);
                                
                                std::cout << "Enter new ISBN (or press Enter to keep current): ";
                                std::getline(std::cin, isbn);
                                if (!isbn.empty()) book->setISBN(isbn);
                                
                                std::cout << "Book updated successfully!\n";
                            } else {
                                std::cout << "Book not found!\n";
                            }
                        }
                        break;
                    case 4:
                        {
                            std::cout << "Enter Book ID to remove: ";
                            int bookId;
                            std::cin >> bookId;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            librarySystem.removeBook(bookId);
                        }
                        break;
                    case 5:
                        librarySystem.displayAllUsers();
                        break;
                    case 6:
                        {
                            std::string name, email, password, role;
                            
                            std::cout << "Enter user name: ";
                            std::getline(std::cin, name);
                            
                            std::cout << "Enter email: ";
                            std::getline(std::cin, email);
                            
                            std::cout << "Enter password: ";
                            std::getline(std::cin, password);
                            
                            std::cout << "Enter role (Student/Faculty/Librarian): ";
                            std::getline(std::cin, role);
                            
                            if (role != "Student" && role != "Faculty" && role != "Librarian") {
                            std::cout << "Error: Invalid role. Please enter 'Student', 'Faculty', or 'Librarian'.\n";
                            break;
                          }

                            librarySystem.addUser(name, email, password, role);
                            std::cout << "User added successfully!\n";
                        }
                        break;
                    case 7:
                        {
                            std::cout << "Enter User ID to remove: ";
                            int userId;
                            std::cin >> userId;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            librarySystem.removeUser(userId);
                        }
                        break;
                    case 8:
                        {
                            std::cout << "Enter User ID to view account details: ";
                            int userId;
                            std::cin >> userId;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            librarySystem.displayUserAccountById(userId);
                        }
                        break;
                    case 9:
                        librarySystem.displayAllTransactions();
                        break;
                    case 10:
                        librarySystem.logout();
                        std::cout << "Logged out successfully.\n";
                        break;
                    case 11:
                        running = false;
                        break;
                    default:
                        std::cout << "Invalid choice. Please try again.\n";
                        break;  
                }
            }
        } else {
            std::cout << "\n=== LIBRARY MANAGEMENT SYSTEM ===\n";
            std::cout << "1. Login\n";
            std::cout << "2. Exit\n";
            std::cout << "Enter your choice: ";
            
            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            switch (choice) {
                case 1:
                    {
                        std::string email, password;
                        std::cout << "Enter email: ";
                        std::getline(std::cin, email);
                        std::cout << "Enter password: ";
                        std::getline(std::cin, password);
                        
                        if (librarySystem.login(email, password)) {
                            std::cout << "Login successful!\n";
                        } else {
                            std::cout << "Invalid email or password. Please try again.\n";
                        }
                    }
                    break;
                case 2:
                    {
                        running = false;
                        break;
                    }      
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break; 
            }
        }
    }
    std::cout << "Saving data before exit..." << std::endl;
        bool saveSuccess = false;
        try {
            librarySystem.saveData();
            saveSuccess = true;
        } catch (const std::exception& e) {
            std::cerr << "Error saving data: " << e.what() << std::endl;
        }
    return 0;
}

