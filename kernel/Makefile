.PHONY: all
all: boot.iso

.PHONY: run
run: boot.iso
	./run.sh

.PHONY: clean
clean:
	rm -f iso_root boot.iso
	$(MAKE) -C stdlib clean
	$(MAKE) -C kernel clean
	$(MAKE) -C demo_term clean
	$(MAKE) -C shell clean
	$(MAKE) -C demo_window clean
	$(MAKE) -C demo_3d clean
	$(MAKE) -C space_invader clean

.PHONY: stdlib
stdlib:
	$(MAKE) -C stdlib

.PHONY: kernel
kernel: stdlib
	$(MAKE) -C kernel

.PHONY: demo_term
demo_term: stdlib
	$(MAKE) -C demo_term

.PHONY: shell
shell: stdlib
	$(MAKE) -C shell

.PHONY: demo_window
demo_window: stdlib
	$(MAKE) -C demo_window

.PHONY: demo_3d
demo_3d: stdlib
	$(MAKE) -C demo_3d

.PHONY: space_invader
space_invader: stdlib
	$(MAKE) -C space_invader

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v2.0-branch-binary --depth=1
	$(MAKE) -C limine

boot.iso: limine kernel shell demo_term demo_window demo_3d space_invader limine.cfg
	rm -rf iso_root
	mkdir -p iso_root
	cp kernel/kernel.elf demo_term/demo_term shell/shell demo_window/demo_window demo_3d/demo_3d space_invader/space_invader limine.cfg limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin iso_root/ 
	xorriso -as mkisofs -b limine-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table --efi-boot limine-eltorito-efi.bin -efi-boot-part --efi-boot-image --protective-msdos-label iso_root -o boot.iso
	limine/limine-install boot.iso
	rm -rf iso_root
