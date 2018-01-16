#include "CodeGenerator.h"

static const uint8_t charCompressionTable[256] = {
    /* NUL SOH STX ETX EOT ENQ ACK BEL BS  TAB LF  VT  FF  CR  SO  SI  */
       0,  1,  1,  1,  1,  1,  1,  1,  1,  2,  3,  1,  1,  4,  1,  1,
    /* DLE DC1 DC2 DC3 DC4 NAM SYN ETB CAN EM  SUB ESC FS  GS  RS  US  */
       1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
    /* SP  !   "   #   $   %   &   '   (   )   *   +   ,   -   .   /   */
       5,  1,  6,  1,  1,  1,  1,  7,  1,  1,  1,  1,  8,  1,  1,  1,
    /* 0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ?   */
       9,  9,  9,  9,  9,  9,  9,  9,  9,  9, 10, 11,  1, 12,  1,  1,
    /* @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O   */
      13, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    /* P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _   */
      14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,  1,  1,  1,  1, 15,
    /* `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o   */
       1, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
    /* p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~   DEL */
      14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 16, 17, 18,  1,  1,

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

static const uint8_t cvsStateTranstionTable[12][19] = {
   /* NUL ANY TAB  LF  CR  SP   "   '   ,  NUM   :   ;   =   @ ALPHA   _   {   |   } */
    {   0,  8,  7,  0,  0,  0,  1,  4,  7,   8,  7,  7,  8,  8,    8,  8,  8,  7,  8 }, /* 0 - Default state */
    {   0,  2,  2,  2,  2,  2,  3,  2,  2,   2,  2,  2,  2,  2,    2,  2,  2,  2,  2 }, /* 1 - Start " quote section */
    {   0,  2,  2,  2,  2,  2,  3,  2,  2,   2,  2,  2,  2,  2,    2,  2,  2,  2,  2 }, /* 2 - Inside " quote section */
    {   0, 11,  7,  9,  9,  7,  1,  4,  7,  11,  7,  7, 11, 11,   11, 11, 11,  7, 11 }, /* 3 - End " quote section */
    {   0,  5,  5,  5,  5,  5,  6,  5,  5,   5,  5,  5,  5,  5,    5,  5,  5,  5,  5 }, /* 4 - Start ' quote section */
    {   0,  5,  5,  5,  5,  5,  6,  5,  5,   5,  5,  5,  5,  5,    5,  5,  5,  5,  5 }, /* 5 - Inside ' quote section */
    {   0, 11,  7,  9,  9,  7,  1,  4,  7,  11,  7,  7, 11, 11,   11, 11, 11,  7, 11 }, /* 6 - End ' quote section */
    {   0,  8,  7,  0,  0,  0,  1,  4,  7,   8,  7,  7,  8,  8,    8,  8,  8,  7,  8 }, /* 7 - Possible field end */
    {   0,  8,  7,  9,  9,  7, 11, 11,  7,   8,  7,  7,  8,  8,    8,  8,  8,  7,  8 }, /* 8 - Field value */
    {   0,  8,  7, 10, 10,  0,  1,  4,  7,   8,  7,  7,  8,  8,    8,  8,  8,  7,  8 }, /* 9 - End of line */
    {   0,  8,  7, 10, 10,  0,  1,  4,  7,   8,  7,  7,  8,  8,    8,  8,  8,  7,  8 }, /* 10 - End of line - again */
    {  11, 11, 11, 11, 11, 11, 11, 11, 11,  11, 11, 11, 11, 11,   11, 11, 11, 11, 11 }, /* 11 - Invalid char */
};


static const uint8_t templateStateTranstionTable[43][19] = {
   /* NUL ANY TAB  LF  CR  SP   "   '   ,  NUM   :   ;   =   @ ALPHA   _   {   |   } */
    {   0,  0,  0,  8,  9,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 0 - Default state */
    {   0,  0, 11, 11, 11, 11, 11, 11, 11,  11, 11, 11, 11,  2,    3, 11, 11, 11, 11 }, /* 1 - Field name start section */
    {   0,  0,  0,  8,  9,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 2 - Double @ section */
    {   0,  4,  4,  5,  6,  4,  4,  4,  4,   3,  4,  4,  4,  7,    3,  3, 11,  4,  4 }, /* 3 - Field name continue section */
    {   0,  0,  0,  8,  9,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 4 - Field complete no parameters */
    {   0,  0,  0,  8, 10,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 5 - Field complete LF */
    {   0,  0,  0, 10,  9,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 6 - Field complete CR */
    {   0,  0,  0,  2,  2,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 7 - Field complete @ */
    {   0,  0,  0,  8, 10,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 8 - LF */
    {   0,  0,  0, 10,  9,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 9 - CR */
    {   0,  0,  0,  8,  9,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 10 - CR>LF or LF>CR */
    {   0,  0, 12, 13, 14, 12,  0,  0,  0,   0,  0,  0,  0,  0,   16,  0,  0,  0,  0 }, /* 11 - Params begin */
    {   0,  0, 12, 13, 14, 12,  0,  0,  0,   0,  0,  0,  0,  0,   16,  0,  0,  0,  0 }, /* 12 - Params name spacing */
    {   0,  0, 12, 13, 15, 12,  0,  0,  0,   0,  0,  0,  0,  0,   16,  0,  0,  0,  0 }, /* 13 - Params name LF */
    {   0,  0, 12, 15, 14, 12,  0,  0,  0,   0,  0,  0,  0,  0,   16,  0,  0,  0,  0 }, /* 14 - Params name CR */
    {   0,  0, 12, 13, 14, 12,  0,  0,  0,   0,  0,  0,  0,  0,   16,  0,  0,  0,  0 }, /* 15 - Params name LF<CR or CR>LF */
    {   0,  0, 18, 19, 20, 18, 17, 17, 17,  16, 17, 17, 23, 17,   16, 16, 17, 17, 17 }, /* 16 - Params name continue section */
    {   0, 17, 17, 17, 17, 17, 17, 17, 17,  17, 17, 17, 17, 17,   17, 17, 17, 17, 17 }, /* 17 - Params name error */
    {   0,  0, 18, 19, 20, 18, 22, 22, 22,  22, 22, 22, 23, 22,   22, 22, 22, 22, 22 }, /* 18 - Params spacing 1 */
    {   0,  0, 18, 21, 19, 18, 22, 22, 22,  22, 22, 22, 23, 22,   22, 22, 22, 22, 22 }, /* 19 - Params spacing 1 LF */
    {   0,  0, 12, 15, 14, 12, 22, 22, 22,  22, 22, 22, 23, 22,   22, 22, 22, 22, 22 }, /* 20 - Params spacing 1 CR */
    {   0,  0, 12, 13, 14, 12, 22, 22, 22,  22, 22, 22, 23, 22,   22, 22, 22, 22, 22 }, /* 21 - Params spacing 1 LF<CR or CR>LF */
    {   0, 22, 22, 22, 22, 22, 22, 22, 22,  22, 22, 22, 22, 22,   22, 22, 22, 22, 22 }, /* 22 - Params spacing error */
    {   0, 35, 23, 24, 25, 23, 27, 31, 35,  35, 35, 35, 22, 35,   35, 35, 22, 35, 22 }, /* 23 - Params spacing 2 */
    {   0, 35, 23, 24, 26, 23, 27, 31, 35,  35, 35, 35, 22, 35,   35, 35, 22, 35, 22 }, /* 24 - Params spacing 2 LF */
    {   0, 35, 23, 26, 25, 23, 27, 31, 35,  35, 35, 35, 22, 35,   35, 35, 22, 35, 22 }, /* 25 - Params spacing 2 CR */
    {   0, 35, 23, 24, 25, 23, 27, 31, 35,  35, 35, 35, 22, 35,   35, 35, 22, 35, 22 }, /* 26 - Params spacing 2 LF<CR or CR>LF */
    {   0, 27, 27, 28, 29, 27, 36, 27, 27,  27, 27, 27, 27, 27,   27, 27, 27, 27, 27 }, /* 27 - Params value " section */
    {   0, 27, 27, 28, 30, 27, 36, 27, 27,  27, 27, 27, 27, 27,   27, 27, 27, 27, 27 }, /* 28 - Params " section LF */
    {   0, 27, 27, 30, 29, 27, 36, 27, 27,  27, 27, 27, 27, 27,   27, 27, 27, 27, 27 }, /* 29 - Params " section CR */
    {   0, 27, 27, 28, 29, 27, 36, 27, 27,  27, 27, 27, 27, 27,   27, 27, 27, 27, 27 }, /* 30 - Params " section LF<CR or CR>LF */
    {   0, 31, 31, 32, 33, 31, 31, 36, 31,  31, 31, 31, 31, 31,   31, 31, 31, 31, 31 }, /* 31 - Params value ' section */
    {   0, 31, 31, 32, 34, 31, 31, 36, 31,  31, 31, 31, 31, 31,   31, 31, 31, 31, 31 }, /* 32 - Params ' section LF */
    {   0, 31, 31, 34, 33, 31, 31, 36, 31,  31, 31, 31, 31, 31,   31, 31, 31, 31, 31 }, /* 33 - Params ' section CR */
    {   0, 31, 31, 32, 33, 31, 31, 36, 31,  31, 31, 31, 31, 31,   31, 31, 31, 31, 31 }, /* 34 - Params ' section LF<CR or CR>LF */
    {   0, 36, 36, 21, 19, 36, 22, 22, 36,  35, 22, 22, 22, 22,   35, 35, 22, 35, 22 }, /* 35 - Params value continue */
    {   0, 40, 37, 13, 14, 37, 40, 40, 12,  40, 40, 40, 40, 40,   16, 41, 41, 41, 42 }, /* 36 - Params value end */
    {   0, 40, 37, 38, 39, 31, 40, 40, 12,  40, 40, 40, 40, 40,   16, 41, 41, 41, 42 }, /* 37 - Params value end spacing */
    {   0, 40, 37, 38, 40, 37, 40, 40, 12,  40, 40, 40, 40, 40,   16, 41, 41, 41, 42 }, /* 38 - Params value end LF */
    {   0, 40, 37, 40, 39, 37, 40, 40, 12,  40, 40, 40, 40, 40,   16, 41, 41, 41, 42 }, /* 39 - Params value end CR */
    {   0, 40, 37, 38, 40, 37, 40, 40, 12,  40, 40, 40, 40, 40,   16, 41, 41, 41, 42 }, /* 40 - Params value end LF<CR or CR>LF */
    {   0, 41, 41, 41, 41, 41, 41, 41, 41,  41, 41, 41, 41, 41,   41, 41, 41, 41, 42 }, /* 41 - Params value error */
    {   0,  0,  0,  8,  9,  0,  0,  0,  0,   0,  0,  0,  0,  1,    0,  0,  0,  0,  0 }, /* 42 - Params end */
};

Field::Field(const char *value, uint32_t length)
{
    mStringValue = new char[length];
    if (mStringValue != NULL)
    {
        memcpy(mStringValue, value, length);
    }
}

Field::~Field()
{
    if (mStringValue != NULL)
    {
        delete mStringValue;
    }
}

Record::Record(uint32_t numberOfFields):
    mFieldList(NULL), mNumberOfFields(numberOfFields)
{

}

Record::~Record()
{
    if (mFieldList)
    {
        for (uint32_t index = 0; index < mNumberOfFields; index++)
        {
            if (mFieldList[index] != NULL)
            {
                delete mFieldList[index];
            }
        }

        delete mFieldList;
    }
}

void Record::AddField(uint32_t index, const char *value, uint32_t length)
{

}

Tag::Tag(const char *buffer, uint32_t length):
    mNextTag(NULL)
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

CodeGenerator::CodeGenerator():
    mInputFile(NULL), mOutputFile(NULL), mTemplateFile(NULL), mLogFile(NULL), mLogDest(stdout), mParseState(0),
    mParseOutputOffset(0), mParseFieldCount(0), mParseCharCount(0), mParseLineCount(0), mCurrentRecord(NULL),
    mNumberOfFields(10)
{

}

CodeGenerator::~CodeGenerator()
{
    if (mInputFile != NULL)
    {
        fclose(mInputFile);
    }

    if (mOutputFile != NULL)
    {
        fclose(mOutputFile);
    }

    if (mTemplateFile != NULL)
    {
        fclose(mTemplateFile);
    }

    if (mLogFile != NULL)
    {
        fclose(mLogFile);
    }
}

int CodeGenerator::DumpOutputBuffer()
{
    int result = 0;

    if (fwrite(mParseOutput, 1, mParseOutputOffset, mOutputFile) != mParseOutputOffset)
    {
        result = -1;
    }

    mParseOutputOffset = 0;

    return result;
}

int CodeGenerator::ParseCsvBlock(const char delimiter, size_t length)
{
    int result = 0;
    size_t index;

    for (index = 0; index < length; index++)
    {
        uint8_t mParseStatePrevious = mParseState;
        mParseState = cvsStateTranstionTable[mParseState][charCompressionTable[mParseBuffer[index]]];
        mParseCharCount++;
        switch (mParseState)
        {
            case 0: /* 0 - Default state */
                mParseOutputOffset = 0;
                break;
            case 1: /* 1 - Start " quote section */
            case 4: /* 4 - Start ' quote section */
                mParseOutputOffset = 0;
                break;
            case 2: /* 2 - Inside " quote section */
            case 5: /* 5 - Inside ' quote section */
            case 8: /* 8 - Field value */
                mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
                if (mParseOutputOffset >= (sizeof(mParseOutput) - 1))
                {
                    mParseState = 11;
                }
                break;
            case 3: /* 3 - End " quote section */
            case 6: /* 6 - End ' quote section */
                mParseOutput[mParseOutputOffset++] = 0;
                break;
            case 7: /* 7 - Possible field end */
                if (delimiter == mParseBuffer[index])
                {
                    mParseOutput[mParseOutputOffset++] = 0;

                    if (mCurrentRecord == NULL)
                    {
                        mCurrentRecord = new Record(mNumberOfFields);
                    }

                    if (mCurrentRecord != NULL)
                    {
                        mCurrentRecord->AddField(mParseFieldCount, (const char*)mParseOutput, mParseOutputOffset);
                    }

                    mParseOutputOffset = 0;
                    mParseFieldCount++;

                    //fprintf(mOutputFile, "Field[%u]: %s\n", mParseFieldCount, mParseOutput);
                }
                else
                {
                    mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
                    mParseState = 8;
                }
                break;
            case 9: /* 9 - End of line */
                mParseOutput[mParseOutputOffset++] = 0;
                if (mParseOutputOffset > 1)
                {
                    if (mCurrentRecord == NULL)
                    {
                        mCurrentRecord = new Record(mNumberOfFields);
                    }

                    if (mCurrentRecord != NULL)
                    {
                        mCurrentRecord->AddField(mParseFieldCount, (const char*)mParseOutput, mParseOutputOffset);
                    }
                }

                mCurrentRecord = NULL;
                mParseOutputOffset = 0;
                mParseCharCount = 0;
                mParseFieldCount = 0;
                mParseLineCount++;
                break;
            case 10: /* 10 - End of line - again */
                break;
            case 11: /* 11 - Invalid char */
                break;
        }

        /*fprintf(mOutputFile, "I:%c (0%02x) S:%u->%u\n",
            mParseBuffer[index] >= 0x20 ? mParseBuffer[index] : ' ',
            mParseBuffer[index], mParseStatePrevious, mParseState);*/
    }

    return result;
}

int CodeGenerator::ParseCsvInputFile()
{
    int result = 0;
    size_t ret, index, total = 0;
    uint32_t linesScanned = 0;
    uint8_t specialCharCounterInitial = 0;
    uint8_t specialCharCounterScanning = 0;
    bool delimiterDetermined = true;

    while ((ret = fread(mParseBuffer, 1, sizeof(mParseBuffer), mInputFile)) > 0)
    {
        if (delimiterDetermined)
        {
            ParseCsvBlock(';', ret);
        }
        else
        {
            for (index = 0; index < ret; index++)
            {
                char c = (char)mParseBuffer[index];

                if (c == ':' || c == ',' || c == ';' || c == '|' || c == '\t')
                {
                    if (linesScanned)
                    {

                    }
                    else
                    {

                    }
                }
            }
        }

        total += ret;
    }

    printf("Read %u bytes from CSV file\n", total);

    return result;
}

int CodeGenerator::ParseTemplateBlock(size_t length)
{
    int result = 0;
    size_t index;

    for (index = 0; index < length && result == 0; index++)
    {
        uint8_t mParseStatePrevious = mParseState;
        mParseState = templateStateTranstionTable[mParseState][charCompressionTable[mParseBuffer[index]]];
        fprintf(mLogDest, "[%4u:%2u] %2u (%c): %2u > %2u\n",
               mParseLineCount,
               mParseCharCount,
               mParseBuffer[index],
               mParseBuffer[index] > 0x20 ? mParseBuffer[index] : ' ',
               mParseStatePrevious,
               mParseState);
        mParseCharCount++;
        switch (mParseState)
        {
        case 0: /* 0 - Default state */
        case 2: /* 4 - Double @ section */
            mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
            mParseTagOnly = false;
            if (mParseOutputOffset == sizeof(mParseOutput))
            {
                result = DumpOutputBuffer();
            }
            break;
        case 1: /* 3 - Field name start section */
            if (mParseCharCount == 1)
            {
                mParseTagOnly = true;
            }
            result = DumpOutputBuffer();
            break;
        case 3: /* 3 - Field name continue section */
            mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
            if (mParseOutputOffset == sizeof(mParseOutput))
            {
                result = -1;
            }
            break;
        case 5: /* 5 - Field complete LF */
        case 6: /* 6 - Field complete CR */
            mParseCharCount = 0;
            mParseLineCount++;
            /* Fallthrough */
        case 4: /* 4 - Field complete no parameters */
        case 7: /* 7 - Field complete @ */
            mParseOutput[mParseOutputOffset++] = 0;
            result = ProcessTag((const char*)mParseOutput, mParseOutputOffset);
            mParseOutputOffset = 0;
            if (!mParseTagOnly)
            {
                mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
            }
            break;
        case 8: /* 8 - LF */
        case 9: /* 9 - CR */
            mParseCharCount = 0;
            mParseLineCount++;

            mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
            if (mParseOutputOffset == sizeof(mParseOutput))
            {
                result = DumpOutputBuffer();
            }
            break;
        case 10: /* 10 - CR>LF or LF>CR */
            mParseCharCount = 0;

            if (!mParseTagOnly)
            {
                mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
                if (mParseOutputOffset == sizeof(mParseOutput))
                {
                    result = DumpOutputBuffer();
                }
            }
            mParseTagOnly = false;
            break;
        case 11: /* 11 - Params begin */
            mParseOutput[mParseOutputOffset++] = 0;
            result = ProcessTag((const char*)mParseOutput, mParseOutputOffset);
            mParseOutputOffset = 0;
            if (!mParseTagOnly)
            {
                mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
            }
            break;
        case 12: /* 12 - Params name spacing */
        case 18: /* 18 - Params spacing 1 */
        case 23: /* 23 - Params spacing 2 */
        case 37: /* 37 - Params value end spacing */
            break;
        case 19: /* 19 - Params spacing 1 LF */
        case 20: /* 20 - Params spacing 1 CR */
        case 24: /* 24 - Params spacing 2 LF */
        case 25: /* 25 - Params spacing 2 CR */
        case 28: /* 28 - Params " section LF */
        case 29: /* 29 - Params " section CR */
        case 32: /* 32 - Params ' section LF */
        case 33: /* 33 - Params ' section CR */
        case 38:/* 38 - Params value end LF */
        case 39:/* 39 - Params value end CR */
            mParseCharCount = 0;
            mParseLineCount++;
            break;
        case 13: /* 13 - Params name LF */
        case 14: /* 14 - Params name CR */
            if (mParseOutputOffset != 0)
            {

                mParseOutputOffset = 0;
            }
            mParseCharCount = 0;
            mParseLineCount++;
            break;
        case 15: /* 15 - Params name LF<CR or CR>LF */
        case 21: /* 21 - Params spacing 1 LF<CR or CR>LF */
        case 26: /* 26 - Params spacing 2 LF<CR or CR>LF */
        case 30: /* 30 - Params " section LF<CR or CR>LF */
        case 34: /* 34 - Params ' section LF<CR or CR>LF */
        case 40:/* 40 - Params value end LF<CR or CR>LF */
            mParseCharCount = 0;
            break;
        case 16: /* 16 - Params name continue section */
            mParseOutput[mParseOutputOffset++] = mParseBuffer[index];
            if (mParseOutputOffset == sizeof(mParseOutput))
            {
                result = -1;
            }
        case 17: /* 17 - Params name error */
            result = -1;
            break;
        case 22: /* 22 - Params spacing error */
        case 27: /* 27 - Params value " section */
        case 31: /* 31 - Params value ' section */
        case 35: /* 35 - Params value continue */
        case 36: /* 36 - Params value end */
        case 41:/* 41 - Params value error */
        case 42:/* 42 - Params end */
            break;
        }
    }

    return DumpOutputBuffer();
}

int CodeGenerator::ParseTemplateInputFile()
{
    int result = 0;
    size_t ret, total = 0;

    mParseState        = 0;
    mParseOutputOffset = 0;
    mParseFieldCount   = 0;
    mParseCharCount    = 0;
    mParseLineCount    = 0;
    mParseTagOnly      = false;

    while ((ret = fread(mParseBuffer, 1, sizeof(mParseBuffer), mTemplateFile)) > 0)
    {
        if ((result = ParseTemplateBlock(ret)) != 0)
        {
            break;
        }
    }

    return result;
}

int CodeGenerator::ProcessTag(const char *tag, uint32_t tag_length)
{
    fprintf(mLogDest, "TAG: %s\n", tag);

    return 0;
}

int CodeGenerator::Run(int argc, char **argv)
{
    int result = 0;

    if (argc < 4)
    {
        Usage(argv[0]);
    }
    else
    {
        mInputFile = fopen(argv[1], "rb");
        if (mInputFile == NULL)
        {
            printf("ERROR: File not found (%s)\n", argv[1]);
            Usage(argv[0]);
            return 1;
        }

        mTemplateFile = fopen(argv[2], "rb");
        if (mTemplateFile == NULL)
        {
            printf("ERROR: File not found (%s)\n", argv[2]);
            Usage(argv[0]);
            return 1;
        }

        mOutputFile = fopen(argv[3], "wb");
        if (mOutputFile == NULL)
        {
            printf("ERROR: Unable to create file: %s\n", argv[3]);
            Usage(argv[0]);
            return 1;
        }

        if (argc >= 5)
        {
            mLogFile = fopen(argv[4], "wb");
            if (mLogFile == NULL)
            {
                printf("ERROR: Unable to create file: %s\n", argv[3]);
                Usage(argv[0]);
                return 1;
            }
            else
            {
                mLogDest = mLogFile;
            }
        }

        result = ParseCsvInputFile();
        if (result != 0)
        {
            printf("ERROR: Unable to parse input file\n");
            Usage(argv[0]);
            return 1;
        }

        result = ParseTemplateInputFile();
        if (result != 0)
        {
            printf("ERROR: Unable to parse template file\n");
            Usage(argv[0]);
            return 1;
        }
    }

    return 0;
}

void CodeGenerator::Usage(const char *appName)
{
    printf("Usage: %s <input_file> <template_file> <output_file>\n", appName);
}

int main(int argc, char **argv)
{
    int             result          = 2;
    CodeGenerator   *codeGenerator  = new CodeGenerator();

    if (codeGenerator != NULL)
    {
        result = codeGenerator->Run(argc, argv);

        delete codeGenerator;
    }

    return result;
}
