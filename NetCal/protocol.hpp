#include <string>
#include <iostream>
#include "Log.hpp"

#define SPACE " "
#define SPACE_LEN 1

namespace NetCal
{
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
            std::string str;
            str += std::to_string(code_);
            str += SPACE;
            str += std::to_string(result_);
            return str;
        }
        bool DeSerialize(const std::string &str)
        {
            size_t pos = str.find(SPACE);
            if (pos == std::string::npos)
            {
                logMessage(ERROR, "DeSerialize");
                return false;
            }
            code_ = atoi(str.substr(0, pos).c_str());
            result_ = atoi(str.substr(pos + SPACE_LEN).c_str());
            return true;
        }

    private:
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
            std::string str;
            str += std::to_string(x_);
            str += SPACE;
            str += op_;
            str += SPACE;
            str += std::to_string(y_);
            return str;
        }
        bool DeSerialize(const std::string &str)
        {
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

    private:
        int x_;
        int y_;
        char op_;
    };


}
