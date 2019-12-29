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
#include "Message.h"

using json = nlohmann::json;

class JsonReader
{
    private:
        json                    mJsonObject;
        std::list <Message>     mMessageList;

        void    VerifyMessages(json& object);
        void    VerifyParameters(Message& message, json& object);
    public:
                JsonReader();
        virtual ~JsonReader();
        bool    ReadFile(const char *filename);
};

#endif /* _JSON_READER_H */