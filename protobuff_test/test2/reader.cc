#include <iostream>
#include <fstream>

#include "test.pb.h"

using namespace std;

void ShowContacts(const contacts::Contacts& contacts)
{
    for(int i = 0; i < contacts.people_size(); i++)
    {
        cout << "第" << i+1 << "位联系人\n";
        cout << "姓名："  << contacts.people(i).name() << endl;
        cout << "年龄："  << contacts.people(i).age() << endl;
        for(int j = 0; j < contacts.people(i).phone_size(); j++)
        {
            cout << "电话号码[" << j+1 << "]:" << contacts.people(i).phone(j).phone_number() << endl;
        }
    }
}

int main()
{
    fstream input("contacts.bin",ios::in | ios::binary);
    if(!input.is_open())
    {
        cout << "文件不存在" << endl;
        exit(1);
    }    
    contacts::Contacts contacts;
    if(!contacts.ParseFromIstream(&input))
    {
        cout << "反序列化失败" << endl;
        exit(2);
    }
    ShowContacts(contacts);
    input.close();
    return 0;
}