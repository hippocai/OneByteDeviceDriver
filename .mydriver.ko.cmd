cmd_/root/assignment3/q2/mydriver.ko := ld -r -m elf_x86_64 -T ./scripts/module-common.lds --build-id  -o /root/assignment3/q2/mydriver.ko /root/assignment3/q2/mydriver.o /root/assignment3/q2/mydriver.mod.o