#include "Formatter.h"


Formatter::Formatter()
{

}

Formatter::~Formatter()
{
    
}

int Formatter::OutputLowerCase(FILE *f, std::string& str)
{
    int count = 0;
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
            count++;
        }

        fputc(std::tolower(*it), f);
        count++;
    }

    return count;
}

int Formatter::OutputUpperCase(FILE *f, std::string& str)
{
    int count = 0;
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
            count++;
        }
        
        fputc(std::toupper(*it), f);
        count++;
    }

    return count;
}
