#include "Document.h"

Document::Document(std::string& filename):
    mOutputOffset(0), mSaved(false), mStringOffset(0)
{
    mFileHandle = fopen(filename.c_str(), "wb");
    mOutputBuffer = (char*)malloc(DOCUMENT_OUTPUT_LENGTH);
}

Document::~Document()
{
    Save();

    if (mFileHandle)
    {
        fclose(mFileHandle);
    }
}

void Document::Append(const char *format, ...)
{
    va_list va;
    int length;

    va_start(va, format);
    length = vsprintf(mStringBuffer, format, va);
    va_end(va);

    OutputBuffer(mStringBuffer, length);
}

void Document::Output(const char *format, ...)
{
    va_list va;
    int length;

    va_start(va, format);
    length = vsprintf(mStringBuffer, format, va);
    va_end(va);

    OutputBuffer(mStringBuffer, length);
}

void Document::Output(std::string& str, uint32_t width)
{
    size_t length = str.length();

    if (width > length)
    {
        str.append(width - length, ' ');
    }

    OutputBuffer(str.c_str(), str.length());
}

void Document::OutputBuffer(const char *buffer, size_t bufferLength)
{
    if (mFileHandle && mOutputBuffer)
    {
        memcpy(&mOutputBuffer[mOutputOffset], buffer, bufferLength);

        mOutputOffset += bufferLength;
        mSaved = false;
    }
}

void Document::OutputCamelCase(std::string& str, uint32_t width)
{
    Output(str, width);
}

void Document::OutputLowerCamelCase(std::string& str, uint32_t width)
{
    uint32_t count = 0;
    bool first = true;

    for ( std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        if (first)
        {
            mStringBuffer[count++] = std::tolower(*it);
            first = false;
        }
        else
        {
            mStringBuffer[count++] = *it;
        }        
    }

    while (count < width)
    {
        mStringBuffer[count++] = ' ';
    }

    OutputBuffer(mStringBuffer, count);
}

void Document::OutputLowerCase(std::string& str, uint32_t width)
{
    uint32_t count = 0;
    bool first = true;

    for ( std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        if (first)
        {
            first = false;
        }
        else if (std::isupper(*it))
        {
            mStringBuffer[count++] = '_';
        }

        mStringBuffer[count++] = std::tolower(*it);
    }

    while (count < width)
    {
        mStringBuffer[count++] = ' ';
    }

    OutputBuffer(mStringBuffer, count);
}

void Document::OutputUpperCase(std::string& str, uint32_t width)
{
    uint32_t count = 0;
    bool first = true;

    for ( std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        if (first)
        {
            first = false;
        }
        else if (std::isupper(*it))
        {
            mStringBuffer[count++] = '_';
        }

        mStringBuffer[count++] = std::toupper(*it);
    }

    while (count < width)
    {
        mStringBuffer[count++] = ' ';
    }

    OutputBuffer(mStringBuffer, count);
}

void Document::Save()
{
    if (mFileHandle && mOutputBuffer && !mSaved)
    {
        fwrite(mOutputBuffer, 1, mOutputOffset, mFileHandle);

        mSaved = true;
    }
}