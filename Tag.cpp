#include "Tag.h"

Tag::Tag(TagType tag_type, const uint8_t *buffer, size_t length):
    mNextTag(NULL),
    mBufferLength(length),
    mTagType(tag_type),
    mTagStyle(TAG_STYLE_STANDARD),
    mTagConvert(TAG_CONVERT_NONE),
    mWidth(0)
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

void Tag::Dump(FILE *output)
{
    switch (mTagType)
    {
        case TAG_TYPE_DATA:
            LOGD("<tag::dump> DATA [len:%u]\n", mBufferLength);
            break;
        case TAG_TYPE_CONTAINER:
            LOGD("<tag::dump> CONTAINER\n");
            break;
        case TAG_TYPE_FOREACH_FIELD_BEGIN:
            LOGD("<tag::dump> FOREACH_FIELD_BEGIN\n");
            break;
        case TAG_TYPE_FOREACH_FIELD_END:
            LOGD("<tag::dump> FOREACH_FIELD_END\n");
            break;
        case TAG_TYPE_FOREACH_NAMESPACE_BEGIN:
            LOGD("<tag::dump> FOREACH_NAMESPACE_BEGIN\n");
            break;
        case TAG_TYPE_FOREACH_NAMESPACE_END:
            LOGD("<tag::dump> FOREACH_NAMESPACE_END\n");
            break;
        case TAG_TYPE_FOREACH_CONTAINER_BEGIN:
            LOGD("<tag::dump> FOREACH_CONTAINER_BEGIN\n");
            break;
        case TAG_TYPE_FOREACH_CONTAINER_END:
            LOGD("<tag::dump> FOREACH_CONTAINER_END\n");
            break;
        case TAG_TYPE_FIELD:
            LOGD("<tag::dump> FIELD [Name:%s, Style: %s]\n", 
                 mName.size() > 0 ? mName.c_str() : "N/A", 
                 mTagStyle == TAG_STYLE_UPPER_CASE ? "uppercase" : 
                 mTagStyle == TAG_STYLE_LOWER_CASE ? "lowercase" : 
                 "default");
            break;
        case TAG_TYPE_FIELD_COUNT:
            LOGD("<tag::dump> FIELD_COUNT\n");
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

int Tag::Output(FILE *output)
{
    return fwrite(mBuffer, 1, mBufferLength, output) == mBufferLength ? 0 : -1;
}

int Tag::Output(FILE *output, std::string& str)
{
    int count;

    switch (mTagStyle)
    {
        case TAG_STYLE_UPPER_CASE:
        {
            count = OutputUpperCase(output, str);
            break;
        }
        case TAG_STYLE_LOWER_CASE:
        {
            count = OutputLowerCase(output, str);
            break;
        }
        default:
        {
            count = fprintf(output, "%s", str.c_str());
            break;
        }
    }

    for (; count < mWidth; count++)
    {
        fputc(' ', output);
    }

    return 0;
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
        default:
        {
            LOGD("<Tag::SetFieldValue> Field not implemented (%s)\n", inputString.c_str());
            break;
        }
    }

    return result;
}