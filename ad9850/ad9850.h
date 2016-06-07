#include <stdint.h>

void dds_setup(void);
void dds_reset(void);
void dds_write(uint8_t byte);
void dds_update_freq(float freq);
