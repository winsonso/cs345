#!/bin/bash

# Lab LinuxInfo
# Brother Jones, CS 345, Operating Systems
# David Brewerton

############################################################################
#
# Summary:
#    This program will summarize a lot of information about Linux and the
#	computer it runs on.
#
############################################################################

echo -e "What is the CPU type and model?"
echo -e "\t"$(grep -m1 "model name" /proc/cpuinfo | cut -c14-60)
echo -n -e "\n"
echo -e "How many processors on this system?"
echo -e "\t"$( echo $(cat /proc/cpuinfo | grep -c "model name"))" processors"
echo -n -e "\n"
echo -e "What version of the Linux kernel is being used?"
echo -e "\t"$(uname -srv)
echo -n -e "\n"
echo "How much CPU execution time has been spent in user, system and idle modes?"
echo -e "\t USER: "$(cat /proc/stat | grep -m1 cpu | cut -c6-12)
echo -e "\t SYSTEM: "$(cat /proc/stat | grep -m1 cpu | cut -c19-24)
echo -e "\t IDLE: "$(cat /proc/stat | grep -m1 cpu | cut -c26-33)
echo -n -e "\n"
echo -e "How much memory is on the machine?"
echo -e "\tMemTotal"$(free | grep Mem | cut -c4-20)
echo -n -e "\n"
echo -e "How much memory is currently available?"
echo -e "\t MemTotal"$(free | grep Mem | cut -c33-43)
echo -n -e "\n"
echo -e "How many kBytes have been read and written to the disk since the last reboot?"
echo -e "\tRead: "$( echo $(iostat -k | grep sda | cut -c54-59))" kb"
echo -e "\tWritten: "$( echo $(iostat -k | grep sda | cut -c65-70))" kb"
echo -n -e "\n"
echo -e "How many processes have been created since the last reboot?"
echo -e "\tProcesses created: "$(cat /proc/stat | grep processes | cut -c11-20)
echo -n -e "\n"
echo -e "How many context switches have been performed since the last reboot?"
echo -e "Context switches: "$(grep ctxt /proc/stat | cut -c6-13)
echo -n -e "\n"
echo -e "What is the current load average for the last 1, 5 and 15 minutes?"
echo -e "Load Average: "$(uptime | cut -c54-69)
