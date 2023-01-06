#include <iostream>
#include "MysqlConnection.h"
using namespace std;


void query(){
    MysqlConnection conn;
    conn.connect("root","11111111","test","localhost",3306);
    string sql = "insert into Student values(10,'B',1990-01-20,'man')";
    conn.update(sql);
    cout<<"query...."<<endl;
    while (conn.next()){
        cout<<conn.value(0)
        <<conn.value(1)
        <<conn.value(2)
        <<conn.value(3)
        <<endl;
    }
    return;
}


int main() {
    query();
    cout<<"end"<<endl;
    return 0;
}
