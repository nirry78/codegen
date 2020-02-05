#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include "JsonReader.h"

JsonReader::JsonReader()
{
    mContainerList.clear();
}

JsonReader::~JsonReader()
{
    mContainerList.clear();
}

void JsonReader::VerifyContainer(json& object)
{
    for (auto& [key, value] : object.items()) 
    {
        (void)key;
        
        if (value.is_object())
        {
            mContainerList.push_back(Container(value));
        }
    }
}

void JsonReader::VerifyParameters(Container& Container, json& object)
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
        std::cerr << "Parse error: " << e.what() << '\n';
        result = false;
    }

    try
    {
        for (auto& [key, value] : mJsonObject.items()) 
        {
            if (value.is_array())
            {
                if (!key.compare("containers"))
                {
                    std::cout << "Containers found\n";
                    VerifyContainer(value);
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

        ForeachContainerReset(NULL);
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

bool JsonReader::ForeachContainerReset(Tag *tag)
{
    mContainerIterator = mContainerList.begin();

    return (mContainerIterator != mContainerList.end());
}

bool JsonReader::ForeachContainerNext()
{
    return ((++mContainerIterator) != mContainerList.end());
}

bool JsonReader::ForeachFieldReset(Tag *tag)
{
    bool result = false;

    if (mContainerIterator != mContainerList.end())
    {
        result = (*mContainerIterator).ForeachFieldReset();
    }   

    return result;
}

bool JsonReader::ForeachFieldNext()
{
    bool result = false;

    if (mContainerIterator != mContainerList.end())
    {
        result = (*mContainerIterator).ForeachFieldNext();
    }   

    return result;
}

void JsonReader::OutputContainer(Document* document, std::string& name, Tag* tag)
{
    if (mContainerIterator != mContainerList.end())
    {
        (*mContainerIterator).Output(document, name, tag);
    }
}

void JsonReader::OutputField(Document* document, std::string& name, Tag* tag)
{
    if (mContainerIterator != mContainerList.end())
    {
        (*mContainerIterator).OutputField(document, name, tag);
    }
}

