#ifndef _DATASTORE_H
#define _DATASTORE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "Platform.h"

@foreachcontainer
#define @container{name = 'name', style = 'upper_case'}_IDENTIFIER @container{name = 'count', alignment='60'}
@endforeachcontainer

@foreachcontainer

class @container{name = 'name'}
{
    private:
@ForEachField{group = 'parameter'}
        @Field{name='type'} m@Field{name='name'}@Field{name='length', style='array'};
@EndForEachField

    public:
        @container{name = 'name'}(@ForEachField{group = 'parameter'}@Field{name='type', style='reference'} m@Field{name='name'}@EndForEachField);
        ~@container{name = 'name'}();

        void        Dump(FILE *dest);
        uint32_t    Serialize(uint8_t *buffer, uint32_t bufferLength);
        uint32_t    Unserialize(uint8_t *buffer, uint32_t bufferLength);
};

@endforeachcontainer

#endif

