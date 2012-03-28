#ifndef _AUDIO_SAMPLE_H_
#define _AUDIO_SAMPLE_H_

#include <stdint.h>

#define MINIMUM_SAMPLE_SET_SIZE	32768
#define MINIMUM_MIX_QUEUE_SIZE MINIMUM_SAMPLE_SET_SIZE * 2

/** @brief The AudioSample_t type definition */
typedef int16_t AudioSample_t;

#endif
