shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ make
make -C /lib/modules/4.18.0-21-generic/build SUBDIRS=/nobackup/shenma/OS/OS_Concept_Labs/chapter-1 modules
make[1]: Entering directory '/usr/src/linux-headers-4.18.0-21-generic'
Makefile:970: "Cannot use CONFIG_STACK_VALIDATION=y, please install libelf-dev, libelf-devel or elfutils-libelf-devel"
  CC [M]  /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/simple.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/simple.mod.o
  LD [M]  /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/simple.ko
make[1]: Leaving directory '/usr/src/linux-headers-4.18.0-21-generic'
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ 
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ 
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ sudo insmod simple.ko
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ sudo rmmod simple 
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ dmesg | tail -n 10
[ 6881.283847] gcd(21,93)= 3
[ 6881.283848] HZ:250 exit jiffies: 4296612597
[ 6881.283848] time lapsed: 4
[ 7562.377955] loading kernel module
[ 7562.377956] GOLDEN RATIO PRIME: 7046029254386353131
[ 7562.377957] HZ:250 entering jiffies: 4296782851
[ 7567.893873] Removing Kernel Module
[ 7567.893876] gcd(21,93)= 3
[ 7567.893877] HZ:250 exit jiffies: 4296784229
[ 7567.893877] time lapsed: 5
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ 


shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ make
make -C /lib/modules/4.18.0-21-generic/build SUBDIRS=/nobackup/shenma/OS/OS_Concept_Labs/chapter-1 modules
make[1]: Entering directory '/usr/src/linux-headers-4.18.0-21-generic'
Makefile:970: "Cannot use CONFIG_STACK_VALIDATION=y, please install libelf-dev, libelf-devel or elfutils-libelf-devel"
  CC [M]  /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/simple.o
  CC [M]  /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/filesystem.o
  Building modules, stage 2.
  MODPOST 2 modules
  CC      /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/filesystem.mod.o
  LD [M]  /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/filesystem.ko
  CC      /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/simple.mod.o
  LD [M]  /nobackup/shenma/OS/OS_Concept_Labs/chapter-1/simple.ko
make[1]: Leaving directory '/usr/src/linux-headers-4.18.0-21-generic'

shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ ls
filesystem.c  filesystem.ko  filesystem.mod.c  filesystem.mod.o  filesystem.o  Makefile  modules.order  Module.symvers  screenoutput.txt  simple.c  simple.ko  simple.mod.c  simple.mod.o  simple.o
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ sudo insmod filesystem.ko
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ 
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ 
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ cat /proc/jiffies 
jiffies:4297294575
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ cat /proc/jiffies 
jiffies:4297295095
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ cat /proc/seconds
time lapsed since load: 15
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ cat /proc/seconds
time lapsed since load: 16
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ cat /proc/seconds
time lapsed since load: 17
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ cat /proc/seconds
time lapsed since load: 19
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ sudo rmmmod filesystem
sudo: rmmmod: command not found
shenma@shenma-VirtualBox:/nobackup/shenma/OS/OS_Concept_Labs/chapter-1$ 

