#!/bin/bash

rm -rf /tmp/checkpoint-loop
mkdir /tmp/checkpoint-loop

sudo ../criu/criu dump -t `pidof simple_loop` -D /tmp/checkpoint-loop -j -v4