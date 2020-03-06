#ifndef _FIELD_H
#define _FIELD_H

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>
#include "Formatter.h"
#include "Logging.h"
#include "Tag.h"

using json = nlohmann::json;

typedef enum {
    MFT_UNKNOWN,
    MFT_UINT8,
    MFT_UINT16,
    MFT_UINT32,
    MFT_UINT64,
    MFT_INT8,
    MFT_INT16,
    MFT_INT32,
    MFT_INT64,
    MFT_FLOAT,
    MFT_UUID,
    MFT_STRING,
} DataType;

typedef struct   
    {
    DataType        mType;
    union
    {
        uint8_t     u8;
        uint16_t    u16;
        uint32_t    u32;
        uint64_t    u64;
        int8_t      i8;
        int16_t     i16;
        int32_t     i32;
        int64_t     i64;
        float       f32;
        std::string *string;
        uint8_t     uuid[16];
    } d;    
} DataTypeVariant;

class Field: public Formatter
{
    private:
        std::string         mName;
        std::string         mGroups;
        uint32_t            mLength;
        DataTypeVariant     mValue;
        DataTypeVariant     mMinValue;
        DataTypeVariant     mMaxValue;
        DataTypeVariant     mDefaultValue;
        DataType            mType;
        uint32_t            mSize;
        
        bool                ConvertToUuid(std::string &str, DataTypeVariant *dest);
        void                CopyVariant(DataTypeVariant *dest, const DataTypeVariant *src);
        void                DecodeFieldType(json& object);
        void                OutputVariant(Tag* tag, Document* document, const DataTypeVariant *value);
    public:
                            Field(json& object);
                            Field(const Field& field);
        virtual             ~Field();   
        bool                AcceptNameAndGroup(Tag *tag);
        bool                Output(Document* document, std::string& name, Tag* tag, uint32_t count);
};

#endif /* _FIELD_H */