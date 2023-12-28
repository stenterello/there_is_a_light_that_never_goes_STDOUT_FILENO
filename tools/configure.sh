#!/bin/bash

sed -i 's/# it_IT.UTF-8 UTF-8/it_IT.UTF-8 UTF-8/' /etc/locale.gen

locale-gen


export TERM=xterm-256color

export LC_ALL="it_IT.UTF-8"

cd /tmp/light
cmake -B build .
cd build
make
./light

