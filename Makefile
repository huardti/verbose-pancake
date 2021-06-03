include Makefile.project

CUBEDIR=test_cube

TOOLSDIR=$(CUBEDIR)/devtools

SRCDIR = src
OBJDIR = obj
BINDIR = bin

ELF = $(BINDIR)/$(PROGNAME).elf
BIN = $(BINDIR)/$(PROGNAME).bin

# OBJS = $(patsubst $(SRCDIR)/%.S,$(OBJDIR)/%.o,$(ASMS))
# OBJS += $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
# OBJS += $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(CPPSRCS))

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
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(OBJDIR)/test.map,--cref -Wl,--gc-sections


CRCOBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(CRCSRC))
CRCOBJOK = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%_ok.o,$(CRCSRC))
CRCLSTOK = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%_ok.lst,$(CRCSRC))
CRCOBJS = $(filter-out $(CRCOBJ),$(OBJS)) $(CRCOBJOK)

all: build-lib $(ELF)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(CDEPS) Makefile
	@mkdir -p $(dir $@)
	@echo "COMPILE $< => $@"
	@arm-none-eabi-gcc $(CFLAGS) -Wa,-ahlns=$(OBJDIR)/$*.lst -g -c $< -o $@
	@echo "DONE $< => $@"

OBJS = $(wildcard obj/*.o)
$(ELF): $(OBJS) Makefile
	@mkdir -p $(dir $@)
	@echo -n "LINK $@\n"
	arm-none-eabi-gcc $(OBJS) $(LDFLAGS) -o $@

build-lib: Makefile
	@echo -n "building HAL\n"
	@make -j$(nproc) -C test_cube
	@echo -n "HAL done\n"

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
