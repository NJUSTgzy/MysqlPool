//
// Created by njustgzy on 23-1-6.
//

#ifndef MYSQLPOOL_SQLPOOL_H
#define MYSQLPOOL_SQLPOOL_H

#include <queue>
#include <iostream>
#include <string>
#include "MysqlConnection.h"
#include <mutex>
#include <condition_variable>
using namespace std;

class SqlPool {
public:
    //dan li
    static SqlPool* getSqlPool();
    shared_ptr<MysqlConnection>  getConnection();
    ~SqlPool();
private:
    SqlPool();
    SqlPool(const SqlPool& obj);
    SqlPool& operator=(const SqlPool& obj);

    bool pasreJson();
    void CreateSqlConnection();


    void produceConnection();
    void recycleConnection();

    string sqlIp;
    short  sqlPort;
    string sqlUser;
    string sqlPwd;
    string sqlDbName;

    queue<MysqlConnection*> sqlConnectionsQueue;
    int minSize;
    int maxSize;
    int timeOut;
    int maxWaitTime;
    mutex mutexQueue;
    condition_variable cond;

};


#endif //MYSQLPOOL_SQLPOOL_H
