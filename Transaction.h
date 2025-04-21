#include <string>
#include <ctime>
using namespace std;

class Transaction {

private:
	int id;
	string name;
	int book_id;
	string book_name;
	string status;
	string dueDate;
	string transDate;


public:
	Transaction(int id, const string& transDate, const string& name, int book_id, const string& book_name, const string& status, const string& dueDate);
	
	void displayTransaction() const;
	int getID() const;
	int getBookID() const;
	string getBookName() const;
	string getUserName() const;
	string getStatus() const;
	void setID(int id);
	void setBookID(int book_id);
	void setBookName(const string& book_name);
	void setUserName(const string& name);
	void setStatus(const string& status);
	string getDate() const;
	string getDueDate() const;
};
