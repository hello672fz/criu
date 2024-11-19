#!/bin/bash

rm -rf /tmp/checkpoint8
mkdir /tmp/checkpoint8

#/home/lfz/workplace/criu-v3/criu/criu dump -t `pidof simple_loop` -D /tmp/checkpoint1 -j -v4
#/home/lgw/criu/criu/criu dump -t `pidof simple_loop` -D /tmp/checkpoint1 -j -v4

sudo criu dump -t `pidof simple_loop` -D /tmp/checkpoint8 -j -v4