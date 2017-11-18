#!/bin/bash

# Lab LinuxInfo
# Brother Jones, CS 345, Operating Systems
# Stephen Jones

############################################################################
#
# Summary:
#    This program will display various information about the operatoin system
#  and its usage.
#
#
############################################################################


echo  "What is the CPU type and model?"
echo -n -e "    "
grep "model name" /proc/cpuinfo |head -n 1| cut -c 14-53
echo ""

echo "How many processors on this system?"
echo -n -e "    "
grep "cpu cores" /proc/cpuinfo | head -n 1 | cut -c 12-20 | xargs echo -n 
echo -e " processors\n"

echo "What version of the Linux kernel is being used?"
echo -n -e "    Linux version "
uname -r -v
echo ""

echo "How long has it been since the system was last booted?"
echo -n -e "    "
uptime | cut -c 11-26
echo ""


echo "How much CPU execution time has been spent in user, system and idle modes?"
echo -n -e "    "
TOTAL_TIME=$(cut -c 1-10 /proc/uptime)
USER_TIME=$(iostat | sed '4q;d' | cut -c 12-15)
SYSTEM_TIME=$(iostat | sed '4q;d' | cut -c 28-31)
IDLE_TIME=$(iostat | sed '4q;d' | cut -c 51-54)
USER=$(echo "scale=2;$USER_TIME * $TOTAL_TIME" |bc)
SYSTEM=$(echo "scale=2;$SYSTEM_TIME * $TOTAL_TIME" | bc)
IDLE=$(echo "scale=2;$IDLE_TIME * $TOTAL_TIME" | bc)
echo "USER: $USER seconds"
echo -e "    SYSTEM: $SYSTEM seconds"
echo -e "    IDLE: $IDLE seconds"
echo ""

echo "How much memory is on the machine?"
echo -n -e "    "
grep MemTotal: /proc/meminfo
echo ""

echo "How much memory is currently available?"
echo -n -e "    "
grep MemFree: /proc/meminfo
echo ""

echo "How many kBytes have been read and written to the disk since the last reboot?"
echo -n -e "    Read: "
iostat |grep sda | cut -c 54-59 | xargs echo -n
echo " kB"
echo -n -e "    Written: "
iostat | grep sda | cut -c 65-70 | xargs echo -n
echo " kB"
echo ""

echo "How many processes have been created since the last reboot?"
echo -n -e "    Processes created: "
grep processes /proc/stat | cut -c 11-16
echo ""

echo "How many context switches have been performed since the last reboot?"
echo -n -e "    Context switches: "
grep ctxt /proc/stat | cut -c 6-15
echo ""

echo "What is the current load average for the last 1, 5 and 15 minutse?"
echo -n -e "    "
uptime | cut -c 40-70
echo ""

