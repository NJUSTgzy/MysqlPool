//
// Created by njustgzy on 23-1-6.
//

#include "SqlPool.h"
#include <jsoncpp/json/json.h>
using namespace Json;
#include <fstream>
#include <stdio.h>
#include <thread>


SqlPool *SqlPool::getSqlPool() {
    static SqlPool pool;
    return &pool;
}

SqlPool::SqlPool() {
    if(!pasreJson()){
        ::fprintf(stderr,"pasreJson() error");
    }

    for(int i=0;i<minSize;i++){
        CreateSqlConnection();
    }

    //chuan jian
    thread producder(&SqlPool::produceConnection, this);
    thread recycler(&SqlPool::recycleConnection, this);
    producder.detach();
    recycler.detach();

}

bool SqlPool::pasreJson() {
    ifstream ifs("config.json");
    Reader rd;
    Value root;
    rd.parse(ifs,root);

    if(root.isObject()){
        sqlDbName = root["dbName"].asString();
        sqlIp = root["ip"].asString();
        sqlPort = root["port"].asInt();
        sqlPwd = root["pwd"].asString();
        sqlUser = root["user"].asString();
        maxSize = root["maxSize"].asInt();
        minSize = root["minSiaze"].asInt();
        maxWaitTime = root["maxWaitTime"].asInt();
        timeOut = root["timeOut"].asInt();

        return true;
    }
    return false;

}

void SqlPool::CreateSqlConnection(){
    MysqlConnection* conn = new MysqlConnection;
    conn->connect(sqlUser,sqlPwd,sqlDbName,sqlIp,sqlPort);
    conn->refreshAliveTime();
    sqlConnectionsQueue.push(conn);
}

void SqlPool::produceConnection() {

    while (1){

        unique_lock<mutex> locker(mutexQueue);

        while (sqlConnectionsQueue.size()>=minSize){
            cond.wait(locker);
        }
        CreateSqlConnection();
        cond.notify_all();
    }

}

void SqlPool::recycleConnection() {

    while (1){

        this_thread::sleep_for(chrono::seconds(1));
        mutexQueue.lock();
        while (sqlConnectionsQueue.size()>=minSize){
            MysqlConnection* conn = sqlConnectionsQueue.front();
            if(conn->getAliveTime()>=maxWaitTime){
                sqlConnectionsQueue.pop();
                delete conn;
            }
        }
        mutexQueue.unlock();
    }

}

shared_ptr<MysqlConnection> SqlPool::getConnection() {

    unique_lock<mutex> locker(mutexQueue);
    while (sqlConnectionsQueue.empty()){
        if(cv_status::timeout==cond.wait_for(locker,chrono::milliseconds(timeOut)));{
            if(sqlConnectionsQueue.empty()){
                continue;
            }
        }

    }

    shared_ptr<MysqlConnection> conn (sqlConnectionsQueue.front(),[this](MysqlConnection* conn){
        mutexQueue.lock();
        sqlConnectionsQueue.push(conn);
        conn->refreshAliveTime();
        mutexQueue.unlock();
    });
    sqlConnectionsQueue.pop();
    cond.notify_all();
    return conn;
}

SqlPool::~SqlPool() {
    while (!sqlConnectionsQueue.empty()){
        MysqlConnection* conn =sqlConnectionsQueue.front();
        sqlConnectionsQueue.pop();
        delete conn;
    }
}


