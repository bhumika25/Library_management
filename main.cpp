#include <iostream>
#include <thread>
#include <cstdlib>
#include "Library.h"
using namespace std;

// Main menus
void environmentMenu()
{
    cout << "1. Do you want to run as a console application?" << "\n";
    cout << "2. Do you want to run the server?" << "\n";
    cout << "3. Do you want to run the client?" << "\n";
}

// Menu for console application
void mainMenu()
{
    cout << "Welcome to the Library Portal" << "\n";
    cout << "Please select from below options and Enter your choice" << "\n";
    cout << "1. Are you a Admin? " << "\n";
    cout << "2. Are you a Library Member? " << "\n";
}

// Admin menu
void adminMenu()
{
    cout << "\nLibrary Menu for Admins:\n";
    cout << "1. Add Book\n";
    cout << "2. Remove Book\n";
    cout << "3. Search Book\n";
    cout << "4. Display All Books\n";
    cout << "5. Add User\n";
    cout << "6. Remove User\n";
    cout << "7. Display all Users\n";
    cout << "8. Display all Transactions\n";
    cout << "9. Exit\n";
}

// library members menu
void membersMenu()
{
    cout << "\nLibrary Menu for Members:\n";
    cout << "1. Display All Books\n";
    cout << "2. Search Book\n";
    cout << "3. Borrow Books\n";
    cout << "4. Return Books\n";
    cout << "5. Exit\n";
}

int main()
{
    Library library;
    bool showMenu = true;
    int env, role, option, id, book_id;
    string name, author, keyword, book_name, type;
    Book *book = nullptr;
    environmentMenu();
    cin >> env;
    while (showMenu)
    {
        switch (env)
        {
        case 1:
            cout << "Running Console Application " << "\n";
            while (true)
            {
                mainMenu();
                cin >> role;

                switch (role)
                {
                case 1:
                    while (true)
                    {
                        adminMenu();
                        cin >> option;

                        switch (option)
                        {
                        case 1:
                            cout << "Enter Book ID: " << "\n";
                            cin >> id;
                            cout << "Enter Book Name: " << "\n";
                            cin >> book_name;
                            cout << "Enter Author Name: " << "\n";
                            cin >> author;
                            library.addBook(id, book_name, author);
                            break;
                        case 2:
                            cout << "Enter Book ID to remove: " << "\n";
                            cin >> id;
                            library.removeBook(id);
                            break;
                        case 3:
                            cout << "Enter Keyword to Search: " << "\n";
                            cin >> keyword;
                            cout << "Enter type of keyword (id or name or author): " << "\n";
                            cin >> type;
                            book = library.searchBook(keyword, type);
                            if (book)
                            {
                                cout << "Book found:\n";
                                book->displayBook();
                            }
                            else
                            {
                                cout << "Book not found.\n";
                            }
                            break;
                        case 4: // Display All Books
                            library.displayBook();
                            break;
                        case 5:
                            cout << "Enter User ID: " << "\n";
                            cin >> id;
                            cout << "Enter User Name: " << "\n";
                            cin >> name;
                            library.addUser(id, name);
                            break;
                        case 6:
                            cout << "Enter User ID to remove: " << "\n";
                            cin >> id;
                            library.removeUser(id);
                            break;
                        case 7: 
                            library.displayUser();
                            break;
                        case 8:
                            library.displayTransaction();
                            break;
                        case 9: 
                            cout << "Exit the program.\n";
                            return 0;

                        default:
                            cout << "Invalid Choice \n";
                        }
                    }
                    break;
                case 2:
                    while (true)
                    {
                        membersMenu();
                        cin >> option;

                        switch (option)
                        {
                        case 1:
                            cout << "Display all the books from Library Catalogue " << "\n";
                            library.displayBook();
                            break;
                        case 2:
                            cout << "Enter Keyword to Search: " << "\n";
                            cin >> keyword;
                            cout << "Enter type of keyword (id or name or author): " << "\n";
                            cin >> type;
                            book = library.searchBook(keyword, type);
                            if (book)
                            {
                                cout << "Book found:\n";
                                book->displayBook();
                            }
                            else
                            {
                                cout << "Book not found.\n";
                            }
                            break;
                        case 3:
                            cout << "Borrow Books " << "\n";
                            cout << "Enter User ID: " << "\n";
                            cin >> id;
                            cout << "Enter Book ID: " << "\n";
                            cin >> book_id;
                            library.addTransaction(id, book_id, "Borrowed");
                            break;
                        case 4: 
                            cout << "Return Books" << "\n";
                            cout << "Enter User ID:" << "\n";
                            cin >> id;
                            cout << "Enter Book ID:" << "\n";
                            cin >> book_id;

                            library.addTransaction(id, book_id, "Returned");
                            break;

                        case 5: 
                            cout << "Exit the program.\n";
                            return 0;

                        default:
                            cout << "Invalid Choice \n";
                        }
                    }
                    break;
                default:
                    cout << "Invalid category choice.\n";
                }

                return 0;
            }
            break;
        case 2:
            cout << "Starting Server\n";
            system("server.exe");
            break;
        case 3:
            cout << "Starting Client\n";
            system("client.exe");
            break;
        }
    }
}
