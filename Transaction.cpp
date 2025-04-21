#include "Transaction.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
using std::vector;

Transaction::Transaction(int id, const string& transDate, const string& name, int book_id, const string& book_name, const string& status, const string& dueDate) : 
	id(id), transDate(transDate), name(name), book_id(book_id), book_name(book_name), status(status) , dueDate(dueDate) {}

void Transaction::displayTransaction() const {
	cout << "User ID: " << id
		<< ", Date: " << transDate
		<< ", Name: " << name
		<< ", Book ID: " << book_id
		<< ", Book Name: " << book_name
		<< ", Status: " << status 
		<< "' DueDate: " << dueDate << "\n";
}

int Transaction::getBookID() const {
	return book_id;
}

int Transaction::getID() const {
	return id;
}

string Transaction::getUserName() const {
	return name;
}

string Transaction::getBookName() const {
	return book_name;
}

string Transaction::getStatus() const {
	return status;
}

void Transaction::setID(int id) {
	id = id;
}

void Transaction::setBookID(int book_id) {
	book_id = book_id;
}

void Transaction::setUserName(const string& name) {
	this->name = name;
}

void Transaction::setBookName(const string& book_name) {
	this->book_name = book_name;
}

void Transaction::setStatus(const string& status) {
	this->status = status;
}

string Transaction::getDueDate() const {
	return dueDate;
}

string Transaction::getDate() const {
	return transDate;
}