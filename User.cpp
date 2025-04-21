#include <string>
#include <vector>
#include "User.h"
using namespace std;
using std::vector;


User:: User(int id, const string& str, const string& hasBorrowed) : id(id), name(str), hasBorrowed(hasBorrowed) {}

int User:: getId() const {
	return id;
}
string User::getName() const {
	return name;
}
void User::setId(int id) {
	id = id;
}
void User::setName(const string& str) {
	name = str;
}

string User::getHasBorrowed() const {
	return hasBorrowed;
}
