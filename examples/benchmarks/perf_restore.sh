#!/bin/bash

#perf stat -r 10 -d /home/lfz/workplace/criu-v3/criu/criu restore --shell-job -D /tmp/checkpoint >> restore.log 2>&1
perf stat -r 5 -d /home/lfz/workplace/criu-v3/criu/criu restore --shell-job -D /tmp/checkpoint3 | tee -a restore.txt
 
# -d -vvv -o restore.log