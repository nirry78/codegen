#include "Message.h"

Message::Message(json& object)
{
    for (auto& [key, value] : object.items()) 
    {
        if (!key.compare("name"))
        {
            mName = value.get<std::string>();

            LOGD("  Name: %s\n", mName.c_str());
        }
        else if (!key.compare("parameters"))
        {
            if (value.is_array())
            {
                ParseParameters(value);
            }
            else
            {
                LOGE("Parameters must be an array\n");
            }
        }
        else
        {
            LOGE("Message has unregonized key: %s\n", key.c_str());
        }
    }

    ForeachFieldReset();
}

Message::~Message()
{

}

bool Message::ForeachFieldReset()
{
    mFieldIterator = mFieldList.begin();

    return (mFieldIterator != mFieldList.end());
}

bool Message::ForeachFieldNext()
{
    return ((++mFieldIterator) != mFieldList.end());
}

bool Message::Output(FILE *outputFile, const char *name)
{
    bool result = true;

    fprintf(outputFile, "%s", mName.c_str());

    return result;
}

bool Message::OutputField(FILE *outputFile, const char *name)
{
    bool result = false;

    if (mFieldIterator != mFieldList.end())
    {
        result = mFieldIterator->Output(outputFile, name);
    }

    return result;
}

void Message::ParseParameters(json& object)
{
    for (auto& [key, value] : object.items()) 
    {
        (void)key;
        
        if (value.is_object())
        {
            mFieldList.push_back(MessageField(value));
        }
        else
        {
            LOGE("Expecting object");
        }        
    }
}

bool Message::IsValid()
{
    return true;
}
