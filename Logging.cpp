#include "Logging.h"

Logging gLogging;

Logging::Logging():
    mFileLogLevel(LOGGING_LEVEL_SILENT), mDisplayLogLevel(LOGGING_LEVEL_INFO)
{

}

Logging::~Logging()
{
    if (mFileLogLevel)
    {
        fclose(mFileHandle);
    }
}

void Logging::OpenLogFile(const char *filename)
{
    gLogging.mFileHandle = fopen(filename, "w");
}

void Logging::Output(uint32_t level, const char *format, va_list va)
{
    va_list va2;
    
    va_copy(va2, va);

    if (mFileHandle && level <= mFileLogLevel)
    {
        vfprintf(mFileHandle, format, va);
    }

    if (level <= mDisplayLogLevel)
    {
        vprintf(format, va2);
    }
}

void Logging::SetLogLevel(uint32_t fileLogLevel, uint32_t displayLogLevel)
{
    if (fileLogLevel <= LOGGING_LEVEL_MAX)
    {
        mFileLogLevel = fileLogLevel;
    }

    if (displayLogLevel <= LOGGING_LEVEL_MAX)
    {
        mDisplayLogLevel = displayLogLevel;
    }
}

void Logging::StateTransition(uint32_t state, uint8_t transition)
{
    mStateTransitionList[mStateTransitionListOffset].mState = state;
    mStateTransitionList[mStateTransitionListOffset].mTransition = transition;

    mStateTransitionListOffset++;

    if (mStateTransitionListOffset >= LOGGING_MAX_STATE_TRANSITION_LIST_COUNT ||
        transition == '\n')
    {
        size_t offset = 0;
        
        offset += sprintf(&mFormatBuffer[offset], " ");

        for (uint32_t index = 0; index < mStateTransitionListOffset; index++)
        {
            if (mStateTransitionList[index].mTransition >= 20)
            {
                offset += sprintf(&mFormatBuffer[offset], " '%c'",  mStateTransitionList[index].mTransition);
            }
            else if (mStateTransitionList[index].mTransition == '\n')
            {
                /* code */
                offset += sprintf(&mFormatBuffer[offset], " CR");
            }
            else
            {
                offset += sprintf(&mFormatBuffer[offset], " %3u",  mStateTransitionList[index].mTransition);
            }
        }

        offset += sprintf(&mFormatBuffer[offset], "\n");

        LOGD(mFormatBuffer);

        offset = 0;
        for (uint32_t index = 0; index < mStateTransitionListOffset; index++)
        {
            offset += sprintf(&mFormatBuffer[offset], " %3u",  mStateTransitionList[index].mState);
        }

        offset += sprintf(&mFormatBuffer[offset], "\n");

        LOGD(mFormatBuffer);

        mStateTransitionListOffset = 0;
    }
}
