#ifndef _DATA_TYPE_H
#define _DATA_TYPE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NAME_LENGTH         256
#define LIST_BLOCK_SIZE     16

typedef enum {
    DATA_TYPE_INT8,
    DATA_TYPE_INT16,
    DATA_TYPE_INT32,
    DATA_TYPE_INT64,
    DATA_TYPE_UINT8,
    DATA_TYPE_UINT16,
    DATA_TYPE_UINT32,
    DATA_TYPE_UINT64,
    DATA_TYPE_UUID,
    DATA_TYPE_CONTAINER,
    DATA_TYPE_CHOICE,
    DATA_TYPE_NAMESPACE,
} DataTypeT;

typedef enum {
    DATA_SCOPE_PRIVATE,
    DATA_SCOPE_PROTECTED,
    DATA_SCOPE_PUBLIC,
} DataScopeT;

typedef struct
{
    char                    mName[NAME_LENGTH];
    char                    mValue[NAME_LENGTH];
    char                    *mDescription;
} NamedValue;

class DataType
{
    protected:
        DataTypeT           mType;
        char                mTypeString[NAME_LENGTH];
        char                mName[NAME_LENGTH];
        char                *mDescription;
        DataScopeT          mScope;
        class DataType      **mContainerList;
        uint32_t            mContainerListCount;
        uint32_t            mContainerListReadOffset;
        uint32_t            mContainerListWriteOffset;

        NamedValue          **mNamedValueList;
        uint32_t            mNamedValueListCount;
        uint32_t            mNamedValueListOffset;

        union 
        {
            uint64_t        mUint64;
            int64_t         mInt64;
        } mMinumimValue;

        union
        {
            uint64_t        mUint64;
            int64_t         mInt64;
        } mMaximumValue;

        union
        {
            uint64_t        mUint64;
            int64_t         mInt64;
        } mDefaultValue;

                            DataType(DataTypeT type, 
                                     const char *name, 
                                     const char *typeString, 
                                     int64_t defaultValue, 
                                     int64_t minimumValue, 
                                     int64_t maximumValue);
        virtual             ~DataType();
    public:

        bool                AddDataType(class DataType *dataType);
        bool                AddNamedValue(const char *name, const char *value, const char *description = NULL);
        const char         *GetTypeAsString() { return mTypeString; }
        const char         *GetDescription() { return mDescription ? mDescription : ""; }
        DataTypeT           GetType() { return mType; }
        DataType            *Next();
        DataType            *ResolveName(const char *name);
        int32_t             Print(FILE *output, const char *field);
};

class DataTypeUint8: public DataType
{
    public:
        DataTypeUint8(const char *name, uint8_t defaultValue = 0, uint8_t minimumValue = 0, uint8_t maximumValue = 0xFF):
            DataType(DATA_TYPE_UINT8, name, "uint8_t", defaultValue, minimumValue, maximumValue) {}
};

class DataTypeUint16: public DataType
{
    public:
        DataTypeUint16(const char *name, uint16_t defaultValue = 0, uint16_t minimumValue = 0, uint16_t maximumValue = 0xFFFF):
            DataType(DATA_TYPE_UINT16, name, "uint16_t", defaultValue, minimumValue, maximumValue) {}

};

class DataTypeUint32: public DataType
{
    public:
        DataTypeUint32(const char *name, uint32_t defaultValue = 0, uint32_t minimumValue = 0, uint32_t maximumValue = 0xFFFFFFFF) :
            DataType(DATA_TYPE_UINT32, name, "uint32_t", defaultValue, minimumValue, maximumValue) {}

};

class DataTypeUint64: public DataType
{
    public:
        DataTypeUint64(const char *name, uint64_t defaultValue = 0, uint64_t minimumValue = 0, uint64_t maximumValue = 0xFFFFFFFFFFFFFFFFL):
            DataType(DATA_TYPE_UINT64, name, "uint64_t", defaultValue, minimumValue, maximumValue) {}

};

class DataTypeInt8: public DataType
{
    public:
        DataTypeInt8(const char *name, int8_t defaultValue = 0, int8_t minimumValue = (int8_t)0x80, int8_t maximumValue = 0x7F) :
            DataType(DATA_TYPE_INT8, name, "int8_t", defaultValue, minimumValue, maximumValue) {}

};

class DataTypeInt16: public DataType
{
    public:
        DataTypeInt16(const char *name, int16_t defaultValue = 0, int16_t minimumValue = (int16_t)0x8000, int16_t maximumValue = 0x7FFF) :
            DataType(DATA_TYPE_INT16, name, "int16_t", defaultValue, minimumValue, maximumValue) {}

};

class DataTypeInt32: public DataType
{
    public:
        DataTypeInt32(const char *name, int32_t defaultValue = 0, int32_t minimumValue = (int32_t)0x80000000, int32_t maximumValue = 0x7FFFFFFF) :
            DataType(DATA_TYPE_INT32, name, "int32_t", defaultValue, minimumValue, maximumValue) {}

};

class DataTypeInt64: public DataType
{
    public:
        DataTypeInt64(const char *name, int64_t defaultValue = 0, int64_t minimumValue = (int64_t)0x8000000000000000L, int64_t maximumValue = 0x7FFFFFFFFFFFFFFFL) :
            DataType(DATA_TYPE_INT64, name, "int64_t", defaultValue, minimumValue, maximumValue) {}
};

class DataTypeUuid: public DataType
{
    public:
        DataTypeUuid(const char *name) : DataType(DATA_TYPE_UUID, name, "uint8_t[16]", 0, 0, 0) {}
};

class DataTypeContainer: public DataType
{
    public:
        DataTypeContainer(const char *name) : DataType(DATA_TYPE_CONTAINER, name, "", 0, 0, 0) {}
};

class DataTypeChoice: public DataType
{
    public:
        DataTypeChoice(const char *typeString) : DataType(DATA_TYPE_CHOICE, "", typeString, 0, 0, 0) {}
};

class DataTypeNamespace: public DataType
{
    public:
        DataTypeNamespace(const char *name) : DataType(DATA_TYPE_NAMESPACE, name, "", 0, 0, 0) {}
};

DataType* DataTypeTester(void);

#endif