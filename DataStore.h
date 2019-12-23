#ifndef _DATASTORE_H
#define _DATASTORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

class DataStore
{
    private:
        uint32_t mType;
        uint8_t* mName;

    public:
        DataStore();
        ~DataStore();

        void        Dump(FILE *dest);
};

#endif

