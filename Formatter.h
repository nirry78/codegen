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

        int             OutputLowerCase(FILE *f, std::string& str);
        int             OutputUpperCase(FILE *f, std::string& str);
};

#endif /* _FORMATTER_H */