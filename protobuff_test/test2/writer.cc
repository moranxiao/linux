#include <iostream>
#include <fstream>

#include "test.pb.h"

using namespace std;

void AddContactPeople(contacts::PeopleInfo* people)
{
    cout << "------------------------添加联系人------------------------" << endl;
    if(!people) return;
    string name;
    int age;
    cout << "请输入name:>";
    getline(cin,name);
    people->set_name(name);
    cout << "请输入age>:";
    cin >> age;
    people->set_age(age);
    cin.ignore(256,'\n');

    for(int i = 0;;i++)
    {
        cout << "请输入telephone[" << i+1 <<  "]>:";
        string phone_num;
        getline(cin, phone_num);
        if(phone_num.empty()) break;
        contacts::PeopleInfo_Phone* phone = people->add_phone();
        phone->set_phone_number(phone_num);
    }
    cout << "------------------------添加完成------------------------" << endl;
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