#include <iostream>
#include "MysqlConnection.h"
#include "SqlPool.h"
#include <stdio.h>
#include <string.h>
using namespace std;


void pool(SqlPool* pool,int begin,int end){
    for (int i = begin; i < end; ++i) {
        shared_ptr<MysqlConnection> conn = pool->getConnection();
        conn->connect("root","11111111","test","localhost",3306);
        char sql[1024]={0};

        conn->update(sql);
    }
}
void noPool(int begin,int end){
    for(int i=begin;i<end;i++){
        MysqlConnection conn;
        conn.connect("root","11111111","test","localhost",3306);
        char sql[1024]={0};
        conn.update(sql);
    }
}


int main(int argc, char *argv[]){

    int begin=100000;
    int count=500;

    steady_clock::time_point s1 = steady_clock::now();
    noPool(begin,begin+count);
    steady_clock::time_point e1 = steady_clock::now();

    SqlPool* p = SqlPool::getSqlPool();
    steady_clock::time_point s2 = steady_clock::now();
    pool(p,begin+count,begin+2*count);
    steady_clock::time_point e2 = steady_clock::now();

    auto t1 = e1-s1;
    auto t2 = e2-s2;
    cout<<"not pool time ,one thread "<<t1.count()/1000000<<"ms"<<endl;
    cout<<"pool time ,one thread "<<t2.count()/1000000<<"ms"<<endl;
    cout<<"time differ: "<<t1.count()/t2.count()<<"times"<<endl;

    return 0;
}
