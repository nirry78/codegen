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
        else if (!key.compare("group"))
        {
            mGroup = value.get<std::string>();

            LOGD("  Group: %s\n", mGroup.c_str());
        }
        else
        {
            LOGE("Container has unregonized key: %s\n", key.c_str());
        }
    }

    ForeachFieldReset(NULL);
}

Container::~Container()
{

}

bool Container::ForeachFieldReset(Tag *tag)
{
    mFieldIterator = mFieldList.begin();
    mIteratorTag = tag;

    if (tag)
    {
        (*mFieldIterator).AcceptNameAndGroup(tag);
    }

    return (mFieldIterator != mFieldList.end());
}

bool Container::ForeachFieldNext()
{
    ++mFieldIterator;

    if (mIteratorTag && mFieldIterator != mFieldList.end())
    {
        (*mFieldIterator).AcceptNameAndGroup(mIteratorTag);
    }

    return (mFieldIterator != mFieldList.end());
}

void Container::Output(Document* document, std::string& name, Tag* tag)
{
    if (tag)
    {
        tag->Output(document, mName);
    }
    else
    {
        document->Output(mName);
    }
}

void Container::OutputField(Document* document, std::string& name, Tag* tag)
{
    if (mFieldIterator != mFieldList.end())
    {
        mFieldIterator->Output(document, name, tag);
    }
}

void Container::ParseParameters(json& object)
{
    for (auto& [key, value] : object.items()) 
    {
        (void)key;
        
        if (value.is_object())
        {
            mFieldList.push_back(Field(value));
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
