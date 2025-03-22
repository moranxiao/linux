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
        cout << "请输入电话类型: 1. 移动电话  2.固定电话 >:";
        int op;
        cin >> op;
        cin.ignore(256,'\n');
        switch(op)
        {
            case 1:
            phone->set_phone_type(contacts::PhoneType::MP);
            break;
            case 2:
            phone->set_phone_type(contacts::PhoneType::TEL);
            break;
            default:
            cout << "输入有误！" << endl;
            break;
        }

    }
    string home_address,work_address;
    cout << "请输入地址信息\n";
    cout << "家庭地址:>";
    getline(cin,home_address);
    cout << "工作地址:>";
    getline(cin,work_address);
    google::protobuf::Any* data = people->mutable_data();
    contacts::PeopleInfo_Address addr;
    addr.set_home(home_address);
    addr.set_work(work_address);
    data->PackFrom(addr);

    cout << "输入额外联系方式:> 1. 微信  2.QQ\n";
    int op;
    cin >> op;
    cin.ignore(256,'\n');
    string info;
    getline(cin,info);
    switch(op)
    {
        case 1:
        people->mutable_wx()->swap(info);
        break;
        case 2:
        people->mutable_qq()->swap(info);
        break;
        default:
        cout << "输入错误" << endl;
        break;
    }

    for(int i = 0; ; i++)    
    {
        cout << "请输入备注i的内容（标题和内容）>:";
        string title,info;
        getline(cin,title);
        if(title.empty()) break;
        getline(cin,info);
        people->mutable_remark()->insert({title,info});
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