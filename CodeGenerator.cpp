#include "CodeGenerator.h"

CodeGenerator::CodeGenerator()
{

}

CodeGenerator::~CodeGenerator()
{

}

int CodeGenerator::Run(int argc, char **argv)
{
    return 0;
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
