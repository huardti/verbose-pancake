CUBEDIR=test_cube
ELF=$(CUBEDIR)/build/test.elf
TOOLSDIR=devtools

all: build load

build:
	make -j$(nproc) -C test_cube

dump : $(ELF)
	@echo "DUMP $<"
	@arm-none-eabi-objdump -h -t -d $(ELF)

load: $(ELF)
	@chmod +x $(TOOLSDIR)/stm32-load
	@echo "LOAD $<"
	@$(TOOLSDIR)/stm32-load $<

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
