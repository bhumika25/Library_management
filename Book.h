#include <string>
using namespace std;

class Book {

private:
	int book_id;
	string book_name;
	string author_name;
	string status;

public:
	Book(int book_id, const string& book_name, const string& author_name, const string& status);

	int getBookID() const;
	string getBookName() const;
	string getAuthorName() const;
	string getStatus() const;
	void setBookID(int book_id);
	void setBookName(const string& book_name);
	void setAuthorName(const string& author_name);
    void displayBook() const;
    string displayBooks() const;
};
