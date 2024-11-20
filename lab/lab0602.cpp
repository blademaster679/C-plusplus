#include <iostream>
#include <vector>
#include <string>
using namespace std;


// Here is the declaration of the User and Bank

class User {
private:
    string name;
    int deposit;
public:
    User(string name, int deposit);
    User(string name);
    string getName() const;
    int getDeposit() const;
    void setDeposit(int deposit);
    void depositMoney(int money);
    void withdrawMoney(int money);
};

class Bank {
private:
    vector<User> users;
public:
    bool addUser(User user);
    bool depositMoney(string name, int money);
    bool withdrawMoney(string name, int money);
    bool transferMoney(string name1, string name2, int money);
};

/*
you need implement all the functions in the class.
The implementations of the following 3 functions of class User have been provided as examples
*/
// e.g.
User::User(string name, int deposit) : name(name), deposit(deposit) {}
User::User(string name) : name(name){}
string User::getName() const
{
    return name;
}
int User::getDeposit() const
{
    return deposit;
}
void User::setDeposit(int deposit)
{
    this->deposit = deposit;
}
void User::depositMoney(int money)
{
    deposit += money;
}
void User::withdrawMoney(int money){
    deposit -= money;
}
bool Bank::addUser(User user){
    for (int i = 0;i < users.size();i++){
        if (users[i].getName() == user.getName()){
            return false;
        }
    }
    users.push_back(user);
    return true;
}
bool Bank::depositMoney(string name, int money){
    for (int i = 0; i <users.size();i++){
        if (users[i].getName() == name){
            users[i].depositMoney(money);
            return true;
        }
        return false;
    }
}
bool Bank::withdrawMoney(string name,int money){
    for (int i = 0;i < users.size(); i++){
        if (users[i].getName() == name){
            if (users[i].getDeposit() < money){
                return false;
            }
            users[i].withdrawMoney(money);
            return true;
        }
        return false;
    }
}
bool Bank::transferMoney(string name1, string name2, int money){
    for (int i = 0;i < users.size(); i++){
        if (users[i].getName() == name1){
            if (users[i].getDeposit() < money){
                return false;
            }
            users[i].withdrawMoney(money);
            for (int j = 0;j < users.size();j++){
                if (users[j].getName() == name2){
                    users[j].depositMoney(money);
                    return true;
                }
            }
            return false;
        }
    }
    return false;
}