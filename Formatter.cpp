#include "Formatter.h"


Formatter::Formatter()
{

}

Formatter::~Formatter()
{
    
}

void Formatter::OutputLowerCase(FILE *f, std::string& str)
{
    bool first = true;
    for ( std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        if (first)
        {
            first = false;
        }
        else if (std::isupper(*it))
        {
            fputc('_', f);    
        }

        fputc(std::tolower(*it), f);
    }
}

void Formatter::OutputUpperCase(FILE *f, std::string& str)
{
    bool first = true;
    for ( std::string::iterator it = str.begin(); it != str.end(); ++it)
    {
        if (first)
        {
            first = false;
        }
        else if (std::isupper(*it))
        {
            fputc('_', f);    
        }
        
        fputc(std::toupper(*it), f);
    }
}
