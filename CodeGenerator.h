#ifndef _CODE_GENERATOR_H
#define _CODE_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

class CodeGenerator
{
    private:

    public:
        CodeGenerator();
        virtual ~CodeGenerator();

        int Run(int argc, char **argv);
};

#endif /* _CODE_GENERATOR_H */