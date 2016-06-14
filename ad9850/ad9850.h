#ifndef AD9850_H
#define AD9850_H

#include <stdint.h>

void dds_setup(void);
void dds_reset(void);
void dds_write(uint8_t byte);
void dds_update_freq(float freq);

#endif  // AD9850_H
