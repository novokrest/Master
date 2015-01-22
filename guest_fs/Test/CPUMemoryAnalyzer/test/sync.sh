#!/bin/bash

sudo mount -o loop Debian-Jessie-AMD64-root_fs loop_dir
sudo rm -rf loop_dir/home/test
sudo mkdir loop_dir/home/test
sudo cp -r big loop_dir/home/test/
sudo cp -r small loop_dir/home/test
sudo umount loop_dir
