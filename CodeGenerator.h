#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataType.h"
#include "JsonReader.h"

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
    TAG_FIELD_TYPE_ALIGNMENT,
    TAG_FIELD_TYPE_STYLE,
    TAG_FIELD_TYPE_CONVERT
} TagFieldType;

typedef enum {
    TAG_STYLE_STANDARD,
    TAG_STYLE_UPPERCASE,
    TAG_STYLE_LOWERCASE,
} TagStyle;

typedef enum {
    TAG_CONVERT_NONE,
    TAG_CONVERT_C,
} TagConvert;

class Field
{
    private:
        char            *mStringValue;
    public:
                        Field(const char *value, size_t length);
                        ~Field();
        char*           GetValue() { return mStringValue; }
};

class Record
{
    private:
        class Record    *mNextRecord;
        Field           **mFieldList;
        uint32_t        mNumberOfFields;

    public:
                        Record(uint32_t numberOfFields);
                        ~Record();

        void            AddField(size_t index, const char *value, size_t length);
        Record*         GetNextRecord() { return mNextRecord; }
        int             Output(FILE *output, size_t index, TagStyle style, TagConvert convert);
        void            SetNextRecord(Record *record) { mNextRecord = record; }
};

class Tag
{
    private:
        class Tag       *mNextTag;
        uint8_t         *mBuffer;
        size_t          mBufferLength;
        TagType         mTagType;
        TagStyle        mTagStyle;
        TagConvert      mTagConvert;
        uint8_t         *mName;
        size_t          mNameLength;
    public:
                        Tag(TagType tag_type, const uint8_t *buffer, size_t length);
                        ~Tag();
        void            Dump(FILE *output);
        uint8_t*        GetName() { return mName; }
        Tag*            GetNextTag() { return mNextTag; }
        TagType         GetTagType() { return mTagType; }
        bool            IsNameEqual(const char *reference);
        bool            IsValid();
        int             Output(FILE *output);
        int             OutputField(FILE *output, Record *record, uint32_t index);
        int             SetFieldValue(TagFieldType fieldType, const uint8_t *buffer, size_t length);
        void            SetNextTag(Tag *tag) { mNextTag = tag;}
};

class CodeGenerator
{
    private:
        FILE            *mInputFile;
        FILE            *mOutputFile;
        FILE            *mTemplateFile;
        FILE            *mLogFile;
        FILE            *mLogDest;
        uint8_t         mParseBuffer[4096];
        uint8_t         mParseOutput[1024];
        size_t          mParseOutputOffset;
        size_t          mParseFieldCount;
        size_t          mParseCharCount;
        size_t          mParseLineCount;
        bool            mParseTagOnly;
        uint8_t         mParseState;
        Record          *mCurrentRecord;
        Record          *mRecordListHead;
        Record          *mRecordListTail;
        char**          mRecordNames;
        uint32_t        mNumberOfFields;
        Tag             *mTagListHead;
        Tag             *mTagListTail;
        TagFieldType    mNextFieldType;
        uint32_t        mRootOffset;
        DataType        *mRootDataType;
        DataType        *mContainerDataType;
        DataType        *mFieldDataType;
        DataType        *mNamespaceDataType;

        int             AddTag();
        int             FieldName(const uint8_t *tag, size_t tag_length);
        int             FieldValue(const uint8_t *tag, size_t tag_length);
        int             GenerateOutput();
        int             ParseCsvBlock(const char delimiter, size_t length);
        int             ParseCsvInputFile();
        int             ParseYamlInputFile();
        int             ParseJsonInputFile(const char *filename);
        int             ParseTemplateBlock(size_t length);
        int             ParseTemplateInputFile();
        int             ProcessTag(const uint8_t *tag, size_t tag_length);
        void            Usage(const char *appName);
        int             VerifyData();
    public:
                        CodeGenerator();
        virtual         ~CodeGenerator();

        int             Run(int argc, char **argv);
};

#endif /* _CODE_GENERATOR_H */
