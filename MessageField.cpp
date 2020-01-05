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
        else if (!key.compare("type"))
        {
            DecodeFieldType(value);
        }
        else if (!key.compare("value"))
        {
            if (value.is_string())
            {
                mValue = value.get<std::string>();
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

bool MessageField::Output(FILE *outputFile, const char *name, Tag* tag)
{
    if (!strcasecmp(name, "name"))
    {
        if (tag->GetTagStyle() == TAG_STYLE_LOWERCASE)
        {
            bool first = true;
            for ( std::string::iterator it = mName.begin(); it != mName.end(); ++it)
            {
                if (first)
                {
                    first = false;
                }
                else if (std::isupper(*it))
                {
                    fputc('_', outputFile);    
                }

                fputc(std::tolower(*it), outputFile);
            }
        }
        else if (tag->GetTagStyle() == TAG_STYLE_UPPERCASE)
        {
            bool first = true;
            for ( std::string::iterator it = mName.begin(); it != mName.end(); ++it)
            {
                if (first)
                {
                    first = false;
                }
                else if (std::isupper(*it))
                {
                    fputc('_', outputFile);    
                }
                
                fputc(std::toupper(*it), outputFile);
            }
        }
        else
        {
            fprintf(outputFile, "%s",  mName.c_str());
        }
    }
    else if (!strcasecmp(name, "type"))
    {
        switch (mType)
        {
            case MFT_UINT8:
            {
                fprintf(outputFile, "uint8_t");
                break;
            }
            case MFT_UINT16:
            {
                fprintf(outputFile, "uint16_t");
                break;
            }
            case MFT_UINT32:
            {
                fprintf(outputFile, "uint32_t");
                break;
            }
            case MFT_UINT64:
            {
                fprintf(outputFile, "uint64_t");
                break;
            }
            case MFT_INT8:
            {
                fprintf(outputFile, "int8_t");
                break;
            }
            case MFT_INT16:
            {
                fprintf(outputFile, "int16_t");
                break;
            }
            case MFT_INT32:
            {
                fprintf(outputFile, "int32_t");
                break;
            }
            case MFT_INT64:
            {
                fprintf(outputFile, "int64_t");
                break;
            }
            case MFT_UUID:
            {
                fprintf(outputFile, "uuid_t");
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else
    {
        LOGD("Unsupported field: %s\n", name);
    }
    

    return true;
}
