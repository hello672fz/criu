#!/bin/bash

echo 3 > /proc/sys/vm/drop_caches
while true; do
    pid=$(ps aux | grep "/opt/venv/bin/python3 function.py" | grep -v grep | awk '{print $2}')
    if [ -n "$pid" ]; then
        #damo start $pid
        #damo show
        #sudo perf record -F 3000 -e page-faults -p $pid && sudo perf report >> monitor.log
        damo record $pid
        #perf stat -d -p $pid >> monitor.log
    fi
done 