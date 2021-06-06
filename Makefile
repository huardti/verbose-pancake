SHELL := /bin/bash

include Makefile.project

CUBEDIR=libs
TOOLSDIR=$(CUBEDIR)/devtools

OBJDIR = obj
SRCDIR = src
BINDIR = bin

OBJS = $(patsubst $(SRCDIR)/%.S,$(OBJDIR)/%.o,$(ASMS))
OBJS += $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
OBJS += $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPPSRCS))

ELF = $(BINDIR)/$(PROGNAME).elf

# cpu
CPU = -mcpu=cortex-m4
# fpu
FPU = -mfpu=fpv4-sp-d16
# float-abi
FLOAT-ABI = -mfloat-abi=hard
# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

LDSCRIPT = $(CUBEDIR)/STM32F446RETx_FLASH.ld

# C includes
C_INCLUDES =  \
-I$(CUBEDIR)/Inc \
-I$(CUBEDIR)/Drivers/STM32F4xx_HAL_Driver/Inc \
-I$(CUBEDIR)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy \
-I$(CUBEDIR)/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
-I$(CUBEDIR)/Drivers/CMSIS/Include \
-I$(CUBEDIR)/Drivers/CMSIS/Include

C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F446xx

ARCHFLAGS = -mtune=cortex-m4 -mthumb -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16
FFLAGS = -fsingle-precision-constant
DIRFLAGS += -Isrc

CFLAGS = $(MCU) $(FFLAGS) -O0 -Os -Wall -Wdouble-promotion -std=gnu99 $(DIRFLAGS) $(C_INCLUDES) $(C_DEFS) -fdata-sections -ffunction-sections
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(OBJDIR)/project.map,--cref -Wl,--gc-sections

all: $(ELF)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(CDEPS) Makefile
	@mkdir -p $(dir $@)
	@echo -e "\e[33mCOMPILE $< => $@\e[39m"
	@arm-none-eabi-gcc $(CFLAGS) -Wa,-ahlns=$(OBJDIR)/$*.lst -g -c $< -o $@
	@echo -e "\e[33mDONE $< => $@\e[39m"

$(ELF): Makefile build-lib $(OBJS)
	@mkdir -p $(dir $@)
	@echo -e "\e[33mLINK $@\e[39m"
	arm-none-eabi-gcc $(wildcard obj/*.o) $(filter-out obj/hal/main.o, $(wildcard obj/hal/*.o)) $(LDFLAGS) -o $@
	@echo -e "\e[33mDONE $@\e[39m"

build-lib:
	@echo -e "\e[33mbuilding HAL \e[39m"
	@make -j$(nproc) -C $(CUBEDIR)
	@echo -e "\e[33mHAL done \e[39m"

dump : $(ELF)
	@echo "DUMP $<"
	@arm-none-eabi-objdump -h -t -d $(ELF)

load: $(ELF)
	@chmod +x $(TOOLSDIR)/stm32-load
	@echo "LOAD $<"
	@$(TOOLSDIR)/stm32-load $<

clean :
	rm -rf $(OBJDIR) $(BINDIR)

debug: $(ELF) load
	@chmod +x $(TOOLSDIR)/stm32-gdb
	@chmod +x $(TOOLSDIR)/stm32-debug
	@echo "DEBUG $<"
	@$(TOOLSDIR)/stm32-debug $<

rawdebug: $(ELF) load
	@chmod +x $(TOOLSDIR)/stm32-gdb
	@echo "DEBUG $<"
	@$(TOOLSDIR)/stm32-gdb $<

minicom:
	@minicom -b9600 -D /dev/ttyACM0
