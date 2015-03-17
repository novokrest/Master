#!/bin/bash

./CPUMemoryAnalyzer    -uml -l log_uml.txt
./CPUMemoryAnalyzer    -uml -l log_uml.txt
./CPUMemoryAnalyzer    -uml -l log_uml.txt
./CPUMemoryAnalyzer    -uml -l log_uml.txt
./CPUMemoryAnalyzer    -uml -l log_uml.txt
./CPUMemoryAnalyzer -v -uml -l log_uml.txt
./CPUMemoryAnalyzer -v -uml -l log_uml.txt
./CPUMemoryAnalyzer -v -uml -l log_uml.txt

sudo chmod 0000 /dev/kvm
./CPUMemoryAnalyzer         -l log_qemu.txt
./CPUMemoryAnalyzer         -l log_qemu.txt
./CPUMemoryAnalyzer         -l log_qemu.txt
./CPUMemoryAnalyzer         -l log_qemu.txt
./CPUMemoryAnalyzer         -l log_qemu.txt
./CPUMemoryAnalyzer -v      -l log_qemu.txt
./CPUMemoryAnalyzer -v      -l log_qemu.txt
./CPUMemoryAnalyzer -v      -l log_qemu.txt

sudo chmod 0666 /dev/kvm
./CPUMemoryAnalyzer         -l log_qemu_kvm.txt
./CPUMemoryAnalyzer         -l log_qemu_kvm.txt
./CPUMemoryAnalyzer         -l log_qemu_kvm.txt
./CPUMemoryAnalyzer         -l log_qemu_kvm.txt
./CPUMemoryAnalyzer         -l log_qemu_kvm.txt
./CPUMemoryAnalyzer -v      -l log_qemu_kvm.txt
./CPUMemoryAnalyzer -v      -l log_qemu_kvm.txt
./CPUMemoryAnalyzer -v      -l log_qemu_kvm.txt

