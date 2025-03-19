#include "test1.pb.h"
#include <iostream>

using namespace std;

int main()
{
    contacts::PeopleInfo p1;
    p1.set_age(21);
    p1.set_name("张三");
    string people_str;
    if(!p1.SerializeToString(&people_str)) 
    {
        cerr << "序列化失败" << endl;
        return 0;
    }
    cout << "序列化成功\n" << people_str << endl;
    contacts::PeopleInfo p2;
    if(!p2.ParseFromString(people_str))
        cerr << "反序列化失败" << endl;
    else
        cout << "序列化成功" << endl \
        << "name:" << p2.name()\
        << "age:" << p2.age() << endl;
    return 0;
}