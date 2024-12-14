#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <string>
#include <chrono>
#include <ctime>


using namespace std;


class File{
public:
    // int Write()
    // {

    // }
    // int Read()
    // {

    // }
    
public:
    int _fd;
    string _name;
    size_t _size;
    short _ugoAuthority; //只用到最后9比特位，低位到高位分别是user、group、others的rwx权限
    time_t _acessTime;
    time_t _createTime;
    time_t _modifyTime;
};

class FileSystem{
public:
    void DisplayDir()
    {
        for(const auto& e : _files)
        {
            PrintFileInfo(e.second);
            
        }
    }
    bool CreateNewFile(const string& filename)
    {

    }
private:
    void PrintFileInfo(File* file)
    {
        cout << "文件名称:" << file->_name << endl;
        cout << "文件大小" << file->_size << endl;
        cout << "文件访问权限";
        char auth[3][4];
        static const char* authority = "rwx";
        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if(file->_ugoAuthority & (1 << (i*3+j)))
                    auth[i][j] = authority[j];
                else auth[i][j] = '-';
            }
            auth[i][3] = 0;
        }
        cout << "user权限:" << auth[0] << "  group权限:" << auth[1] << "  others权限:" << auth[2] << endl;
        cout << "文件最后访问时间:";
        PrintTime(file->_acessTime);
        cout << "文件创建时间:";
        PrintTime(file->_createTime);
        cout << "文件最后修改时间:";
        PrintTime(file->_modifyTime);
    }

    //根据时间戳打印时间
    void PrintTime(time_t timestamp)
    {
        tm* local_time = localtime(&timestamp);
        std::cout << (local_time->tm_year + 1900) << "-"
            << (local_time->tm_mon + 1) << "-"
            << local_time->tm_mday << " "
            << local_time->tm_hour << ":"
            << local_time->tm_min << ":"
            << local_time->tm_sec << std::endl;
    }
private:
    unordered_map<string,File*> _files;    
};

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
int main()
{

    // int opt;
    // do{
    //     Print();
    //     cout << "请输入选项:>";
    //     cin >> opt;
    //     switch(opt)
    //     {
    //         case 0:
    //         break;
    //         case 1:
    //         break;
    //         case 2:
    //         break;
    //         case 3:
    //         break;
    //         case 4:
    //         break;
    //         case 5:
    //         break;
    //         case 6:
    //         break;
    //         default:
    //         cout << "输入选项无效，请再次输入" << endl;
    //         break;
    //     }
    // }while(opt);
    return 0;
}
