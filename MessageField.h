#ifndef _MESSAGE_FIELD_H
#define _MESSAGE_FIELD_H

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
    MFT_UUID,
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
    } d;    
} DataTypeVariant;

class MessageField: public Formatter
{
    private:
        std::string         mName;
        std::string         mValueStr;
        uint32_t            mLength;
        DataTypeVariant     mValue;
        DataTypeVariant     mMinValue;
        DataTypeVariant     mMaxValue;
        DataTypeVariant     mDefaultValue;
        DataType            mType;
        uint32_t            mSize;
        

        void                DecodeFieldType(json& object);
    public:
                            MessageField(json& object);
        virtual             ~MessageField();   
        bool                Output(FILE *outputFile, const char *name, Tag* tag);
};

#endif /* _MESSAGE_FIELD_H */