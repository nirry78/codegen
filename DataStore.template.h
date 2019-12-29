#ifndef _DATASTORE_H
#define _DATASTORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

class DataStore
{
    private:
@ForEachField
        @Field{name='type'} m@Field{name='name'};
@EndForEachField

    public:
        DataStore();
        ~DataStore();

        void        Dump(FILE *dest);
        uint32_t    Serialize(uint8_t *buffer, uint32_t bufferLength);
        uint32_t    Unserialize(uint8_t *buffer, uint32_t bufferLength);
};

#endif

