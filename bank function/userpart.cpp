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
void registerFunc(string username,string password, double balance){
    int id = 0;
    bool sameid = true, skipHeader = true;
    vector<User> users;
    User u;
    
    u.password=password;
    u.username=username;
    ifstream readfile("../database/demo_user.txt");
    string line;

    while (getline(readfile,line)){
        if (line == "" || line == "\r") continue;
        if (skipHeader) { skipHeader = false; id++; continue; }
        id++;
        vector<string> data = splitData(line,'|');
        if(data.size() >2 && u.username == data[2]){
            cout<<"Username already exists!"<<endl;
            return;
        }
    }
    readfile.close();

    while(sameid){
        sameid = false;
        u.userid = rand()%900000 + 100000;
        ifstream infile("../database/demo_user.txt");
        string line;
        while (getline(infile,line))
        {
            vector<string> data = splitData(line,'|');
            if(data.size() > 1 && to_string(u.userid) == data[1]){
                sameid = true;
                break;
            }
        }
    }

    u.id = id;
    string salt = generateSalt();
    string hash = hashPassword(password, salt);
    u.salt = salt;
    u.balance = balance;

    users.push_back(u);
    ofstream user;
    user.open("../database/demo_user.txt",ios::app);
    user << u.id<<'|'<< u.userid <<'|'<<u.username<<'|'<<hash<<'|'<<u.salt<<'|'<<u.balance<<endl;
    user.close();
}
int main() {
    registerFunc("eq", "littlebear",0);
    registerFunc("chenchoy", "kontairakjing",1000);
    registerFunc("eq", "dogeater",500);//same username case test
    //
    return 0;
}