#ifndef _JSON_READER_H
#define _JSON_READER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <list>
#include <nlohmann/json.hpp>
#include "Logging.h"
#include "Container.h"

using json = nlohmann::json;

class JsonReader
{
    private:
        json                            mJsonObject;
        std::list<Container>            mContainerList;
        std::list<Container>::iterator  mContainerIterator;

        void    VerifyContainer(json& object);
        void    VerifyParameters(Container& container, json& object);
    public:
                JsonReader();
        virtual ~JsonReader();
        bool    ReadFile(const char *filename);
        bool    SelectNamespace(const char *value);
        bool    ForeachContainerReset();
        bool    ForeachContainerNext();
        bool    ForeachFieldReset();
        bool    ForeachFieldNext();
        bool    OutputContainer(FILE *outputFile, const char *name, Tag* tag);
        bool    OutputField(FILE *outputFile, const char *name, Tag* tag);
};

#endif /* _JSON_READER_H */