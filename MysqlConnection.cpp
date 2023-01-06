//
// Created by njustgzy on 23-1-6.
//

#include "MysqlConnection.h"

MysqlConnection::MysqlConnection() {
    sqlConn = mysql_init(nullptr);
    mysql_set_character_set(sqlConn,"utf8");
}

MysqlConnection::~MysqlConnection() {

    freeRes();
    if(sqlConn!=nullptr){
        mysql_close(sqlConn);
    }
}

bool MysqlConnection::connect(string user, string passwd, string dbName, string ip, short port) {
    mysql_real_connect(sqlConn,ip.c_str(),user.c_str(),passwd.c_str(),dbName.c_str(),port, nullptr,0);
    if(sqlConn== nullptr)
        return false;
    else return true;
}

int MysqlConnection::update(string sql) {
    int res = mysql_query(sqlConn,sql.c_str());
    return res;
}

int MysqlConnection::query(string sql) {
    freeRes();
    int res = mysql_query(sqlConn,sql.c_str());
    cout<<"query res:"<<res<<"   query sql:"<<sql<<endl;
    if(res==0){
        return 0;
    }
    sqlRes = mysql_store_result(sqlConn);
    return res;
}

bool MysqlConnection::next() {
    if(sqlRes== nullptr)
        return false;

    sqlRow = mysql_fetch_row(sqlRes);
    return true;
}

string MysqlConnection::value(int index) {
    int rCount = mysql_num_fields(sqlRes);

    if(index<0 || index>=rCount){
        return string() ;
    }

    char * val = sqlRow[index];
    return val;
}

bool MysqlConnection::transaction() {

    return mysql_autocommit(sqlConn, false);
}

bool MysqlConnection::commit() {
    return mysql_commit(sqlConn);
}

bool MysqlConnection::rollBack() {
    return mysql_rollback(sqlConn);
}

void MysqlConnection::freeRes() {
    if(sqlRes!= nullptr){
        mysql_free_result(sqlRes);
        sqlRes= nullptr;
    }
}

void MysqlConnection::refreshAliveTime() {
    liveTime = steady_clock::now();

}

long long MysqlConnection::getAliveTime() {
    nanoseconds  res = steady_clock::now() - liveTime;
    milliseconds mi = duration_cast<milliseconds>(res);
    return mi.count();
}
