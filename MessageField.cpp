#include "MessageField.h"

MessageField::MessageField(json& object):
    mType(MFT_UNKNOWN), mSize(0)
{
    for (auto& [key, value] : object.items()) 
    {
        if (!key.compare("name"))
        {
            if (value.is_string())
            {
                mName = value.get<std::string>();            
            }
            else
            {
                LOGE("Expecting string\n");
            }            
        }
        else if (!key.compare("identifier"))
        {
            DecodeFieldType(value);
        }
        else if (!key.compare("value"))
        {
            if (value.is_string())
            {
                
            }
            else if (value.is_array())
            {

            }
        }
    }
}

MessageField::~MessageField()
{

}

void MessageField::DecodeFieldType(json& object)
{
    if (object.is_string())
    {
        std::string str = object.get<std::string>();
        if (!str.compare("uint8_t"))
        {
            mType = MFT_UINT8;
            mSize = sizeof(uint8_t);
        }
        else if (!str.compare("uint16_t"))
        {
            mType = MFT_UINT16;
            mSize = sizeof(uint16_t);
        }
        else if (!str.compare("uint32_t"))
        {
            mType = MFT_UINT32;
            mSize = sizeof(uint32_t);
        }
        else if (!str.compare("uint64_t"))
        {
            mType = MFT_UINT64;
            mSize = sizeof(uint64_t);
        }
        else if (!str.compare("int8_t"))
        {
            mType = MFT_INT16;
            mSize = sizeof(int8_t);
        }
        else if (!str.compare("int16_t"))
        {
            mType = MFT_INT16;
            mSize = sizeof(int16_t);
        }
        else if (!str.compare("int32_t"))
        {
            mType = MFT_INT32;
            mSize = sizeof(int32_t);
        }
        else if (!str.compare("int64_t"))
        {
            mType = MFT_INT32;
            mSize = sizeof(int64_t);
        }
        else if (!str.compare("uuid"))
        {
            mType = MFT_UUID;
            mSize = 16;
        }
    }
}
