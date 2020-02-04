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
    mContainerIterator = mContainerList.begin();

    return (mContainerIterator != mContainerList.end());
}

bool JsonReader::ForeachContainerNext()
{
    return ((++mContainerIterator) != mContainerList.end());
}

bool JsonReader::ForeachFieldReset()
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

bool JsonReader::OutputContainer(FILE *outputFile, std::string& name, Tag* tag)
{
    bool result = false;

    if (mContainerIterator != mContainerList.end())
    {
        result = (*mContainerIterator).Output(outputFile, name, tag);
    }

    return result;
}

bool JsonReader::OutputField(FILE *outputFile, std::string& name, Tag* tag)
{
    bool result = true;

    if (mContainerIterator != mContainerList.end())
    {
        result = (*mContainerIterator).OutputField(outputFile, name, tag);
    }

    return result;
}

