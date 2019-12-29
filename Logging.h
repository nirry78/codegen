#ifndef _LOGGING_H
#define _LOGGING_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define LOGGING_LEVEL_SILENT    (0)
#define LOGGING_LEVEL_ERROR     (1)
#define LOGGING_LEVEL_INFO      (2)
#define LOGGING_LEVEL_VERBOSE   (3)
#define LOGGING_LEVEL_DEBUG     (4)
#define LOGGING_LEVEL_MAX       (4)

#define LOGGING_MAX_STATE_TRANSITION_LIST_COUNT     (512)

class Logging
{
    private:
        typedef struct 
        {
            uint32_t        mState;
            uint8_t         mTransition;
        } StateTransitionT;

        FILE                *mFileHandle;
        uint32_t            mFileLogLevel;
        uint32_t            mDisplayLogLevel;
        StateTransitionT    mStateTransitionList[LOGGING_MAX_STATE_TRANSITION_LIST_COUNT];
        uint32_t            mStateTransitionListOffset;
        char                mFormatBuffer[(LOGGING_MAX_STATE_TRANSITION_LIST_COUNT << 2) + 4];

    public:
                    Logging();
        virtual     ~Logging();

        static void OpenLogFile(const char *filename);
        void        Output(uint32_t level, const char *format, va_list va);
        void        SetLogLevel(uint32_t fileLogLevel, uint32_t displayLogLevel);
        void        StateTransition(uint32_t state, uint8_t transition);
};

static inline void LOGSETLEVEL(uint32_t fileLogLevel, uint32_t displayLogLevel) {
    extern Logging gLogging;
    gLogging.SetLogLevel(fileLogLevel, displayLogLevel);
}

static inline void LOGD(const char *format, ...) {
    extern Logging gLogging;
    va_list va;
    va_start(va, format);
    gLogging.Output(LOGGING_LEVEL_DEBUG, format, va);
    va_end(va);
}

static inline void LOGE(const char *format, ...) {
    extern Logging gLogging;
    va_list va;
    va_start(va, format);
    gLogging.Output(LOGGING_LEVEL_ERROR, format, va);
    va_end(va);
}

static inline void LOGI(const char *format, ...) {
    extern Logging gLogging;
    va_list va;
    va_start(va, format);
    gLogging.Output(LOGGING_LEVEL_INFO, format, va);
    va_end(va);
}

static inline void LOGV(const char *format, ...) {
    extern Logging gLogging;
    va_list va;
    va_start(va, format);
    gLogging.Output(LOGGING_LEVEL_VERBOSE, format, va);
    va_end(va);
}

static inline void LOG_STATE_TRANSITION(uint32_t state, uint8_t transition) {
    extern Logging gLogging;
    gLogging.StateTransition(state, transition);
}

#endif

