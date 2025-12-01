#ifndef USERSYSTEM_H
#define USERSYSTEM_H

#include <string>
#include <vector>

using namespace std;

class UserSystem
{
public:
	void account_log_in(void);
	void save(void);
private:
	string Index;
	string Username;
	string Password;

	void log_password(void);
	int search_account(string account);
	void splitstring(const string& s, vector<string>& v, const string& c);
};

#endif
