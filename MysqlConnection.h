//
// Created by njustgzy on 23-1-6.
//

#ifndef MYSQLPOOL_MYSQLCONNECTION_H
#define MYSQLPOOL_MYSQLCONNECTION_H

#include <iostream>
#include <string>
#include <mysql/mysql.h>
#include <memory>
#include <time.h>
#include <chrono>
using namespace std;
using namespace chrono;



class MysqlConnection{

public:
    //init
    MysqlConnection();

    //destroy
    ~MysqlConnection();

    //connect
    bool connect(string user,string passwd,string dbName,string ip,short port = 3306);

    //update
    int update(string sql);

    //query
    int query(string sql);

    //list all
    bool next();

    string value(int index);

    //event
    bool transaction();

    //commit
    bool commit();

    //roll
    bool rollBack();

    void refreshAliveTime();
    long long getAliveTime();


private:
    MYSQL* sqlConn= nullptr;
    MYSQL_RES* sqlRes = nullptr;
    MYSQL_ROW  sqlRow = nullptr;
    void freeRes();

    steady_clock::time_point liveTime;

};


#endif //MYSQLPOOL_MYSQLCONNECTION_H
