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

        ForeachContainerReset();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        result = false;
    }

    return result;
}

bool JsonReader::SelectNamespace(const char *value)
{
    bool result = false;

    return result;
}

bool JsonReader::ForeachContainerReset()
{
    mMessageIterator = mMessageList.begin();

    return (mMessageIterator != mMessageList.end());
}

bool JsonReader::ForeachContainerNext()
{
    return ((++mMessageIterator) != mMessageList.end());
}

bool JsonReader::ForeachFieldReset()
{
    bool result = false;

    if (mMessageIterator != mMessageList.end())
    {
        result = (*mMessageIterator).ForeachFieldReset();
    }   

    return result;
}

bool JsonReader::ForeachFieldNext()
{
    bool result = false;

    if (mMessageIterator != mMessageList.end())
    {
        result = (*mMessageIterator).ForeachFieldNext();
    }   

    return result;
}

bool JsonReader::OutputContainer(FILE *outputFile, const char *name, Tag* tag)
{
    bool result = false;

    if (mMessageIterator != mMessageList.end())
    {
        result = (*mMessageIterator).Output(outputFile, name, tag);
    }

    return result;
}

bool JsonReader::OutputField(FILE *outputFile, const char *name, Tag* tag)
{
    bool result = true;

    if (mMessageIterator != mMessageList.end())
    {
        result = (*mMessageIterator).OutputField(outputFile, name, tag);
    }

    return result;
}

