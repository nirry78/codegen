#include "Field.h"

static const uint8_t charCompressionTable[256] = {
    /* NUL SOH STX ETX EOT ENQ ACK BEL BS  TAB LF  VT  FF  CR  SO  SI  */
       0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* DLE DC1 DC2 DC3 DC4 NAM SYN ETB CAN EM  SUB ESC FS  GS  RS  US  */
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* SP  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   */
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  5,  1,  1,
    /* 0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?   */
       2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,
    /* @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   */
       1,  3,  3,  3,  3,  3,  3,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _   */
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   */
       1,  4,  4,  4,  4,  4,  4,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~   DEL */
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,

    /* Extended ASCII codes */
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
};

static const uint8_t uuidStateTranstionTable[38][6] = {
    /* NUL ANY NUM CHU CHL  - */
    {   0, 37,  1,  1,  1,  37  }, /*  0 */
    {   0, 37,  2,  2,  2,  37  }, /*  1 */
    {   0, 37,  3,  3,  3,  37  }, /*  2 */
    {   0, 37,  4,  4,  4,  37  }, /*  3 */
    {   0, 37,  5,  5,  5,  37  }, /*  4 */
    {   0, 37,  6,  6,  6,  37  }, /*  5 */
    {   0, 37,  7,  7,  7,  37  }, /*  6 */
    {   0, 37,  8,  8,  8,  37  }, /*  7 */
    {   0, 37, 37, 37, 37,   9  }, /*  8 */
    {   0, 37, 10, 10, 10,  37  }, /*  9 */
    {   0, 37, 11, 11, 11,  37  }, /* 10 */
    {   0, 37, 12, 12, 12,  37  }, /* 11 */
    {   0, 37, 13, 13, 13,  37  }, /* 12 */
    {   0, 37, 37, 37, 37,  14  }, /* 13 */
    {   0, 37, 15, 15, 15,  37  }, /* 14 */
    {   0, 37, 16, 16, 16,  37  }, /* 15 */
    {   0, 37, 17, 17, 17,  37  }, /* 16 */
    {   0, 37, 18, 18, 18,  37  }, /* 17 */
    {   0, 37, 37, 37, 37,  19  }, /* 18 */
    {   0, 37, 20, 20, 20,  37  }, /* 19 */
    {   0, 37, 21, 21, 21,  37  }, /* 20 */
    {   0, 37, 22, 22, 22,  37  }, /* 21 */
    {   0, 37, 23, 23, 23,  37  }, /* 22 */
    {   0, 37, 37, 37, 37,  24  }, /* 23 */
    {   0, 37, 25, 25, 25,  37  }, /* 24 */
    {   0, 37, 26, 26, 26,  37  }, /* 25 */
    {   0, 37, 27, 27, 27,  37  }, /* 26 */
    {   0, 37, 28, 28, 28,  37  }, /* 27 */
    {   0, 37, 29, 29, 29,  37  }, /* 28 */
    {   0, 37, 30, 30, 30,  37  }, /* 29 */
    {   0, 37, 31, 31, 31,  37  }, /* 30 */
    {   0, 37, 32, 32, 32,  37  }, /* 31 */
    {   0, 37, 33, 33, 33,  37  }, /* 32 */
    {   0, 37, 34, 34, 34,  37  }, /* 33 */
    {   0, 37, 35, 35, 35,  37  }, /* 34 */
    {   0, 37, 36, 36, 36,  37  }, /* 35 */
    {   0, 36, 36, 36, 36,  36  }, /* 36 */
    {  37, 37, 37, 37, 37,  37  }, /* 37 */
};

Field::Field(json& object):
    mLength(0), mType(MFT_UNKNOWN), mSize(0) 
{
    mMinValue = { .mType = MFT_UNKNOWN };
    mMaxValue = { .mType = MFT_UNKNOWN };
    mDefaultValue = { .mType = MFT_UNKNOWN };

    for (auto& [key, value] : object.items()) 
    {
        if (!key.compare("name"))
        {
            if (value.is_string())
            {
                mName = value.get<std::string>();            

                LOGD("<Field::Field> Name: %s\n", mName.c_str());
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
                std::string str(value.get<std::string>());

                if (!ConvertToUuid(str, &mDefaultValue))
                {
                    mDefaultValue.mType = MFT_STRING;
                    mDefaultValue.d.string = new std::string();

                    LOGD("<Field::Field> Value: %s\n", mDefaultValue.d.string->c_str());
                }
            }
            else if (value.is_number_unsigned())
            {
                mDefaultValue.mType = MFT_UINT64;
                mDefaultValue.d.u64 = value.get<uint64_t>();

                LOGD("<Field::Field> Value: %llu\n", mDefaultValue.d.u64);
            }
            else if (value.is_number_integer())
            {
                mDefaultValue.mType = MFT_INT64;
                mDefaultValue.d.i64 = value.get<int64_t>();

                LOGD("<Field::Field> Value: %lld\n", mDefaultValue.d.i64);
            }
            else if (value.is_number_float())
            {
                mDefaultValue.mType = MFT_FLOAT;
                mDefaultValue.d.f32 = value.get<float>();

                LOGD("<Field::Field> Value: %f\n", mDefaultValue.d.f32);
            }
            else if (value.is_array())
            {

            }
        }
        else if (!key.compare("length"))
        {
            if (value.is_number())
            {
                mLength = value.get<uint32_t>();

                LOGD("<Field::Field> Length: %u\n", mLength);
            }
            else
            {
                LOGD("<Field::Field> Length found with unexpected type\n");
            }            
        }
        else if (!key.compare("groups"))
        {
            if (value.is_string())
            {
                mGroups = value.get<std::string>();            

                LOGD("<Field::Field> Groups: %s\n", mGroups.c_str());
            }
            else
            {
                LOGE("Expecting string\n");
            }            
        }
        else
        {
            LOGD("<Field::Field> Unknown entry: %s\n", key.c_str());
        }
        
    }
}

Field::Field(const Field &field):
    mLength(field.mLength), mType(field.mType), mSize(field.mSize)
{
    mName = field.mName;
    mGroups = field.mGroups;
    
    CopyVariant(&mValue, &field.mValue);
    CopyVariant(&mMinValue, &field.mMinValue);
    CopyVariant(&mMaxValue, &field.mMaxValue);
    CopyVariant(&mDefaultValue, &field.mDefaultValue);
}

Field::~Field()
{
    if (mDefaultValue.mType == MFT_STRING)
    {
        delete mDefaultValue.d.string;
    }
}

bool Field::AcceptNameAndGroup(Tag *tag)
{
    return tag->AcceptNameAndGroup(mName, mGroups);
}

bool Field::ConvertToUuid(std::string &str, DataTypeVariant *dest)
{
    bool result = false;
    uint8_t buffer[16];
    size_t buffer_offset = 0;
    uint8_t state = 0;
    const char *c = str.c_str();

    for (size_t index = 0; index < str.length(); index++)
    {
        const uint8_t cc = charCompressionTable[(uint32_t)c[index]];
        uint8_t v = 0xff;
        state = uuidStateTranstionTable[state][cc];

        if (state <= 36)
        {
            if (cc == 2)
            {
                v = c[index] - '0';
            }
            else if (cc == 3)
            {
                v = c[index] - 'A' + 10;
            }
            else if (cc == 4)
            {
                v = c[index] - 'a' + 10;
            }
            
            if (v != 0xff)
            {
                uint32_t offset = buffer_offset >> 1;

                if (buffer_offset & 1)
                {
                    buffer[offset] = (buffer[offset] << 4) | v;
                }
                else
                {
                    buffer[offset] = v;
                }
                
                buffer_offset++;
            }

            if (state == 36)
            {
                break;
            }
        }
        else
        {
            break;
        }        
    }

    if (state == 36)
    {
        dest->mType = MFT_UUID;
        memcpy(&dest->d.uuid, buffer, sizeof(buffer));
        result = true;
    }

    return result;
}

void Field::CopyVariant(DataTypeVariant *dest, const DataTypeVariant *src)
{
    if (src->mType == MFT_STRING)
    {
        dest->mType = MFT_STRING;
        dest->d.string = new std::string(*src->d.string);
    }
    else
    {
        *dest = *src;
    }    
}

void Field::DecodeFieldType(json& object)
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

bool Field::Output(Document* document, std::string& name, Tag* tag, uint32_t count)
{
    if (StringCompare(name, "name"))
    {
        LOGD("<Field::Output> Name\n");

        tag->Output(document, mName);
    }
    else if (StringCompare(name, "type"))
    {
        LOGD("<Field::Output> Type\n");

        switch (mType)
        {
            case MFT_UINT8:
            {
                tag->Output(document, "uint8_t");
                break;
            }
            case MFT_UINT16:
            {
                tag->Output(document, "uint16_t");
                break;
            }
            case MFT_UINT32:
            {
                tag->Output(document, "uint32_t");
                break;
            }
            case MFT_UINT64:
            {
                tag->Output(document, "uint64_t");
                break;
            }
            case MFT_INT8:
            {
                tag->Output(document, "int8_t");
                break;
            }
            case MFT_INT16:
            {
                tag->Output(document, "int16_t");
                break;
            }
            case MFT_INT32:
            {
                tag->Output(document, "int32_t");
                break;
            }
            case MFT_INT64:
            {
                tag->Output(document, "int64_t");
                break;
            }
            case MFT_UUID:
            {
                tag->Output(document, "uuid_t");
                break;
            }
            default:
            {
                break;
            }
        }
    }
    else if (StringCompare(name, "length"))
    {
        LOGD("<Field::Output> Length: %u\n", mLength);

        if (mLength > 0)
        {
            tag->Output(document, "%u", mLength);
        }
    }
    else if (StringCompare(name, "count"))
    {
        LOGD("<Field::Output> Count: %u\n", count);

        tag->Output(document, "%u", count);
    }
    else if (StringCompare(name, "value"))
    {
        OutputVariant(tag, document, &mDefaultValue);
    }
    else
    {
        LOGD("<Field::Output> Unsupported field: %s\n", name.c_str());
    }
    

    return true;
}

void Field::OutputVariant(Tag* tag, Document* document, const DataTypeVariant *value)
{
    switch (value->mType)
    {
        case MFT_STRING:
        {
            tag->Output(document, "%s", value->d.string->c_str());
            break;
        }
        case MFT_UINT64:
        {
            tag->Output(document, "%llu", value->d.u64);
            break;
        }
        case MFT_INT64:
        {
            tag->Output(document, "%lld", value->d.i64);
            break;
        }
        case MFT_FLOAT:
        {
            tag->Output(document, "%f", value->d.f32);
            break;
        }
        case MFT_UUID:
        {
            tag->Output(document, "0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x", 
                        value->d.uuid[0], value->d.uuid[1], value->d.uuid[2], value->d.uuid[3], 
                        value->d.uuid[4], value->d.uuid[5], value->d.uuid[6], value->d.uuid[7], 
                        value->d.uuid[8], value->d.uuid[9], value->d.uuid[10], value->d.uuid[11], 
                        value->d.uuid[12], value->d.uuid[13], value->d.uuid[14], value->d.uuid[15]);
            break;
        }
        default:
        {
            break;
        }
    }
}
