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
./CPUMemoryAnalyzer         -l log_gemu.txt
./CPUMemoryAnalyzer         -l log_gemu.txt
./CPUMemoryAnalyzer         -l log_gemu.txt
./CPUMemoryAnalyzer         -l log_gemu.txt
./CPUMemoryAnalyzer         -l log_gemu.txt
./CPUMemoryAnalyzer -v      -l log_gemu.txt
./CPUMemoryAnalyzer -v      -l log_gemu.txt
./CPUMemoryAnalyzer -v      -l log_gemu.txt

sudo chmod 0666 /dev/kvm
./CPUMemoryAnalyzer         -l log_gemu_kvm.txt
./CPUMemoryAnalyzer         -l log_gemu_kvm.txt
./CPUMemoryAnalyzer         -l log_gemu_kvm.txt
./CPUMemoryAnalyzer         -l log_gemu_kvm.txt
./CPUMemoryAnalyzer         -l log_gemu_kvm.txt
./CPUMemoryAnalyzer -v      -l log_gemu_kvm.txt
./CPUMemoryAnalyzer -v      -l log_gemu_kvm.txt
./CPUMemoryAnalyzer -v      -l log_gemu_kvm.txt

