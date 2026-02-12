#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<random>
using namespace std;
struct User {
    int id;
    int userid;
    string username;
    string password;
    string salt;
    double balance;
};
vector<User> allUsers;
string hashPassword(const string& password, const string& salt) {
    hash<string> hasher;
    size_t hashed = hasher(password + salt);
    return to_string(hashed);
}
string generateSalt(int len = 16) {
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, sizeof(charset) - 2);
        string salt;
        for (int i = 0; i < len; i++) {
            salt += charset[dis(gen)];
        }
        return salt;
}
vector<string> splitData(string rowData, char seperator) {
    string dataToAppend;
    vector<string> result;
    int count = 0;
    for (int i = 0; i < rowData.size(); i++) {
        if (rowData[i] == seperator) {
            result.push_back(dataToAppend);
            dataToAppend = "";
        } else dataToAppend += rowData[i];
    }
    result.push_back(dataToAppend);
    return result;
}
void loadDataFromFile(){
    allUsers.clear();
    ifstream readfile("../database/demo_user.txt");
    string line;
    bool skipHeader = true;
    while (getline(readfile,line)){
        if (line == "" || line == "\r") continue;
        if (skipHeader) { skipHeader = false; continue; }
        vector<string> data = splitData(line,'|');
        User u;
        u.id = stoi(data[0]);
        u.userid = stoi(data[1]);
        u.username = data[2];
        u.password = data[3];
        u.salt = data[4];
        u.balance = stod(data[5]);
        allUsers.push_back(u);
    }
}
void registerFunc(string username,string password, double balance){
    bool skipHeader = true;
    User u;
    u.username=username;
    u.password=password;
    u.userid = rand() % 900000 + 100000;
    for (int i = 0; i < allUsers.size(); i++) {
        if (allUsers[i].username == username) {
            cout << "Username already exists!" << endl;
            return;
        }
    }
    for(int i=0;i<allUsers.size();i++){
        if(allUsers[i].userid == u.userid){
        u.userid = rand() % 900000 + 100000;
        i=0;
        }else{
        continue;
        }
    }
    u.id = allUsers.size() + 1;
    string salt = generateSalt();
    string hash = hashPassword(password, salt);
    u.salt = salt;
    u.balance = balance;
    allUsers.push_back(u);
    ofstream user;
    user.open("../database/demo_user.txt",ios::app);
    user << u.id<<'|'<< u.userid <<'|'<<u.username<<'|'<<hash<<'|'<<u.salt<<'|'<<u.balance<<endl;
    user.close();
}
int main() {
    loadDataFromFile();
    registerFunc("eq", "littlebear",0);
    registerFunc("chenchoy", "kontairakjing",1000);
    registerFunc("eq", "fahrakphor",500);//same username case test
    cout << "ID " << allUsers[0].id<<"UserID: " << allUsers[0].userid << " Username: " << allUsers[0].username << " Password: " << allUsers[0].password << " Salt: " << allUsers[0].salt << " Balance: " << allUsers[0].balance << endl;
    return 0;
}