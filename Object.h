#ifndef _OBJECT_H
#define _OBJECT_H

#include <string>
#include "Logging.h"

class Object
{
    public:
        virtual ~Object() {}

        bool StringCompare(std::string & str1, std::string &str2)
        {
            //LOGD("<Object::StringCompare> %s == %s (%u <> %u)\n", str1.c_str(), str2.c_str(), str1.size(), str2.size());
            return ((str1.size() == str2.size()) && std::equal(str1.begin(), str1.end(), str2.begin(), [](char & c1, char & c2){
                                        return (c1 == c2 || std::toupper(c1) == std::toupper(c2));
                                    }));
        }

        bool StringCompare(std::string & str1, const char *str2)
        {
            std::string str3(str2);

            return StringCompare(str1, str3);
        }
};

#endif /* _OBJECT_H */