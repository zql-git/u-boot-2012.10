#!/bin/sh
set -v on

rm u-boot-*

cat ../u-boot.bin > temp
cat ../u-boot.bin >> temp

split -b 300k temp
mv xaa u-boot-300k.bin
rm -f temp x*

cat ../nand_spl/u-boot-spl-16k.bin > temp
split -b 8k temp
mv xaa u-boot-8k.bin
rm -f temp x*



