#include "Platform.h"
#include "DataType.h"

DataType::DataType(DataTypeT type, const char *name, const char *typeString, int64_t defaultValue, int64_t minimumValue, int64_t maximumValue):
    mType(type),
    mDescription(NULL),
    mScope(DATA_SCOPE_PRIVATE),
    mContainerList(NULL),
    mContainerListCount(0),
    mContainerListReadOffset(0),
    mContainerListWriteOffset(0),
    mNamedValueList(NULL),
    mNamedValueListCount(0),
    mNamedValueListOffset(0)
{
    strncpy_s(mName, name, NAME_LENGTH);
    strncpy_s(mTypeString, typeString, NAME_LENGTH);
    mMinumimValue.mInt64 = minimumValue;
    mMaximumValue.mInt64 = maximumValue;
    mDefaultValue.mInt64 = defaultValue;
};

DataType::~DataType()
{
    if (mContainerList != NULL)
    {
        for (uint32_t index = 0; index < mContainerListWriteOffset; index++)
        {
            delete mContainerList[index];
        }

        delete mContainerList;
    }

    if (mNamedValueList != NULL)
    {
        for (uint32_t index = 0; index < mNamedValueListOffset; index++)
        {
            if (mNamedValueList[index]->mDescription != NULL)
            {
                free(mNamedValueList[index]->mDescription);
            }

            delete mNamedValueList[index];
        }

        delete mNamedValueList;
    }

    if (mDescription != NULL)
    {
        delete mDescription;
    }
}

bool DataType::AddDataType(class DataType *dataType)
{
    bool result = false;

    if (dataType != NULL)
    {
        if (mContainerList == NULL)
        {
            mContainerList = (DataType**) new size_t[LIST_BLOCK_SIZE];
            mContainerListCount = (mContainerList != NULL ? LIST_BLOCK_SIZE : 0);
        }
        else if (mContainerListWriteOffset == mContainerListCount)
        {
            DataType** list;

            list = (DataType**) new size_t[mContainerListCount + LIST_BLOCK_SIZE];
            if (list != NULL)
            {
                memcpy(list, mContainerList, sizeof(DataType**) * mContainerListCount);

                delete mContainerList;

                mContainerList = list;
                mContainerListCount += LIST_BLOCK_SIZE;
            }
        }

        if (mContainerListWriteOffset != mContainerListCount)
        {
            mContainerList[mContainerListWriteOffset] = dataType;

            mContainerListWriteOffset++;

            result = true;
        }
    }

    return result;
}

bool DataType::AddNamedValue(const char *name, const char *value, const char *description)
{
    bool result = false;

    if (mNamedValueList == NULL)
    {
        mNamedValueList = (NamedValue**) new size_t[LIST_BLOCK_SIZE];
        mNamedValueListCount = (mNamedValueList != NULL ? LIST_BLOCK_SIZE : 0);
    }
    else if (mNamedValueListOffset == mNamedValueListCount)
    {
        NamedValue** list;
        
        list = (NamedValue**) new size_t[mNamedValueListCount + LIST_BLOCK_SIZE];
        if (list != NULL)
        {
            memcpy(list, mNamedValueList, sizeof(NamedValue**) * mNamedValueListCount);
            
            delete mNamedValueList;

            mNamedValueList      = list;
            mNamedValueListCount += LIST_BLOCK_SIZE;
        }
    }

    if (mNamedValueListOffset != mNamedValueListCount)
    {
        mNamedValueList[mNamedValueListOffset] = new NamedValue;

        if (mNamedValueList[mNamedValueListOffset] != NULL)
        {
            strncpy_s(mNamedValueList[mNamedValueListOffset]->mName, name, NAME_LENGTH);
            strncpy_s(mNamedValueList[mNamedValueListOffset]->mValue, value, NAME_LENGTH);

            if (description)
            {
                mNamedValueList[mNamedValueListOffset]->mDescription = _strdup(description);
            }
            else
            {
                mNamedValueList[mNamedValueListOffset]->mDescription = NULL;
            }

            mNamedValueListOffset++;

            result = true;
        }
    }

    return result;
}

DataType *DataType::Next()
{
    return mContainerListReadOffset != mContainerListWriteOffset ? mContainerList[mContainerListReadOffset++] : NULL;
}

DataType *DataType::Reset()
{
    mContainerListReadOffset = 0;

    return Next();
}

DataType *DataType::ResolveName(const char *name)
{
    DataType *dt = NULL;

    if (name != NULL)
    {
        const char *nameEnd = strchr(name, '.');

        if (nameEnd != NULL)
        {
            if (!_strnicmp(name, mName, nameEnd - name))
            {
                for (uint32_t index = 0; index < mContainerListWriteOffset; index++)
                {
                    dt = mContainerList[index]->ResolveName(&nameEnd[1]);
                }
            }
        }
        else if (!_stricmp(name, mName) || name[0] == '*')
        {
            mContainerListReadOffset = 0;

            dt = this;
        }
    }

    return dt;
}

int32_t DataType::Print(FILE *output, const char *field)
{
    if (!_strnicmp(field, "name", 4))
    {
        fprintf(output, "%s", mName);
    }
    else if (!_strnicmp(field, "type", 4))
    {
        fprintf(output, "%s", mTypeString);
    }
    else if (!_strnicmp(field, "default", 7))
    {
        OnPrintDefaultValue(output);
    }
    else if (!_strnicmp(field, "format", 6))
    {
        OnPrintFormat(output, false);
    }
    else if (!_strnicmp(field, "minimum_value", 13))
    {
        OnPrintMinimumValue(output);
    }
    else if (!_strnicmp(field, "maximum_value", 13))
    {
        OnPrintMaximumValue(output);
    }
    else
    {
        printf("[ERROR] BLA\n");
    }

    return 0;
}

DataType* DataTypeTester(void)
{
    DataTypeNamespace *nameSpace = new DataTypeNamespace("test");

    if (nameSpace)
    {
        DataTypeContainer *c1 = new DataTypeContainer("DataStore1");
        DataTypeContainer *c2 = new DataTypeContainer("DataStore2");

        c1->AddDataType(new DataTypeUint8("Type", 12));
        c1->AddDataType(new DataTypeUint16("Length", 1234));
        c1->AddDataType(new DataTypeUint32("ListOffset", 123456));
        c1->AddDataType(new DataTypeUint64("Power", 234920923408092384ULL));
        c1->AddDataType(new DataTypeInt8("V1", -1));
        c1->AddDataType(new DataTypeInt16("V2", -23942));
        c1->AddDataType(new DataTypeInt32("V3", -1233451));
        c1->AddDataType(new DataTypeInt64("V4", -31234532452345L));

        nameSpace->AddDataType(c1);

        c2->AddDataType(new DataTypeUint8("Type", 12));
        c2->AddDataType(new DataTypeUint16("Length", 1234));
        c2->AddDataType(new DataTypeUint32("ListOffset", 123456));
        c2->AddDataType(new DataTypeUint64("Power", 234920923408092384ULL));
        c2->AddDataType(new DataTypeInt8("V1", -1));
        c2->AddDataType(new DataTypeInt16("V2", -23942));
        c2->AddDataType(new DataTypeInt32("V3", -1233451));
        c2->AddDataType(new DataTypeInt64("V4", -31234532452345L));

        nameSpace->AddDataType(c2);
    }

    return nameSpace;
}