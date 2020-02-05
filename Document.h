#ifndef _DOCUMENT_H
#define _DOCUMENT_H

#include <stdio.h>
#include <string.h>
#include "Object.h"

#define DOCUMENT_STRING_LENGTH      (2048)
#define DOCUMENT_OUTPUT_LENGTH      (4*1024*1024)

class Document: public Object
{
    private:
        FILE*   mFileHandle;
        char*   mOutputBuffer;
        size_t  mOutputOffset;
        bool    mSaved;
        char    mStringBuffer[DOCUMENT_STRING_LENGTH];
        int     mStringOffset;
    public:
        Document(std::string& filename);
        virtual ~Document();

        void    Append(const char *format, ...);
        void    Output(const char *format, ...);
        void    Output(std::string& str, uint32_t width = 0);
        void    OutputBuffer(const char *buffer, size_t bufferLength);
        void    OutputCamelCase(std::string& str, uint32_t width = 0);
        void    OutputLowerCamelCase(std::string& str, uint32_t width = 0);
        void    OutputLowerCase(std::string& str, uint32_t width = 0);
        void    OutputUpperCase(std::string& str, uint32_t width = 0);
        void    Save();
};

#endif /* _DOCUMENT_H */