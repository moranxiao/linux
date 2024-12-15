#include "FileSystem.hpp"

//单例模式：饿汉模式
FileSystem FileSystem::_sInst; 

void Print()
{
    cout << "********************************************" << endl;
    cout << "****\t1.创建新文件\t2.删除文件\t****" << endl;
    cout << "****\t3.读文件\t4.写文件\t****" << endl;
    cout << "****\t5.查看文件目录\t0.退出文件系统\t****" << endl;
    cout << "********************************************" << endl;
}

void Test1()
{
    // File file;
    // file._name = "hehe";
    // file._acessTime = time(nullptr);
    // file._createTime = time(nullptr);
    // file._modifyTime = time(nullptr);
    // file._ugoAuthority = 367;
    // file._size = 1024;
    // FileSystem filesys;
    // filesys.PrintFileInfo(&file);
}

void Read()
{
    string filename;
    string buffer;
    cout << "请输入要读取文件名>:";
    cin >> filename;
    if(FileSystem::GetInstance()->ReadAll(filename,&buffer) < 0)
        cout << "读取失败" << endl;
    else cout << buffer << endl;
}
void Write()
{
    string filename;
    string line;
    string buffer;
    cout << "请输入要读取文件名>:";
    cin >> filename;
    cout << "请输入要写入的内容(输入一行quit结束)>:";
    //按行读取
    while(getline(cin,line))
    {
        if(line == "quit") break;
        buffer += line;
    }
    if(FileSystem::GetInstance()->Write(filename,buffer) < 0)
        cout << "写入文件失败" << endl;
    else{
        cout << "写入成功" << endl;
    }
}
void Create()
{
    string filename;
    short auth;
    cout << "请输入文件名和权限 例如:test 755\n>:";
    cin >> filename;
    //已八进制形式读取权限
    scanf("%ho",&auth);
    if(!FileSystem::GetInstance()->CreateNewFile(filename,auth))
    {
        cout << "输入文件名已存在" << endl;
    }
    else cout << "创建文件成功" << endl;
}
void Delete()
{
    string filename;
    cout << "请输入要删除的文件名>:";
    cin >> filename;
    if(!FileSystem::GetInstance()->DeleteFile(filename))
    {
        cout << "输入文件名不存在" << endl;
    }
    else cout << "删除文件成功" << endl;
}
int main()
{
    int opt;
    do{
        Print();
        cout << "请输入选项:>";
        cin >> opt;
        string filename;
        string buffer;
        int auth;
        switch(opt)
        {
            case 0:
            break;
            case 1:
            Create();
            break;
            case 2:
            Delete();
            break;
            case 3:
            Read();
            break;
            case 4:
            Write();
            break;
            case 5:
            FileSystem::GetInstance()->DisplayDir();
            break;
            default:
            cout << "输入选项无效，请再次输入" << endl;
            break;
        }
    }while(opt);
    cout << "已退出文件系统，欢迎下次使用！" << endl;
    return 0;
}
