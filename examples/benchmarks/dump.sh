#!/bin/bash

rm -rf /tmp/checkpoint3
mkdir /tmp/checkpoint3

#/home/lfz/workplace/criu-v3/criu/criu dump --shell-job -t `pidof python` -D /tmp/checkpoint3  > fl_dump.log 2>&1
sudo /home/lfz/workplace/criu-v3/criu/criu dump --shell-job -t $(pgrep xz_s_base.mytes) -D /tmp/checkpoint3 > 657_dump.log 2>&1

