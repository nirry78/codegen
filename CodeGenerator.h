#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tag.h"
//#include "DataType.h"
#include "JsonReader.h"

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
        uint8_t         mParseState;
        size_t          mParseOutputOffset;
        size_t          mParseFieldCount;
        size_t          mParseCharCount;
        size_t          mParseLineCount;
        bool            mParseTagOnly;
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

        JsonReader      mJsonReader;


        int             AddTag();
        int             FieldName(const uint8_t *tag, size_t tag_length);
        int             FieldValue(const uint8_t *tag, size_t tag_length);
        int             GenerateOutput();
        int             GenerateOutputJson();
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
