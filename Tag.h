#ifndef _TAG_H
#define _TAG_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "Object.h"
#include "Formatter.h"
#include "Logging.h"
#include "Platform.h"

typedef enum {
    TAG_TYPE_DATA,
    TAG_TYPE_CONTAINER,
    TAG_TYPE_NAMESPACE,
    TAG_TYPE_FOREACH_NAMESPACE_BEGIN,
    TAG_TYPE_FOREACH_NAMESPACE_END,
    TAG_TYPE_FOREACH_CONTAINER_BEGIN,
    TAG_TYPE_FOREACH_CONTAINER_END,
    TAG_TYPE_FOREACH_FIELD_BEGIN,
    TAG_TYPE_FOREACH_FIELD_END,
    TAG_TYPE_FIELD,
    TAG_TYPE_FIELD_COUNT,
    TAG_TYPE_SEPARATOR,
} TagType;

typedef enum {
    TAG_FIELD_TYPE_NONE,
    TAG_FIELD_TYPE_NAME,
    TAG_FIELD_TYPE_WIDTH,
    TAG_FIELD_TYPE_STYLE,
    TAG_FIELD_TYPE_CONVERT,
    TAG_FIELD_TYPE_LENGTH
} TagFieldType;

typedef enum {
    TAG_STYLE_STANDARD,
    TAG_STYLE_UPPER_CASE,
    TAG_STYLE_LOWER_CASE,
    TAG_STYLE_CAMEL_CASE,
    TAG_STYLE_LOWER_CAMEL_CASE,
} TagStyle;

typedef enum {
    TAG_CONVERT_NONE,
    TAG_CONVERT_C,
} TagConvert;

class Tag: public Formatter
{
    private:
        class Tag       *mNextTag;
        uint8_t         *mBuffer;
        size_t          mBufferLength;
        TagType         mTagType;
        TagStyle        mTagStyle;
        TagConvert      mTagConvert;
        uint32_t        mWidth;
        std::string     mName;
        std::string     mLength;
    public:
                        Tag(TagType tag_type, const uint8_t *buffer, size_t length);
                        ~Tag();
        void            Dump(FILE *output);
        std::string&    GetName() { return mName; }
        Tag*            GetNextTag() { return mNextTag; }
        TagType         GetTagType() { return mTagType; }
        TagStyle        GetTagStyle() { return mTagStyle; }
        bool            IsNameEqual(const char *reference);
        bool            IsValid();
        int             Output(FILE *output);
        int             Output(FILE *output, std::string& str);
        //int             OutputField(FILE *output, Record *record, uint32_t index);
        int             SetFieldValue(TagFieldType fieldType, const uint8_t *buffer, size_t length);
        void            SetNextTag(Tag *tag) { mNextTag = tag;}
};


#endif /* _TAG_H */