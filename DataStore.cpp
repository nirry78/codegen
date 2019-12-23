#include "DataStore.h"


DataStore::DataStore():
    mType(12),
    mLength(1234),
    mListOffset(123456),
    mPower(234920923408092384ULL),
    mV1(-1),
    mV2(-23942),
    mV3(-1233451),
    mV4(-31234532452345LL)
{

}

DataStore::~DataStore()
{

}

void DataStore::Dump(FILE *dest)
{
    fprintf(dest, "Type = %u\n", mType);
    fprintf(dest, "Length = %u\n", mLength);
    fprintf(dest, "ListOffset = %u\n", mListOffset);
    fprintf(dest, "Power = %llu\n", mPower);
    fprintf(dest, "V1 = %d\n", mV1);
    fprintf(dest, "V2 = %d\n", mV2);
    fprintf(dest, "V3 = %d\n", mV3);
    fprintf(dest, "V4 = %lld\n", mV4);
}

uint32_t DataStore::Serialize(uint8_t *buffer, uint32_t bufferLength)
{
    uint32_t offset = 0;
    uint32_t length = 0;

    if ((offset + length) < bufferLength)
    {

    }
    if ((offset + length) < bufferLength)
    {

    }
    if ((offset + length) < bufferLength)
    {

    }
    if ((offset + length) < bufferLength)
    {

    }
    if ((offset + length) < bufferLength)
    {

    }
    if ((offset + length) < bufferLength)
    {

    }
    if ((offset + length) < bufferLength)
    {

    }
    if ((offset + length) < bufferLength)
    {

    }
}

uint32_t DataStore::Unserialize(uint8_t *buffer, uint32_t bufferLength)
{
}

