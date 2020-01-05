#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include <list>
#include <nlohmann/json.hpp>

#include "Formatter.h"
#include "Logging.h"
#include "MessageField.h"
#include "Tag.h"

using json = nlohmann::json;

class Container: public Formatter
{
    private:
        std::string                         mName;
        std::list<MessageField>             mFieldList;
        std::list<MessageField>::iterator   mFieldIterator;

        void                    ParseParameters(json& object);
    public:
                                Container(json& object);
        virtual                 ~Container();

        bool                    ForeachFieldReset();
        bool                    ForeachFieldNext();
        bool                    IsValid();
        bool                    Output(FILE *outputFile, const char *name, Tag* tag);
        bool                    OutputField(FILE *outputFile, const char *name, Tag* tag);
        void                    SetName(std::string& name) { mName = name; }
};

#endif /* _MESSAGE_H */