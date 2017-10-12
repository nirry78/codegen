#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wmmintrin.h> 

class Field
{
    private:
        char        *mStringValue;
    public:
                    Field(const char *value, uint32_t length);
                    ~Field();
};

class Record
{
    private:
        Field       **mFieldList;
        uint32_t    mNumberOfFields;

    public:
                    Record(uint32_t numberOfFields);
                    ~Record();
        
        void        AddField(uint32_t index, const char *value, uint32_t length);
};

class CodeGenerator
{
    private:
        FILE        *mInputFile;
        FILE        *mOutputFile;
        FILE        *mTemplateFile;
        uint8_t     mParseBuffer[4096];
        uint8_t     mParseOutput[1024];
        size_t      mParseOutputOffset;
        size_t      mParseFieldCount;
        size_t      mParseCharCount;
        size_t      mParseLineCount;
        bool        mParseTagOnly;
        uint8_t     mParseState;
        Record      *mCurrentRecord;
        uint32_t    mNumberOfFields;

        int         DumpOutputBuffer();
        int         ParseCsvBlock(const char delimiter, size_t length);
        int         ParseCsvInputFile();
        int         ParseTemplateBlock(size_t length);
        int         ParseTemplateInputFile();
        int         ProcessTag(const char *tag, uint32_t tag_length);
        void        Usage(const char *appName);
    public:
                    CodeGenerator();
        virtual     ~CodeGenerator();

        int         Run(int argc, char **argv);
};

#endif /* _CODE_GENERATOR_H */
