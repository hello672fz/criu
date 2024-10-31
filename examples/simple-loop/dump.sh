#!/bin/bash

rm -rf /tmp/checkpoint1
mkdir /tmp/checkpoint1

#/home/lfz/workplace/criu-v3/criu/criu dump -t `pidof simple_loop` -D /tmp/checkpoint1 -j -v4
#/home/lgw/criu/criu/criu dump -t `pidof simple_loop` -D /tmp/checkpoint1 -j -v4

sudo ../../criu/criu dump -t `pidof simple_loop` -D /tmp/checkpoint1 -j -v4
