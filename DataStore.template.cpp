#include "DataStore.h"

@foreachcontainer

@container{name = 'name'}::@container{name = 'name'}():
@ForEachField{group = 'parameter'}
    m@field{name='name'}(@field{name='default'})@separator
@endforeachfield
{

}

@container{name = 'name'}::~@container{name = 'name'}()
{

}

void @container{name = 'name'}::Dump(FILE *dest)
{
@foreachfield
//    fprintf(dest, "@field{name='name'} = @field{name='format'}\n", m@field{name = 'name'});
@endforeachfield
}

uint32_t @container{name = 'name'}::Serialize(uint8_t *buffer, uint32_t bufferLength)
{
    uint32_t offset = 0;
    uint32_t length = 0;

@foreachfield
    if ((offset + length) < bufferLength)
    {

    }
@endforeachfield

    return length;
}

uint32_t @container{name = 'name'}::Unserialize(uint8_t *buffer, uint32_t bufferLength)
{
@foreachfield
@endforeachfield

    return bufferLength;
}

@endforeachcontainer
