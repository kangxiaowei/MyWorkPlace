#!/bin/bash

timestamp="1"
x="1"
y="1"
uid="1"

while true; do
    msg="$timestamp,$x,$y,$uid"
    echo $msg
    sleep 1
done
