#include "Document.h"

Document::Document(std::string& filename):
    mCurrentLineOffset(0), mOutputOffset(0), mSaved(false), mStringOffset(0)
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

void Document::Fill(const char c, uint32_t width)
{
    while (mCurrentLineOffset < width)
    {
        mOutputBuffer[mOutputOffset++] = c;
        mCurrentLineOffset++;
    }
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
        for (uint32_t index = 0; index < bufferLength; index++)
        {
            if (buffer[index] == '\r' || buffer[index] == '\n')
            {
                mCurrentLineOffset = 0;
            }
            else
            {
                mCurrentLineOffset++;
            }
            
            mOutputBuffer[mOutputOffset++] = buffer[index];
        }

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