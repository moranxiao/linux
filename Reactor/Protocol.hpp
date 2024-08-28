#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "Log.hpp"
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <jsoncpp/json/json.h>


#define SPACE " "
#define SPACE_LEN 1

#define SEP  "\r\n"
#define SEP_LEN  strlen(SEP)

#define MYSELF 1

#define SPLIT_CHAR 'x'


void SplitMessage(std::string& inbuffer,std::vector<std::string>* out)
{
    if(!out) return;
    while(true)
    {
        size_t pos = inbuffer.find(SPLIT_CHAR);
        if(pos != std::string::npos)
        {
            out->push_back(inbuffer.substr(0,pos));
            inbuffer.erase(0,pos+1);
        }
        else
            break;
    }
}

class Response
{
public:
    Response(int code, int result)
        : code_(code), result_(result)
    {
    }
    Response() {}
    std::string Serialize()
    {
#ifdef MYSELF
        std::string str;
        str += std::to_string(code_);
        str += SPACE;
        str += std::to_string(result_);
        return str;
#else
        Json::Value root;
        root["code"] = code_;
        root["result"] = result_;
        Json::FastWriter writer;
        return writer.write(root);
#endif
    }
    bool DeSerialize(const std::string &str)
    {
#ifdef  MYSELF
        size_t pos = str.find(SPACE);
        if (pos == std::string::npos)
        {
            logMessage(ERROR, "DeSerialize");
            return false;
        }
        code_ = atoi(str.substr(0, pos).c_str());
        result_ = atoi(str.substr(pos + SPACE_LEN).c_str());
        return true;
#else
        Json::Value root;
        Json::Reader reader;
        if(!reader.parse(str,root)) return false;
        code_ = root["code"].asInt();
        result_ = root["result"].asInt();
        return true;
#endif
    }

public:
    int code_;
    int result_;
};
class Request
{
public:
    Request(int x, int y, char op)
        : x_(x), y_(y), op_(op)
    {
    }
    Request() {}
    std::string Serialize()
    {
#ifdef MYSELF
        std::string str;
        str += std::to_string(x_);
        str += SPACE;
        str += op_;
        str += SPACE;
        str += std::to_string(y_);
        return str;
#else
        Json::Value root;
        root["x"] = x_;
        root["y"] = y_;
        root["op"] = op_;
        Json::FastWriter writer;
        return writer.write(root);
        
#endif       
    }
    bool DeSerialize(const std::string &str)
    {
#ifdef MYSELF

        size_t pos1 = str.find(SPACE);
        if (pos1 == std::string::npos)
        {
            logMessage(ERROR, "DeSerialize");
            return false;
        }
        size_t pos2 = str.find_last_of(SPACE);
        if (pos2 == std::string::npos || pos2 == pos1)
        {
            logMessage(ERROR, "DeSerialize");
            return false;
        }
        x_ = atoi(str.substr(0, pos1).c_str());
        op_ = str[pos1 + SPACE_LEN];
        y_ = atoi(str.substr(pos2 + SPACE_LEN).c_str());
        return true;
#else 
        Json::Value root;
        Json::Reader reader;
        reader.parse(str,root);
        x_ = root["x"].asInt();
        y_ = root["y"].asInt();
        op_ = root["op"].asInt();
        
#endif       
    }
    Response Execute()
    {
        int code, result;
        switch (op_)
        {
        case '+':
            result = x_ + y_;
            code = 0;
            break;
        case '-':
            result = x_ - y_;
            code = 0;
            break;
        case '*':
            result = x_ * y_;
            code = 0;
            break;
        case '/':
            if(y_ == 0)
            {
                code = 1;
                break;
            }
            result = x_ / y_;
            code = 0;
            break;
        case '%':
            if(y_ == 0)
            {
                code = 2;
                break;
            }
            result = x_ % y_;
            code = 0;
            break;
        default:
            code = 3;
            break;
        }
        return Response(code,result);
    }

public:
    int x_;
    int y_;
    char op_;
};



//定制传输控制协议 "LEN\r\n******\r\n"
std::string EnCode(const std::string& str)
{   
    std::string res;
    res += std::to_string(str.length());
    res += SEP;
    res += str;
    res += SEP;
    return res;
}

std::string DeCode(std::string& str) //此处str代表tcp传输的字节流数据，不一定是完整报文
{
    size_t pos = str.find(SEP);
    if(pos == std::string::npos)
        return "";
    int len = atoi(str.substr(0,pos).c_str());
    if(len > str.length() - SEP_LEN * 2 - pos)
    {
        return "";
    }
    std::string res = str.substr(pos+SEP_LEN,len);
    str.erase(0,pos+SEP_LEN*2+len);
    return res;
}
void Send(int sock, const std::string str)
{
    ssize_t n = send(sock,str.c_str(),str.size(),0);
    if(n < 0)
        std::cout << "Send Error" << std::endl;    
}
bool Recv(int sock,std::string* s)
{
    if(s == nullptr)  return false;
    char buffer[1024];
    ssize_t sz = recv(sock,buffer,sizeof(buffer)-1,0);
    //这个判断是必要的，当客户端关闭时，read\recv会返回0,如果此时依旧往下走，还往此文件描述符里write的话
    //系统会发送SIGPIPE信号，关闭进程
    //但是只判断也会有问题，当正在发送的时候，客户端关闭，还是会有SIGPIPE信号
    //一般经验：将SIGPIPE信号忽略，防止运行中出现非法写入的问题
    if(sz <= 0)
        return false;
    buffer[sz] = 0;
    *s += buffer;
    return true;
}