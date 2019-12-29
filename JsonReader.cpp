#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "JsonReader.h"

JsonReader::JsonReader()
{
    mMessageList.clear();
}

JsonReader::~JsonReader()
{
    mMessageList.clear();
}

void JsonReader::VerifyMessages(json& object)
{
    for (auto& [key, value] : object.items()) 
    {
        (void)key;
        
        if (value.is_object())
        {
            mMessageList.push_back(Message(value));
        }
    }
}

void JsonReader::VerifyParameters(Message& message, json& object)
{
    uint32_t index = 0;
    LOGD("  Fields:\n");
    for (auto& [arrayKey, arrayValue] : object.items()) 
    {
        if (arrayValue.is_object())
        {
            
            LOGD("    [%u]:\n", index++);
/*            for (auto& [key, value] : arrayValue.items()) 
            {
                if (!key.compare("name"))
                {
                    LOGD("      Name: %s\n", value.get<std::string>().c_str());
                }
                else if (!key.compare("type"))
                {
                    if (value.is_string())
                    {
                        LOGD("      Type: %s\n", value.get<std::string>().c_str());
                    }
                }
            }*/
        }
    }
}

bool JsonReader::ReadFile(const char *filename)
{
    bool result = true;

    try
    {
        std::ifstream inFile(filename, std::ifstream::in);    

        inFile >> mJsonObject;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        result = false;
    }

    try
    {
        for (auto& [key, value] : mJsonObject.items()) 
        {
            if (value.is_array())
            {
                if (!key.compare("messages"))
                {
                    std::cout << "messages found\n";
                    VerifyMessages(value);
                }
                else
                {
                    std::cout << "Unsupprted section " << key << "\n";
                }            
            }
            else
            {
                std::cout << key << " : " << value << "\n";
            }
        }   
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        result = false;
    }

    return result;
}
