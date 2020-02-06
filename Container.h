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
        std::string                  mGroup;
        std::list<Field>             mFieldList;
        std::list<Field>::iterator   mFieldIterator;
        Tag*                         mIteratorTag;

        void                    ParseParameters(json& object);
    public:
                                Container(json& object);
        virtual                 ~Container();

        bool                    ForeachFieldReset(Tag *tag);
        bool                    ForeachFieldNext();
        bool                    IsValid();
        void                    Output(Document* document, std::string& name, Tag* tag);
        void                    OutputField(Document* document, std::string& name, Tag* tag);
        void                    SetName(std::string& name) { mName = name; }
};

#endif /* _MESSAGE_H */