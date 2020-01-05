#include "Container.h"

Container::Container(json& object)
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
            LOGE("Container has unregonized key: %s\n", key.c_str());
        }
    }

    ForeachFieldReset();
}

Container::~Container()
{

}

bool Container::ForeachFieldReset()
{
    mFieldIterator = mFieldList.begin();

    return (mFieldIterator != mFieldList.end());
}

bool Container::ForeachFieldNext()
{
    return ((++mFieldIterator) != mFieldList.end());
}

bool Container::Output(FILE *outputFile, const char *name, Tag* tag)
{
    bool result = true;

    if (tag->GetTagStyle() == TAG_STYLE_LOWERCASE)
    {
        OutputLowerCase(outputFile, mName);
    }
    else if (tag->GetTagStyle() == TAG_STYLE_UPPERCASE)
    {
        OutputUpperCase(outputFile, mName);
    }
    else
    {
        fprintf(outputFile, "%s",  mName.c_str());
    }

    return result;
}

bool Container::OutputField(FILE *outputFile, const char *name, Tag* tag)
{
    bool result = false;

    if (mFieldIterator != mFieldList.end())
    {
        result = mFieldIterator->Output(outputFile, name, tag);
    }

    return result;
}

void Container::ParseParameters(json& object)
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

bool Container::IsValid()
{
    return true;
}
