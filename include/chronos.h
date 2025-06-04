#ifndef __CHRONOS_H__
#define __CHRONOS_H__

#include <time.h>


typedef clock_t JM_Timestamp;

/// @brief acquire a timing instance
/// @return a timestamp
static JM_Timestamp jm_chronos_start()
{
    return clock();
}
/// @brief ends the timing instance
/// @param start offset from
/// @return the seconds since in cpu time
static F32 jm_chronos_end(const JM_Timestamp start)
{
    return (F32) (clock() - start) / CLOCKS_PER_SEC;
}

#define JM_TIMED_BLOCK(block_statements)                             \
    ({                                                               \
        JM_Timestamp __jm_start_time_macro_var = jm_chronos_start(); \
        block_statements;                                            \
        jm_chronos_end(__jm_start_time_macro_var);                   \
    })

#endif