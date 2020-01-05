#ifndef _MESSAGE_FIELD_H
#define _MESSAGE_FIELD_H

#include <stdint.h>
#include <string>
#include <nlohmann/json.hpp>
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
} MessageFieldType;

class MessageField
{
    private:
        std::string         mName;
        std::string         mValue;
        MessageFieldType    mType;
        uint32_t            mSize;

        void                DecodeFieldType(json& object);
    public:
                            MessageField(json& object);
        virtual             ~MessageField();   
        bool                Output(FILE *outputFile, const char *name, Tag* tag);
};

#endif /* _MESSAGE_FIELD_H */