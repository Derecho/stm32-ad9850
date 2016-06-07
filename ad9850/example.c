#include "ad9850.h"
#include "example.h"

static const uint32_t SWEEP_BEGIN = 1e6;
static const uint32_t SWEEP_END = 30e6;
static const uint32_t SWEEP_STEP = 2e3;

void delay_ms(uint32_t delay)
{
    uint32_t cycles = (delay * 8000)/5;  // 8 Mhz, CMP+BEQ+NOP+ADDS+B
    uint32_t i = 0;
    while(i++ < cycles) {
        __asm__("nop");
    }
}

int main(void)
{
    dds_setup();
    dds_reset();

    dds_update_freq(15e6);

    delay_ms(3000);

    while (1) {
        float i;
        for(i = SWEEP_BEGIN; i < SWEEP_END; i += SWEEP_STEP) {
            dds_update_freq(i);
            delay_ms(1);
        }
        for(; i > SWEEP_BEGIN; i -= SWEEP_STEP) {
            dds_update_freq(i);
            delay_ms(1);
        }

    }

    return 0;
}
