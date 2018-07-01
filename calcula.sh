nasm -f elf -o calculadora.o calculadora.asm
ld -m elf_i386 -o calculadora calculadora.o
./calculadora