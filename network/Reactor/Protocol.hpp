#pragma once
#include <string>
#include <cstring>
#include <jsoncpp/json/json.h>

#include "../Log.h"

#define SEP "||"
#define SEP_LENGTH strlen(SEP)

class Request{
public:
    std::string Serialize()
    {
        Json::Value root;
        root["x"] = x;
        root["op"] = op;
        root["y"] = y;
        Json::StyledWriter writer;
        return writer.write(root);
    }
    bool Deserialize(const std::string& buffer)
    {
        Json::Reader reader;
        Json::Value root;
        if(!reader.parse(buffer,root)) return false;
        x = root["x"].asInt();
        y = root["y"].asInt();
        op = root["op"].asString();
        return true;
    }
public:
    int x;
    int y;
    std::string op;
};

class Response{
public:
    std::string Serialize() const
    {
        Json::Value root;
        root["res"] = res;
        root["status"] = status;
        Json::StyledWriter writer;
        return writer.write(root);
    }
    bool Deserialize(const std::string& buffer)
    {
        Json::Reader reader;
        Json::Value root;
        if(!reader.parse(buffer,root))
            return false;
        res = root["res"].asInt();
        status = root["status"].asInt();
        return true;
    }
public:
    int res;
    int status;
};

void Encode(std::string& buffer,std::vector<Request>* reqs)
{
    while(true)
    {
        size_t pos = buffer.find(SEP,0);
        if(pos == std::string::npos) break;
        size_t len = std::stoul(buffer.substr(0,pos));
        buffer.erase(0,pos+SEP_LENGTH);
        if(buffer.size() < len) break;
        Request req;
        req.Deserialize(buffer.substr(0,len));
        reqs->push_back(req);
        buffer.erase(0,len);
    }

}
void Decode(const Response& resp,std::string* outbuffer)
{
    if(outbuffer == nullptr) return;
    (*outbuffer) += resp.Serialize();
}
