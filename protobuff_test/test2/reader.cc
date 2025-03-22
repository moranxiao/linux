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
            cout << "电话号码[" << j+1 << "]:" << contacts.people(i).phone(j).phone_number();
            cout << "\t(" << contacts::PhoneType_Name(contacts.people(i).phone(j).phone_type()) << ")\n"; 
        }
        
        if(contacts.people(i).data().Is<contacts::PeopleInfo_Address>())
        {
            contacts::PeopleInfo_Address addr;
            contacts.people(i).data().UnpackTo(&addr);
            cout << "家庭住址:" << addr.home() << endl;
            cout << "工作地址:" << addr.work() << endl; 
        }

        switch(contacts.people(i).other_contact_case())
        {
            case contacts::PeopleInfo::OtherContactCase::kQq:
            cout << "QQ:" << contacts.people(i).qq() << endl;
            break;
            case contacts::PeopleInfo::OtherContactCase::kWx:
            cout << "WX:" << contacts.people(i).wx() << endl;
            break;
        }

        for(auto& it : contacts.people(i).remark())
        {
            cout << "标题:" << it.first << endl;
            cout << "内容:" << it.second << endl;
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