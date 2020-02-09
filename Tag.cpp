#include "Tag.h"

Tag::Tag(TagType tag_type, const uint8_t *buffer, size_t length):
    mNextTag(NULL),
    mBufferLength(length),
    mTagType(tag_type),
    mTagStyle(TAG_STYLE_STANDARD),
    mTagConvert(TAG_CONVERT_NONE),
    mWidth(0),
    mAlignment(0)
{
    mBuffer = new uint8_t[length];
    if (mBuffer != NULL)
    {
        memcpy(mBuffer, buffer, length);
    }
}

Tag::~Tag()
{
    if (mBuffer != NULL)
    {
        delete mBuffer;
    }
}

bool Tag::AcceptNameAndGroup(std::string& name, std::string& groups)
{
    bool result = true;

    if (mName.length() > 0)
    {
        result = StringCompare(name, mName);
    }

    if (mGroup.length() > 0)
    {
        if (groups.length() > 0)
        {
            size_t pos = 0;
            size_t next_pos;

            do
            {
                next_pos = groups.find_first_of(',', pos);
                if (next_pos == std::string::npos)
                {
                    std::string group = groups.substr(pos, groups.length());
                    if (StringCompare(group, mGroup))
                    {
                        result = true;
                    }
                    break;
                }
                else
                {
                    std::string group = groups.substr(next_pos, groups.length());
                    if (StringCompare(group, mGroup))
                    {
                        result = true;
                        break;
                    }
                }
                pos = next_pos;
            } while (true);            
        }
        else
        {
            result = false;
        }        
    }

    LOGD("<Tag::AcceptNameAndGroup> Name: [%s eq %s], Group: [%s in %s] - %s\n", 
         name.c_str(), mName.c_str(), mGroup.c_str(), groups.c_str(), result ? "accepted" : "rejected");

    return result;
}

void Tag::Dump(FILE *output)
{
    switch (mTagType)
    {
        case TAG_TYPE_DATA:
            LOGD("<Tag::Dump> DATA [len:%u]\n", mBufferLength);
            break;
        case TAG_TYPE_CONTAINER:
            LOGD("<Tag::Dump> CONTAINER\n");
            break;
        case TAG_TYPE_FOREACH_FIELD_BEGIN:
            LOGD("<Tag::Dump> FOREACH_FIELD_BEGIN\n");
            break;
        case TAG_TYPE_FOREACH_FIELD_END:
            LOGD("<Tag::Dump> FOREACH_FIELD_END\n");
            break;
        case TAG_TYPE_FOREACH_NAMESPACE_BEGIN:
            LOGD("<Tag::Dump> FOREACH_NAMESPACE_BEGIN\n");
            break;
        case TAG_TYPE_FOREACH_NAMESPACE_END:
            LOGD("<Tag::Dump> FOREACH_NAMESPACE_END\n");
            break;
        case TAG_TYPE_FOREACH_CONTAINER_BEGIN:
            LOGD("<Tag::Dump> FOREACH_CONTAINER_BEGIN\n");
            break;
        case TAG_TYPE_FOREACH_CONTAINER_END:
            LOGD("<Tag::Dump> FOREACH_CONTAINER_END\n");
            break;
        case TAG_TYPE_FIELD:
            LOGD("<Tag::Dump> FIELD [Name:%s, Style: %s]\n", 
                 mName.size() > 0 ? mName.c_str() : "N/A", 
                 mTagStyle == TAG_STYLE_UPPER_CASE ? "uppercase" : 
                 mTagStyle == TAG_STYLE_LOWER_CASE ? "lowercase" : 
                 "default");
            break;
        case TAG_TYPE_FIELD_COUNT:
            LOGD("<Tag::Dump> FIELD_COUNT\n");
            break;
        default:
            LOGD("TAG[%u]\n", mTagType);
            break;
    }
}

bool Tag::IsNameEqual(const char *reference)
{
    return !mName.compare(reference);
}

bool Tag::IsValid()
{
    bool result = true;

    switch (mTagType)
    {
        case TAG_TYPE_FIELD:
            if (mName.size() == 0)
            {
                result = false;
            }
            break;
        default:
        {
            break;
        }
    }

    return result;
}

void Tag::Output(Document *document)
{
    document->OutputBuffer((const char*)mBuffer, mBufferLength);
}

void Tag::Output(Document *document, const char* format, ...)
{
    va_list va;
    int length, offset = 0;
    char buffer[256];

    buffer[255] = 0;

    if (mTagStyle == TAG_STYLE_ARRAY)
    {
        buffer[offset++] = '[';
    }

    va_start(va, format);
    length = vsnprintf(buffer, sizeof(buffer) - 1 - offset, format, va);
    va_end(va);

    offset += length;

    if (mTagStyle == TAG_STYLE_ARRAY && length < (int)(sizeof(buffer) - 1))
    {
        buffer[offset++] = ']';
    }

    if (offset)
    {
        std::string str(buffer, offset);

        Output(document, str);
    }
}

void Tag::Output(Document *document, std::string& str)
{
    if (mAlignment)
    {
        document->Fill(' ', mAlignment);
    }

    switch (mTagStyle)
    {
        case TAG_STYLE_CAMEL_CASE:
        {
            document->OutputCamelCase(str, mWidth);
            break;
        }
        case TAG_STYLE_LOWER_CAMEL_CASE:
        {
            document->OutputLowerCamelCase(str, mWidth);
            break;
        }
        case TAG_STYLE_LOWER_CASE:
        {
            document->OutputLowerCase(str, mWidth);
            break;
        }
        case TAG_STYLE_UPPER_CASE:
        {
            document->OutputUpperCase(str, mWidth);
            break;
        }
        default:
        {
            document->Output(str, mWidth);
            break;
        }
    }
}

int Tag::SetFieldValue(TagFieldType fieldType, const uint8_t *buffer, size_t length)
{
    std::string inputString((const char*)buffer, length);
    int result = 0;

    switch (fieldType)
    {
        case TAG_FIELD_TYPE_NAME:
        {
            mName.assign((const char*)buffer, length);
            LOGD("<Tag::SetFieldValue> Name: %s\n", mName.c_str());
            break;
        }
        case TAG_FIELD_TYPE_STYLE:
        {
            if (StringCompare(inputString, "upper_case"))
            {
                mTagStyle = TAG_STYLE_UPPER_CASE;

                LOGD("<Tag::SetFieldValue> Style: Upper case\n");
            }
            else if (StringCompare(inputString, "lower_case"))
            {
                mTagStyle = TAG_STYLE_LOWER_CASE;

                LOGD("<Tag::SetFieldValue> Style: Lower case\n");
            }
            else if (StringCompare(inputString, "camel_case"))
            {
                mTagStyle = TAG_STYLE_CAMEL_CASE;

                LOGD("<Tag::SetFieldValue> Style: Camel case\n");
            }
            else if (StringCompare(inputString, "lower_camel_case"))
            {
                mTagStyle = TAG_STYLE_LOWER_CAMEL_CASE;

                LOGD("<Tag::SetFieldValue> Style: Lowercase\n");
            }
            else
            {
                LOGD("<Tag::SetFieldValue> Style: Default (%s)\n", inputString.c_str());
            }   
            break;
        }
        case TAG_FIELD_TYPE_WIDTH:
        {
            mWidth = strtol(inputString.c_str(), NULL, 0);
            LOGD("<Tag::SetFieldValue> Width: %u\n", mWidth);
            break;
        }
        case TAG_FIELD_TYPE_ALIGNMENT:
        {
            mAlignment = strtol(inputString.c_str(), NULL, 0);
            LOGD("<Tag::SetFieldValue> Width: %u\n", mWidth);
            break;
        }
        case TAG_FIELD_TYPE_GROUP:
        {
            mGroup.assign((const char*)buffer, length);
            LOGD("<Tag::SetFieldValue> Group: %s\n", mGroup.c_str());            
            break;
        }
        default:
        {
            LOGD("<Tag::SetFieldValue> Field not implemented (%s)\n", inputString.c_str());
            break;
        }
    }

    return result;
}