#ifndef _FORMATTER_H
#define _FORMATTER_H

#include <string>
#include "Object.h"

class Formatter: public Object
{
    private:

    public:
                        Formatter();
        virtual         ~Formatter();

        void            OutputLowerCase(FILE *f, std::string& str);
        void            OutputUpperCase(FILE *f, std::string& str);
};

#endif /* _FORMATTER_H */