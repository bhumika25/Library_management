#include <string>
#include <vector>
using namespace std;
using std::vector;

class User {
private:
	int id;
	string name;
	string hasBorrowed;

public:
	User(int id, const string& str, const string& borrowed);
	int getId() const;
	string getName() const;
	void setId(int id);
	void setName(const string& str);
	string getHasBorrowed() const;
};