qemu-system-x86_64 -enable-kvm \
	-drive if=pflash,format=raw,file=./Build/OvmfX64/DEBUG_GCC5/FV/OVMF.fd \
	-drive format=raw,file=fat:rw:~/uefi_disk/ --net none
