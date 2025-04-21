#include "Book.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream> 

using namespace std;
using std::vector;

Book::Book(int book_id, const string& book_name, const string& author_name, const string& status): book_id(book_id),
book_name(book_name), author_name(author_name), status(status) {}

int Book::getBookID() const {
	return book_id;
}
string Book::getBookName() const {
	return book_name;
}
string Book::getAuthorName() const {
	return author_name;
}
string Book::getStatus() const {
	return status;
}
void Book::setBookID(int book_id) {
	this->book_id = book_id;
}
void Book::setBookName(const string& book_name) {
	this->book_name = book_name;
}
void Book::setAuthorName(const string& author_name) {
	this->author_name = author_name;
}

void Book::displayBook() const {
	cout << "Book ID: " << book_id
		<< ", Name: " << book_name
		<< ", Author: " << author_name 
		<< ", Status: " << status << std::endl;
}

std::string Book::displayBooks() const {
    std::ostringstream oss;
    oss << "Book ID: " << book_id << "|"
        << "Book Name: " << book_name << "|"
        << "Author: " << author_name << "|"
        << "Status: " << status << "\n";
    return oss.str();
}
