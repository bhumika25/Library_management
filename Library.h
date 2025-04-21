#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "Book.h"
#include "User.h"
#include <vector>
#include <string>
#include "Transaction.h"

using namespace std;

class Library {

private:
    vector<Book> books;
    vector<User> users;
    vector<Transaction> transactions;
    string bookFilePath = "Books.txt";
    string userFilePath = "Users.txt";  
    string transactionPath = "Transaction.txt";
    void loadBooks();
    void saveBooks() const;
    void loadUsers();
    void saveUsers() const;
    void loadTransaction();
    void saveTransaction() const;

public:
    Library();
    ~Library();
    void addBook(int book_id, const string& book_name, const string& author_name);
    void removeBook(int book_id);
    void displayBook() const;
    void displayBook(SOCKET clientSock) const;
    Book* searchBook(const string& text, const string& type);
    void addUser(int id, const string& name);
    void removeUser(int id);
    void displayUser() const;
    void addTransaction(int id, int book_id, const string& status);
    void displayTransaction() const;
    string setDueDate(int days);
    void updateDataById(const string& filename, const int& uniqueID, const string& newName) const;
    bool isUniqueID(int ID, string fileName);
    bool isBorrowedEqualsReturned(int userId);
    void checkBorrowedBooks(int userId);
    string returnUserName(int id);
    string returnBookName(int id);
    string checkStatusOfBook(int id);
    void addTransaction(int id, int book_id, const string& status, SOCKET clientSock);
    void returnBooks(int userId);

};