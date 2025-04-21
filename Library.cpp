#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "Library.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Constructor to load files
Library::Library()
{
    loadBooks();
    loadUsers();
    loadTransaction();
}

// Destructor to save files
Library::~Library()
{
    saveBooks();
    saveUsers();
    saveTransaction();
}

// Load books from the file
void Library::loadBooks()
{
    ifstream inFile(bookFilePath);

    if (!inFile)
    {
        cerr << "File not present. Creating the file " << bookFilePath << endl;
        ofstream outFile(bookFilePath);
        if (!outFile)
        {
            cerr << "Failed to create file " << bookFilePath << endl;
            return;
        }
        outFile.close();
        inFile.open(bookFilePath);
        if (!inFile)
        {
            cerr << "Could not open file " << bookFilePath  << endl;
            return;
        }
    }
    string line;

    while (std::getline(inFile, line))
    {
        istringstream iss(line);
        int book_id;
        string book_name, author_name, status;

        if (iss >> book_id)
        {
            iss.ignore(); // Ignore space after ID
            getline(iss, book_name, '|');
            getline(iss, author_name, '|');
            getline(iss, status);
            books.emplace_back(book_id, book_name, author_name, status);
        }
    }
    inFile.close();
}

// Save books to the file
void Library::saveBooks() const
{
    ofstream outFile(bookFilePath);

    if (!outFile)
    {
        cerr << "Could not open file for writing: " << bookFilePath << endl;
        return;
    }

    for (const auto &book : books)
    {
        outFile << book.getBookID() << " " << book.getBookName() << "|" << book.getAuthorName() << "|" << book.getStatus() << "\n";
    }
    outFile.close();
}

// Add a new book to the library
void Library::addBook(int book_id, const string &book_name, const string &author_name)
{
    if (isUniqueID(book_id, bookFilePath))
    {
        books.emplace_back(book_id, book_name, author_name, "Available");
        saveBooks();
        cout << "Book added successfully!" << endl;
    }
    else
    {
        cout << "Book with ID " << book_id << "exists. Please add a unique ID \n";
    }
}

// Remove a book from the library by book_id
void Library::removeBook(int book_id)
{
    auto it = std::find_if(books.begin(), books.end(), [book_id](const Book &book)
                           { return book.getBookID() == book_id; });

    if (it != books.end())
    {
        books.erase(it);
        saveBooks();
        cout << "Book removed successfully from Library!" << std::endl;
    }
    else
    {
        cout << "Book with ID " << book_id << " not found in Library! " << endl;
    }
}

// Display all books present in the library
void Library::displayBook() const {
    if (books.empty()) {
        cout << "No Books in the Library!" << endl;
        return;
    }

    cout << "Books in the Library:" << endl;

    for (const auto& book : books) {
        book.displayBook();
    }
}

// Display all books in the library
void Library::displayBook(SOCKET clientSock) const
{
    string message;
    if (books.empty())
    {
        message = "No Books in the Library!\n";
    }
    else
    {
        for (const auto &book : books)
        {
            message += "Book ID = " + std::to_string(book.getBookID()) + "|";
            message += "Book Name = " + book.getBookName() + "|";
            message += "Author Name = " + book.getAuthorName() + "|";
            message += "Status = " + book.getStatus() + "\n";
        }
    }
    send(clientSock, message.c_str(), static_cast<int>(message.length()), 0);
    return;
}

// Load Users from the file
void Library::loadUsers()
{
    ifstream inFile(userFilePath);

    if (!inFile)
    {
        cerr << "Could not open file. Creating a new file. " << userFilePath << endl;
        ofstream outFile(userFilePath);
        if (!outFile)
        {
            cerr << "Failed to create file " << userFilePath << endl;
            return;
        }
        outFile.close();
        inFile.open(userFilePath);
        if (!inFile)
        {
            cerr << "Could not open file " << userFilePath << endl;
            return;
        }
    }
    string line;
    while (std::getline(inFile, line))
    {
        istringstream iss(line);
        int id;
        string name, status;

        if (iss >> id)
        {
            iss.ignore(); // Ignore space after ID
            getline(iss, name, '|');
            getline(iss, status);
            users.emplace_back(id, name, status);
        }
    }
    inFile.close();
}

// Save Users to the file
void Library::saveUsers() const
{
    ofstream outFile(userFilePath);

    if (!outFile)
    {
        cerr << "Could not open file for writing: " << userFilePath << endl;
        return;
    }

    for (const auto &user : users)
    {
        outFile << user.getId() << " " << user.getName() << "|" << user.getHasBorrowed() << "\n";
    }
    outFile.close();
}

// Add a new User to the library
void Library::addUser(int id, const string &name)
{
    if (isUniqueID(id, userFilePath))
    {
        users.emplace_back(id, name, "No");
        saveUsers();
        cout << "User added successfully!" << endl;
    }
    else
    {
        cout << "User with ID " << id << "exists. Please add a unique ID \n";
    }
}

// Remove a User from the library by user ID
void Library::removeUser(int id)
{
    auto it = std::find_if(users.begin(), users.end(), [id](const User &user)
                           { return user.getId() == id; });

    if (it != users.end())
    {
        checkBorrowedBooks(id); // check if books are borrowed
        users.erase(it);
        saveUsers();
        cout << "User removed successfully!"  << endl;
    }
    else
    {
        cout << "User with ID " << id << " not found in Library! " << endl;
    }
}

// Display all Users in the library
void Library::displayUser() const
{
    if (users.empty())
    {
        cout << "No Users in the library!" << endl;
        return;
    }

    cout << "Registered Users in the Library:" << endl;
    for (const auto &user : users)
    {
        cout << "ID: " << user.getId() << ", Name: " << user.getName() << ", HasBorrowed: " << user.getHasBorrowed() << endl;
    }
}

// Search for a book by ID, name, or author
Book *Library::searchBook(const std::string &text, const std::string &type)
{
    auto it = std::find_if(books.begin(), books.end(), [&text, &type](const Book &book)
                           {
        if (type == "id") {
            int id = std::stoi(text); 
            return book.getBookID() == id;
        }
        else if (type == "name") {
            return book.getBookName() == text;
        }
        else if (type == "author") {
            return book.getAuthorName() == text;
        }
        return false; });

    if (it != books.end())
    {
        return &(*it);
    }
    else
    {
        return nullptr;
    }
}

// Load Users from the file
void Library::loadTransaction()
{
    ifstream inFile(transactionPath);

    if (!inFile)
    {
        cerr << "Could not open file. Creating a new file. " << transactionPath  << endl;
        ofstream outFile(transactionPath);
        if (!outFile)
        {
            cerr << "Failed to create file " << transactionPath << endl;
            return;
        }
        outFile.close();
        inFile.open(transactionPath);
        if (!inFile)
        {
            cerr << "Could not open file " << transactionPath << endl;
            return;
        }
    }
    string line;
    while (std::getline(inFile, line))
    {
        istringstream iss(line);
        int id;
        int book_id = 0;
        string name, book_name, status, dueDate, date;
        string book_id_str = to_string(book_id);

        if (iss >> id)
        {
            iss.ignore(); // Ignore space after ID
            getline(iss, date, '|');
            getline(iss, name, '|');
            getline(iss, book_id_str, '|');
            getline(iss, book_name, '|');
            getline(iss, status, '|');
            getline(iss, dueDate);
            book_id = stoi(book_id_str);
            transactions.emplace_back(id, date, name, book_id, book_name, status, dueDate);
        }
    }
    inFile.close();
}

// Save Users to the file
void Library::saveTransaction() const
{
    ofstream outFile(transactionPath);

    if (!outFile)
    {
        cerr << "Could not open file for writing: " << transactionPath << endl;
        return;
    }

    for (const auto &transaction : transactions)
    {
        outFile << transaction.getID() << " " << transaction.getDate() << "|" << transaction.getUserName() << "|" << transaction.getBookID() << "|" << transaction.getBookName() << "|" << transaction.getStatus() << "|" << transaction.getDueDate() << "\n";
    }
    outFile.close();
}

// Add a new User to the library
void Library::addTransaction(int id, int book_id, const string &status)
{
    string dueDate, date, book_status;
    date = setDueDate(0);
    bool allowTransaction = 0;

    if (!isUniqueID(id, userFilePath) && !isUniqueID(book_id, bookFilePath))
    {

        if (status == "Borrowed" && checkStatusOfBook(book_id) == "Available")
        {
            dueDate = setDueDate(15);
            book_status = "Not Available";
            allowTransaction = 1;
        }
        else if (status == "Returned" && checkStatusOfBook(book_id) == "Not Available")
        {
            dueDate = "NA";
            book_status = "Available";
            allowTransaction = 1;
        }
        else
        {
            allowTransaction = 0;
            cout << "Transaction Unsucessfull! Book is either already Returned or Borrowed\n " << endl;
        }

        if (allowTransaction)
        {
            transactions.emplace_back(id, date, returnUserName(id), book_id, returnBookName(book_id), status, dueDate);
            saveTransaction();
            books.clear();
            users.clear();
            updateDataById(bookFilePath, book_id, book_status);
            loadBooks();
            bool userStatus = isBorrowedEqualsReturned(id);
            if (userStatus == 1)
            {
                updateDataById(userFilePath, id, "No");
            }
            else
            {
                updateDataById(userFilePath, id, "Yes");
            }
            loadUsers();
            cout << "Transaction added successfully!" << endl;
        }
    }
    else
    {
        cout << "Book ID or User ID is incorrect. Please recheck! \n" << endl;
    }
}

// Display all Transactions in the library
void Library::displayTransaction() const
{
    if (transactions.empty())
    {
        cout << "No Transactions in the Library!" << endl;
        return;
    }

    cout << "Transactions of the Library:" << endl;

    for (const auto &transaction : transactions)
    {
        transaction.displayTransaction();
    }
}

string Library::setDueDate(int days)
{
    time_t timestamp = time(NULL);
    struct tm datetime;
    localtime_s(&datetime, &timestamp);
    datetime.tm_mday += days;
    mktime(&datetime);
    char output[50];
    strftime(output, 50, "%d/%m/%y", &datetime);
    const char *dates = output;
    return dates;
}

// Update data in the file by ID
void Library::updateDataById(const string &filename, const int &uniqueID, const string &newName) const
{
    ifstream inputFile(filename);
    stringstream buffer;
    string line;

    if (filename == Library::bookFilePath)
    {

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string name, author, status;
            int id;
            if (ss >> id)
            {
                ss.ignore(); // Ignore space after ID
                getline(ss, name, '|');
                getline(ss, author, '|');
                getline(ss, status);

                if (id == uniqueID)
                {
                    status = newName;
                }

                buffer << id << " " << name << "|" << author << "|" << status << endl;
            }
        }
    }
    else
    {
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string name, status;
            int id;
            if (ss >> id)
            {
                ss.ignore(); // Ignore space after ID
                getline(ss, name, '|');
                getline(ss, status);
                if (id == uniqueID)
                {
                    status = newName;
                }

                buffer << id << " " << name << "|" << status << endl;
            }
        }
    }

    inputFile.close();

    ofstream outputFile(filename, ios::trunc);

    outputFile << buffer.str();
    cout << buffer.str();
    outputFile.flush();
    outputFile.close();
}

// Check if the ID is unique in the file
bool Library::isUniqueID(int ID, string fileName)
{
    ifstream file(fileName);
    if (!file.is_open())
    {
        cout << "Error: Could not open file.\n";
        return false;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        int existingID;
        ss >> existingID; 

        if (existingID == ID)
        {
            file.close();
            return false; 
        }
    }

    file.close();
    return true; 
}

// Check if the number of borrowed books is equal to the number of returned books
bool Library::isBorrowedEqualsReturned(int userId)
{
    int borrowCount = 0, returnCount = 0;

    for (const auto &transaction : transactions)
    {
        if (transaction.getID() == userId)
        {
            if (transaction.getStatus() == "Borrowed")
            {
                borrowCount++;
            }
            else if (transaction.getStatus() == "Returned")
            {
                returnCount++;
            }
        }
    }

    return borrowCount == returnCount;
}

string Library::returnUserName(int id)
{
    for (const auto &user : users)
    {
        if (user.getId() == id)
        {
            return user.getName();
        }
    }
    return "";
}

string Library::returnBookName(int id)
{
    for (const auto &book : books)
    {
        if (book.getBookID() == id)
        {
            return book.getBookName();
        }
    }
    return "";
}

string Library::checkStatusOfBook(int id)
{
    for (const auto &book : books)
    {
        if (book.getBookID() == id)
        {
            return book.getStatus();
        }
    }
    return "";
}

void Library::addTransaction(int id, int book_id, const std::string& status, SOCKET clientSock) {
    std::string dueDate, date, book_status;
    date = setDueDate(0);
    bool allowTransaction = 0;
    std::string message;

    if (!isUniqueID(id, userFilePath) && !isUniqueID(book_id, bookFilePath)) {
        if (status == "Borrowed" && checkStatusOfBook(book_id) == "Available") {
            dueDate = setDueDate(15);
            book_status = "Not Available";
            allowTransaction = 1;
        } else if (status == "Returned" && checkStatusOfBook(book_id) == "Not Available") {
            dueDate = "NA";
            book_status = "Available";
            allowTransaction = 1;
        } else {
            message = "Transaction not allowed. Book status does not match the requested operation.\n";
            send(clientSock, message.c_str(), static_cast<int>(message.length()), 0);
            return;
        }
         if (allowTransaction) {
            transactions.emplace_back(id, date, returnUserName(id), book_id, returnBookName(book_id), status, dueDate);
            saveTransaction();
            books.clear();
            users.clear();
            updateDataById(bookFilePath, book_id, book_status);
            loadBooks();
            bool userStatus = isBorrowedEqualsReturned(id);
            if (userStatus == 1)
            {
                updateDataById(userFilePath, id, "No");
            }
            else
            {
                updateDataById(userFilePath, id, "Yes");
            }
            loadUsers();
            message = "Transaction completed successfully!\n";
            send(clientSock, message.c_str(), static_cast<int>(message.length()), 0);
        }
    } else {
        message = "Invalid User ID or Book ID.\n";
        send(clientSock, message.c_str(), static_cast<int>(message.length()), 0);
    }
}

// Function to check if all borrowed books have been returned and get the book IDs of any books that have not been returned
void Library::checkBorrowedBooks(int userId) {
    std::vector<int> notReturnedBookIds;
    for (const auto& transaction : transactions) {
        if (transaction.getID() == userId && transaction.getStatus() == "Borrowed") {
        
            auto it = std::find_if(transactions.begin(), transactions.end(), [&](const Transaction& t) {
                return t.getID() == userId && t.getBookID() == transaction.getBookID() && t.getStatus() == "Returned";
            });

            if (it == transactions.end()) {
                notReturnedBookIds.push_back(transaction.getBookID());
            }
        }
    }

    if (!notReturnedBookIds.empty()) {
        for (int bookId : notReturnedBookIds) {
            addTransaction(userId,bookId,"Returned");
        }
        std::cout << std::endl;
    } else {
        std::cout << "All borrowed books have been returned." << std::endl;
    }

}