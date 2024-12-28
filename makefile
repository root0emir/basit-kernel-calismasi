# Makefile

NASM = nasm
GCC = gcc
LD = ld
DD = dd
MKISOFS = mkisofs

BOOTLOADER_SRC = bootloader.asm
BOOTLOADER_BIN = bootloader.bin
KERNEL_SRC = kernel.c
KERNEL_BIN = kernel.bin
OS_IMAGE = os_image.bin
ISO_FILE = os.iso

NASM_FLAGS = -f bin
GCC_FLAGS = -ffreestanding -nostdlib -nostartfiles -lgcc
LD_FLAGS = -T linker.ld


all: $(ISO_FILE)


$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
	$(NASM) $(NASM_FLAGS) -o $(BOOTLOADER_BIN) $(BOOTLOADER_SRC)


$(KERNEL_BIN): $(KERNEL_SRC)
	$(GCC) $(GCC_FLAGS) -c $(KERNEL_SRC) -o $(KERNEL_BIN)


$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $(OS_IMAGE)


$(ISO_FILE): $(OS_IMAGE)
	$(MKISOFS) -o $(ISO_FILE) -b $(OS_IMAGE) .


clean:
	rm -f $(BOOTLOADER_BIN) $(KERNEL_BIN) $(OS_IMAGE) $(ISO_FILE)


rebuild: clean all
