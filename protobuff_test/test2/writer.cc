#include <iostream>
#include <fstream>

#include "test.pb.h"

using namespace std;

void AddContactPeople(contacts::PeopleInfo* people)
{
    if(!people) return;
    string name;
    int age;
    cin.ignore(256,'\n');
    cout << "请输入name:>";
    getline(cin,name);
    cout << "请输入age>:";
    cin >> age;
    cin.ignore(256,'\n');

    for(int i = 0;;i++)
    {
        contacts::PeopleInfo_Phone* phone = people->add_phone();
        for(int j = 0; ; j++)
        {
            cout << "请输入telephone>:";
            string phone_num;
            getline(cin, phone_num);
            if(phone_num.empty()) break;
            phone->set_phone_number(phone_num);
        }
    }
}

int main()
{
    fstream input("contacts.bin", ios::binary | ios::in);
    contacts::Contacts contacts;
    if(!input.is_open())
    {
        cout << "不存在contacts.bin文件,直接创建" << endl;
    }
    else if(!contacts.ParseFromIstream(&input))
    {
        cerr << "contacts.bin文件解析错误" << endl;
        input.close();
    }
    AddContactPeople(contacts.add_people());
    
    fstream output("contacts.bin",ios::out | ios::trunc | ios::binary);
    if(!contacts.SerializeToOstream(&output))
    {
        cerr << "序列化失败" << endl;
    }
    else cout << "序列化成功" << endl;
    
    input.close();
    output.close();
    return 0;
}