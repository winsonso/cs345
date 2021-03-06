#!/bin/bash

# Lab 01
# Brother Jones, CS 345, Operating Systems
# Winson So

############################################################################
#
# Summary:
#    This is a bash script that will have various information about this 
# system. 
#
############################################################################
echo 'What is the CPU type and model?'
echo "    $(awk 'FNR>=31 && FNR<=31' /proc/cpuinfo | cut -c14-53)"
echo

echo 'How many processors on this system?'
echo "    $(grep -c "processor" /proc/cpuinfo) processors"
echo
    
echo 'What version of the Linux kernel is being used?'
var=$(more /proc/version | cut -d " " -f 1-3)
var2=$(more /proc/version | cut -d " " -f 14-21)
echo "    $var $var2"
echo

echo 'How long has it been since the system was last booted?'
echo "    $(uptime | cut -c 11-21)$(uptime | cut -c 21-26)"
echo

echo 'How much CPU execution time has been spent in user, system and idle modes?'
cpuT1=$(cat /proc/stat | head -n 1 | cut -d " " -f 3)
cpuT2=$(cat /proc/stat | head -n 1 | cut -d " " -f 4)
sysT=$(cat /proc/stat | head -n 1 | cut -d " " -f 5)
idleT=$(cat /proc/stat | head -n 1 | cut -d " " -f 6)
user=$(python -c "print($(($cpuT1 + $cpuT2)) /100.0)")
sys=$(python -c "print($(($sysT)) /100.0)")
idle=$(python -c "print($(($idleT)) /100.0)")
echo "    USER: $user seconds"
echo "    SYSTEM: $sys seconds"
echo "    IDLE: $idle seconds"
echo

echo 'How much memory is on the machine?'
echo "    $(grep 'MemTotal' /proc/meminfo)"
echo

echo 'How much memory is currently available?'
echo "    $(grep 'MemFree' /proc/meminfo)"
echo

echo 'How many kBytes have been read and written to the disk since the last reboot?'
echo "    Read: "$(iostat -k | grep sda | cut -d ' ' -f39)" kB"
echo "    Written: "$(iostat -k | grep sda | cut -d ' ' -f44)" kB"
echo

echo 'How many processes have been created since the last reboot?'
echo "    Processes created: $(grep 'processes' /proc/stat | cut -d " " -f 2)"
echo

echo 'How many context switches have been performed since the last reboot?'
echo "    Context switches: $(grep 'ctxt' /proc/stat | cut -d " " -f 2)"
echo

echo 'What is the current load average for the last 1, 5 and 15 minutes?'
echo "    Load average:$(uptime | cut -d ":" -f 5)"
echo
