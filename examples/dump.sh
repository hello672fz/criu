#!/bin/bash

rm -rf /tmp/checkpoint
mkdir /tmp/checkpoint

sudo ../criu/criu dump -t `pidof simple_loop` -D /tmp/checkpoint -j -v4