CC := clang -target x86_64-elf
LD := x86_64-elf-ld

CFLAGS := --std=c17 -Wall -O2 -I. -Iinclude -isystem ../stdlib/include -ffreestanding -nostdlib -fno-stack-protector -fno-pic -mno-red-zone -mcmodel=kernel -MMD -MP 
LDFLAGS := -nostdlib -static -L../stdlib -lc

OUT := obj

SRC := $(wildcard src/*.c)
ASM := $(wildcard asm/*.s)
FONT := $(wildcard fonts/*.psf)

F_OBJ := $(patsubst %.psf, $(OUT)/%.o, $(FONT))
C_OBJ := $(patsubst %.c, $(OUT)/%.o, $(SRC))
S_OBJ := $(patsubst %.s, $(OUT)/%.o, $(ASM))
DEP := $(patsubst %.c, $(OUT)/%.d, $(SRC))

.PHONY: all
all: kernel.elf

.PHONY: clean
clean:
	rm -rf kernel.elf $(OUT)

.PHONY: run
run:
	$(MAKE) -C .. run

kernel.elf: $(C_OBJ) $(S_OBJ) $(F_OBJ) linker.ld ../stdlib/libc.a
	$(LD) -T linker.ld -o $@ $(C_OBJ) $(S_OBJ) $(F_OBJ) $(LDFLAGS)

$(F_OBJ): $(OUT)/%.o: %.psf
	@mkdir -p `dirname $@`
	$(LD) -r -b binary -o $@ $<

$(C_OBJ): $(OUT)/%.o: %.c
	@mkdir -p `dirname $@`
	$(CC) $(CFLAGS) -c $< -o $@


$(S_OBJ): $(OUT)/%.o: %.s
	@mkdir -p `dirname $@`
	$(CC) -c $< -o $@

-include $(DEP)
