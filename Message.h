#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <string>
#include <list>
#include <nlohmann/json.hpp>

#include "Logging.h"
#include "MessageField.h"

using json = nlohmann::json;

class Message
{
    private:
        std::string             mName;
        std::list<MessageField> mFieldList;

        void                    ParseParameters(json& object);
    public:
                                Message(json& object);
        virtual                 ~Message();

        bool                    IsValid();
        void                    SetName(std::string& name) { mName = name; }
};

#endif /* _MESSAGE_H */