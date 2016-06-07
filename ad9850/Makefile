BINARY = example
SRCS = $(filter-out $(BINARY).c,$(wildcard *.c))
OBJS = $(subst .c,.o,$(SRCS))

OPENCM3_DIR = ../libopencm3
LDSCRIPT = $(OPENCM3_DIR)/lib/stm32/f1/stm32f103x8.ld

include ../libopencm3.target.mk
