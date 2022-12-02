#!/bin/bash

cd temp3
tar xvf linux-5.19.9.tar.xz

cd linux-5.19.9
make mrproper
wget https://raw.githubusercontent.com/plon-Susk7/Config-File/main/config-rev-9-gold
mv config-rev-9-gold .config

make -j 6
