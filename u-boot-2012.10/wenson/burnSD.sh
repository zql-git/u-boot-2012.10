#!/bin/bash
#
# Created by qinglin.Zhang
# Date: 2021/08/31 
#
 
SDCARD=/dev/sdb
SPL_BOOT=u-boot-8k.bin
UBOOT=u-boot-300k.bin

if [ `whoami` != "root" ];then
    echo "Need root permision to execute the script!"
    exit
fi


echo "SD Card Writer program V1.0"
echo "S1 = $1  S2 = $2 "

if [ "$1" != "" ]; then
    SDCARD=$1
fi

if [ "$2" == "sd" ]; then
    RES_SZ=1
else
    RES_SZ=1025
fi

echo "sd/mmc: $SDCARD  $RES_SZ"
if [ ! -b "$SDCARD" ]; then
cat << EOF
no device found

Usage: $0 <sd-dev> [sd-type]
     defalutly, <sd-dev> is /dev/sdb, sd-type is "sdhc" in ("sdhc","sd")
EOF
    exit 1        
fi

# Check SDCARD Sectors
# SDCARD_SECTORS=`fdisk -l -u $SDCARD | grep sectors | head -n 1 | \
#                cut -d',' -f4 | cut -d' ' -f3`
 
SDCARD_SECTORS=`cat /sys/block/${SDCARD##*/}/size`
 
# unit: sectors -512 bytes
SECTOR_SIZE=512

EFUSE_SZ=1
BL1_SZ=16
BL2_SZ=600
ENV_SZ=32
 
SIZE_FAT=$(($SDCARD_SECTORS-$FL210_FREE-$FL210_FREE-2))
 
START_BL1_POS=$(($SDCARD_SECTORS-$RES_SZ-$EFUSE_SZ-$BL1_SZ))
START_BL2_POS=$(($START_BL1_POS-$ENV_SZ-$BL2_SZ))
 
print_success()
{
    if [ "$1" == 0 ]; then
        echo "success"
    else
        echo "failed"
        exit -1
    fi
}
 
# Ask for correct calculation
cat <<EOF
sd/mmc device  : $SDCARD
byte per sector: $SECTOR_SIZE
total sectors  : $SDCARD_SECTORS
 
fusing sd/mmc acticon in the unit: sector
EOF
#echo "Do you want to continue(yes/no): "
#read ANS
#if [ "$ANS" != "yes" ]; then
#    exit -1
#fi
 
#size 1M
# echo -n "mmc.bin : "
# dd bs=512 seek=1 if=/dev/zero of=$SDCARD count=2048 > /dev/null 2>&1
# dd bs=512 seek=1 if=./mmc.bin of=$SDCARD > /dev/null 2>&1
 
echo $SDCARD

echo -n "write bl1 to sd/mmc at offset: $START_BL1_POS size: $BL1_SZ ..."
dd bs=512 seek=$START_BL1_POS if=/dev/zero of=$SDCARD count=$BL1_SZ  > /dev/null 2>&1
dd bs=512 seek=$START_BL1_POS if=$SPL_BOOT of=$SDCARD count=$BL1_SZ  > /dev/null 2>&1
print_success "$?"
 
echo -n "write bl2 to sd/mmc at offset: $START_BL2_POS size: $BL2_SZ ..."
dd bs=512 seek=$START_BL2_POS if=/dev/zero of=$SDCARD count=$BL2_SZ > /dev/null 2>&1
dd bs=512 seek=$START_BL2_POS if=$UBOOT of=$SDCARD count=$BL2_SZ  > /dev/null 2>&1


print_success "$?"
 
sync
exit  0

