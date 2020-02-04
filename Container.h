#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include <list>
#include <nlohmann/json.hpp>

#include "Formatter.h"
#include "Logging.h"
#include "Field.h"
#include "Tag.h"

using json = nlohmann::json;

class Container: public Formatter
{
    private:
        std::string                  mName;
        std::list<Field>             mFieldList;
        std::list<Field>::iterator   mFieldIterator;

        void                    ParseParameters(json& object);
    public:
                                Container(json& object);
        virtual                 ~Container();

        bool                    ForeachFieldReset();
        bool                    ForeachFieldNext();
        bool                    IsValid();
        bool                    Output(FILE *outputFile, std::string& name, Tag* tag);
        bool                    OutputField(FILE *outputFile, std::string& name, Tag* tag);
        void                    SetName(std::string& name) { mName = name; }
};

#endif /* _MESSAGE_H */