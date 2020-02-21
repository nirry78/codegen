#include "Container.h"

Container::Container(json& object):
    mFieldCount(0)
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
    bool result = false;

    mFieldIterator = mFieldList.begin();
    mIteratorTag = tag;

    if (mFieldIterator != mFieldList.end())
    {
        if (tag)
        {
            if (!(*mFieldIterator).AcceptNameAndGroup(tag))
            {
                result = ForeachFieldNext();
            }
            else
            {
                result = true;
            }            
        }
        else
        {
            result = true;
        }    
    }

    mFieldCount = 0;

    LOGD("<Container::ForeachFieldReset> count: %u, result: %u\n",
          mFieldCount, result);

    return result;
}

bool Container::ForeachFieldNext()
{
    bool result = false;

    ++mFieldIterator;

    if (mIteratorTag)
    {
        while (mFieldIterator != mFieldList.end())
        {
            result = (*mFieldIterator).AcceptNameAndGroup(mIteratorTag);
            if (result)
            {
                break;
            }
            
            ++mFieldIterator;
        }
    }
    else
    {
        result = (mFieldIterator != mFieldList.end());
    }

    if (result)
    {
        mFieldCount++;
    }
    
    LOGD("<Container::ForeachFieldNext> count: %u, result: %u\n",
          mFieldCount, result);

    return result;
}

void Container::Output(Document* document, std::string& name, Tag* tag, uint32_t count)
{
    LOGD("<Container::Output> name: %s, count: %u\n", name.c_str(), count);

    if (StringCompare(name, "name"))
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
    else if (StringCompare(name, "count") && tag)
    {
        tag->Output(document, "%u", count);
    }
}

void Container::OutputField(Document* document, std::string& name, Tag* tag)
{
    if (mFieldIterator != mFieldList.end())
    {
        mFieldIterator->Output(document, name, tag, mFieldCount);
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
