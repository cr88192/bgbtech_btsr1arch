gcc -o bin2rom.elf bin2rom.c -g

./bin2rom.elf ../../tst_jx2boot.bin

./bin2rom.elf ../../tst_jx2boot_uc.bin bootrom_uc
./bin2rom.elf ../../tst_jx2boot_rbd.bin bootrom_rbd

cp bootrom*.txt ringbus
