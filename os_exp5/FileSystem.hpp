#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <string>
#include <chrono>
#include <ctime>
#include <cassert>
#include <iostream>
#include <cstring>

using namespace std;


class File{
public:
    File(const string& filename,short ugoAuthority,time_t create)
    :_name(filename),_ugoAuthority(ugoAuthority),_accessTime(create),_createTime(create),_modifyTime(create)
    {
        //creat出来的文件可以写入但不能读取，可以用open打开文件
        _fd = open(_name.c_str(),O_CREAT | O_TRUNC | O_RDWR,ugoAuthority);
        if(_fd < 0) assert(false);
    }
    int Write(const string& buffer,size_t size)
    {
        lockf(_fd,F_LOCK,size);
        int n = write(_fd,buffer.c_str(),size);
        lockf(_fd,F_UNLCK,size);
        if(n > 0) _size += n;
        _accessTime = _modifyTime = time(nullptr);
        return n;
    }
    int Read(string* buffer,size_t size)
    {
        //读取文件时，应该先讲文件指针指向文件开头
        lseek(_fd,0,SEEK_SET);
        assert(buffer);
        char buff[10240];
        lockf(_fd,F_LOCK,size);
        int n = read(_fd,buff,size);
        lockf(_fd,F_UNLCK,size);
        if(n > 0) buff[n] = 0;
        *buffer += buff;
        _accessTime = time(nullptr);

        // cout << _fd << " " << n << endl;
        // cout << strerror(errno);
        return n;
    }
    
    ~File()
    {
        if(_fd >= 0) close(_fd);
    }
public:
    int _fd = -1;
    string _name;
    size_t _size = 0;
    short _ugoAuthority; //只用到最后9比特位，高位到低位分别是user、group、others的rwx权限
    time_t _accessTime;
    time_t _createTime;
    time_t _modifyTime;
};

class FileSystem{
public:
    void DisplayDir()
    {
        cout << "-----------------------------\n";
        for(const auto& e : _files)
        {
            PrintFileInfo(e.second);
            cout << "-----------------------------\n";
        }
    }
    int Write(const string& filename,const string& buffer)
    {
        auto it = _files.find(filename);
        if(it == _files.end()) //说明文件不存在
            return -1;
        File* file = it->second;
        return file->Write(buffer,buffer.size());
    }
    int ReadAll(const string& filename,string* buffer)
    {
        if(buffer == nullptr) return -1;
        auto it = _files.find(filename);
        if(it == _files.end()) //说明文件不存在
            return -1;
        File* file = it->second;
        return file->Read(buffer,file->_size);
    }
    bool CreateNewFile(const string& filename,size_t authority)
    {
        auto it = _files.find(filename);
        if(it != _files.end()) //说明有同名文件，创建失败
            return false;
        File* file = new File(filename,authority,time(nullptr));
        _files[filename] = file;
        return true;
    }
    bool DeleteFile(const string& filename)
    {
        auto it = _files.find(filename);
        if(it == _files.end())
            return false;
        delete it->second;
        _files.erase(it);
        return true;
    }
    ~FileSystem()
    {
        for(auto& e : _files)
        {
            delete e.second;
        }
    }
    static FileSystem* GetInstance()
    {
        return &_sInst;
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
                if(file->_ugoAuthority & (1 << ((2-i)*3+2-j)))
                    auth[i][j] = authority[j];
                else auth[i][j] = '-';
            }
            auth[i][3] = 0;
        }
        cout << "user权限:" << auth[0] << "  group权限:" << auth[1] << "  others权限:" << auth[2] << endl;
        cout << "文件最后访问时间:";
        PrintTime(file->_accessTime);
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

    FileSystem()
    {}
    FileSystem(const FileSystem&) = delete;
    FileSystem& operator=(const FileSystem&) = delete;
private:
    static FileSystem _sInst;
    unordered_map<string,File*> _files;    
};