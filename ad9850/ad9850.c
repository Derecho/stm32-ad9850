#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "ad9850.h"

#define DDS_PORT GPIOA
#define DDS_RCC RCC_GPIOA
#define DDS_CLOCK GPIO0
#define DDS_RESET GPIO1
#define DDS_LOAD GPIO2 // Also called FQ_UD (Frequency Update)
#define DDS_DATA GPIO3

static const float DDS_REF = 125e6;
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

void dds_setup(void)
{
	rcc_periph_clock_enable(DDS_RCC);
	gpio_set_mode(DDS_PORT, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
            DDS_CLOCK | DDS_RESET | DDS_LOAD | DDS_DATA);
}

void dds_reset(void)
{
    // Set everything low first
    gpio_clear(DDS_PORT, DDS_CLOCK | DDS_RESET | DDS_LOAD | DDS_DATA);

    // Pulse reset
    gpio_set(DDS_PORT, DDS_RESET);
    gpio_clear(DDS_PORT, DDS_RESET);

    // Pulse clock
    gpio_set(DDS_PORT, DDS_CLOCK);
    gpio_clear(DDS_PORT, DDS_CLOCK);

    // Pulse load
    gpio_set(DDS_PORT, DDS_LOAD);
    gpio_clear(DDS_PORT, DDS_LOAD);
}

void dds_write(uint8_t byte)
{
    uint8_t i;
    uint8_t bit;
    for(i = 0; i < 8; i++) {
        bit = ((byte >> i) & 1);
        if(bit == 1)
            gpio_set(DDS_PORT, DDS_DATA);
        else
            gpio_clear(DDS_PORT, DDS_DATA);

        gpio_set(DDS_PORT, DDS_CLOCK);
        gpio_clear(DDS_PORT, DDS_CLOCK);
    }
}

void dds_update_freq(float freq)
{
    // Updates DDS output frequency. Supply frequency in Hz.

    uint32_t tuning_word = (freq * 4294967295UL) / DDS_REF;
    dds_write(tuning_word & 0xFF);
    dds_write((tuning_word >> 8) & 0xFF);
    dds_write((tuning_word >> 16) & 0xFF);
    dds_write((tuning_word >> 24) & 0xFF);
    dds_write(0);

    gpio_set(DDS_PORT, DDS_LOAD);
    gpio_clear(DDS_PORT, DDS_LOAD);
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
