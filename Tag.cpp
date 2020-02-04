#include "Tag.h"

Tag::Tag(TagType tag_type, const uint8_t *buffer, size_t length):
    mNextTag(NULL),
    mBufferLength(length),
    mTagType(tag_type),
    mTagStyle(TAG_STYLE_STANDARD),
    mTagConvert(TAG_CONVERT_NONE)
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
            LOGD("TAG[DATA:%u]\n", mBufferLength);
            break;
        case TAG_TYPE_FOREACH_FIELD_BEGIN:
            LOGD("TAG[FOREACH_FIELD_BEGIN]\n");
            break;
        case TAG_TYPE_FOREACH_FIELD_END:
            LOGD("TAG[FOREACH_FIELD_END]\n");
            break;
        case TAG_TYPE_FOREACH_NAMESPACE_BEGIN:
            LOGD("TAG[FOREACH_NAMESPACE_BEGIN]\n");
            break;
        case TAG_TYPE_FOREACH_NAMESPACE_END:
            LOGD("TAG[FOREACH_NAMESPACE_END]\n");
            break;
        case TAG_TYPE_FOREACH_CONTAINER_BEGIN:
            LOGD("TAG[FOREACH_CONTAINER_BEGIN]\n");
            break;
        case TAG_TYPE_FOREACH_CONTAINER_END:
            LOGD("TAG[FOREACH_CONTAINER_END]\n");
            break;
        case TAG_TYPE_FIELD:
            LOGD("TAG[FIELD] Name:%s, Style: %s\n", 
                 mName.size() > 0 ? mName.c_str() : "N/A", 
                 mTagStyle == TAG_STYLE_UPPER_CASE ? "uppercase" : 
                 mTagStyle == TAG_STYLE_LOWER_CASE ? "lowercase" : 
                 "default");
            break;
        case TAG_TYPE_FIELD_COUNT:
            LOGD("TAG[FIELD_COUNT]\n");
            break;
        default:
            LOGD("TAG[UNKNOWN]\n");
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

int Tag::SetFieldValue(TagFieldType fieldType, const uint8_t *buffer, size_t length)
{
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
            if (!_strnicmp("upper", (char*)buffer, length) || !_strnicmp("uppercase", (char*)buffer, length))
            {
                mTagStyle = TAG_STYLE_UPPER_CASE;

                LOGD("<Tag::SetFieldValue> Style: Uppercase\n");
            }
            else if (!_strnicmp("lower", (char*)buffer, length) || !_strnicmp("lowercase", (char*)buffer, length))
            {
                mTagStyle = TAG_STYLE_LOWER_CASE;

                LOGD("<Tag::SetFieldValue> Style: Lowercase\n");
            }
            else
            {
                LOGD("<Tag::SetFieldValue> Style: Default\n");
            }   
        }
        case TAG_FIELD_TYPE_CONVERT:
        {
            if (!_strnicmp("c-style", (char*)buffer, length))
            {
                mTagConvert = TAG_CONVERT_C;
            }
        }
        default:
        {

        }
    }

    return result;
}