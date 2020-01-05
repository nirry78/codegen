#ifndef _FORMATTER_H
#define _FORMATTER_H

#include <string>

class Formatter
{
    private:

    public:
                        Formatter();
        virtual         ~Formatter();

        void            OutputLowerCase(FILE *f, std::string& str);
        void            OutputUpperCase(FILE *f, std::string& str);
};

#endif /* _FORMATTER_H */