# date 30-Dec-2017
# author <iysheng@163.com>
# Project for yyfish
CROSS_COMPILE?=arm-none-eabi-

PWD := $(shell 'pwd')
TOPDIR ?= $(PWD)

CC:=$(CROSS_COMPILE)gcc
AS:=$(CROSS_COMPILE)as
LD:=$(CROSS_COMPILE)ld
OBJDUMP:=$(CROSS_COMPILE)objdump
OBJCOPY:=$(CROSS_COMPILE)objcopy

#cortex-M7(eg:stm32f767) is armv7e-m
COREFLAGS = -mcpu=cortex-m7

CPUDIR = $(TOPDIR)/core_src
CPUINC = $(TOPDIR)/core_inc
HALDIR = $(TOPDIR)/hal_src
HALINC = $(TOPDIR)/hal_inc
USRDIR = $(TOPDIR)/usr_src
USRINC = $(TOPDIR)/usr_inc

CFLAGS = $(COREFLAGS) -c -O2 
CFLAGS += -I$(HALINC) -I$(USRINC) -I$(CPUINC)
#CFLAGS += -nostdlib
#CFLAGS += -specs=nosys.specs
#CFLAGS += -specs=nano.specs
#CFLAGS += --specs=rdimon.specs
CFLAGS += -Wa,-mthumb

CROSS_COMPILE_PATH ?= $(shell which $(CROSS_COMPILE)gcc)
LD_PATH := /usr/local/arm/gcc-arm-none-eabi-6_2-2016q4/arm-none-eabi/lib/thumb/v7e-m
LGCC_PATH := /usr/local/arm/gcc-arm-none-eabi-6_2-2016q4/lib/gcc/arm-none-eabi/6.2.1/thumb/v7e-m

LDFLAGS = -Bstatic -T STM32F767IGT_ITCM_FLASH.ld --gc-sections
LDFLAGS += -L $(LD_PATH) -l c -L $(LGCC_PATH) -l gcc -Ttext 0x08002000
LDFLAGS += --gc-sections 
$(info "CROSS_COMPILE_PATH="$(CROSS_COMPILE_PATH))

OBJS = $(CPUDIR)/startup_stm32f767xx.o
OBJS += $(CPUDIR)/system_stm32f7xx.o
OBJS += $(USRDIR)/yyfish.o

LIBS = $(HALDIR)/stm32f7xx_hal.o
LIBS += $(HALDIR)/stm32f7xx_hal_cortex.o
LIBS += $(HALDIR)/stm32f7xx_hal_dma.o
LIBS += $(HALDIR)/stm32f7xx_hal_rcc.o
LIBS += $(HALDIR)/stm32f7xx_hal_gpio.o
LIBS += $(HALDIR)/stm32f7xx_hal_uart.o

ALL:$(OBJS) $(LIBS) yyfish yyfish.bin
yyfish.bin:yyfish
	$(OBJCOPY) -O binary $< $@
	
yyfish:$(OBJS) $(LIBS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) $< -o $@

%.o:%.s
	$(AS) $(COREFLAGS) $< -o $@

.PHONY:clean

clean:
	rm -fr $(OBJS) $(LIBS) yyfish

