#!/usr/bin/bash

#    /usr/qemu/qemu-system-x86_64 \
#    -display none \
#    -m 500 \
#    -no-reboot \
#    -rtc driftfix=slew \
#    -no-kvm-pit-reinjection \
#    -kernel /var/tmp/.guestfs-1000/appliance.d/kernel \
#    -initrd /var/tmp/.guestfs-1000/appliance.d/initrd \
#    -drive file=/home/novokrestWin/Master/guest_fs/test_data/appliance.d/disk.img,cache=writeback,format=raw,id=hd0,if=none \
#    -device virtio-blk-pci,drive=hd0 \
#    -drive file=/var/tmp/.guestfs-1000/appliance.d/root,snapshot=on,id=appliance,cache=unsafe,if=none \
#    -device virtio-blk-pci,drive=appliance \
#    -device virtio-serial-pci \
#    -serial stdio \
#    -chardev socket,host=localhost,port=7777,id=channel0 \
#    -device virtserialport,chardev=channel0,name=org.libguestfs.channel.0 \
#    -append 'panic=1 console=ttyS0 udevtimeout=6000 udev.event-timeout=6000 no_timer_check acpi=off printk.time=1 cgroup_disable=memory root=/dev/vdb selinux=0 guestfs_verbose=1 TERM=xterm'

    /usr/qemu/qemu-system-x86_64 \
    -m 500 \
    -no-reboot \
    -rtc driftfix=slew \
    -no-kvm-pit-reinjection \
    -kernel C:/cygwin64/var/tmp/.guestfs-1000/appliance.d/kernel \
    -initrd C:/cygwin64/var/tmp/.guestfs-1000/appliance.d/initrd \
    -drive file=C:/cygwin64/home/novokrestWin/Master/guest_fs/test_data/appliance.d/disk.img,cache=writeback,format=raw,id=hd0,if=none \
    -device virtio-blk-pci,drive=hd0 \
    -drive file=C:/cygwin64/var/tmp/.guestfs-1000/appliance.d/root,snapshot=on,id=appliance,cache=unsafe,if=none \
    -device virtio-blk-pci,drive=appliance \
    -device virtio-serial-pci \
    -serial stdio \
    -append 'panic=1 console=ttyS0 udevtimeout=6000 udev.event-timeout=6000 no_timer_check acpi=off printk.time=1 cgroup_disable=memory root=/dev/vdb selinux=0 guestfs_verbose=1 TERM=xterm'


#     /usr/qemu/qemu-system-x86_64 \
#    -display none \
#    -m 500 \
#    -no-reboot \
#    -rtc driftfix=slew \
#    -no-kvm-pit-reinjection \
#    -kernel kernel \
#    -initrd initrd \
#    -drive file=Fedora20-AMD64-root_fs,cache=writeback,format=raw,id=hd0,if=none \
#    -device virtio-blk-pci,drive=hd0 \
#    -drive file=root,snapshot=on,id=appliance,cache=unsafe,if=none \
#    -device virtio-blk-pci,drive=appliance \
#    -device virtio-serial-pci \
#    -serial stdio \
#    -chardev socket,id=channel0,host=localhost,port=7777 \
#    -chardev socket,path=C:/cygwin64/tmp/guestfsd_sock,id=channel0 \
#    -device virtserialport,chardev=channel0,name=org.libguestfs.channel.0 \
#    -append 'panic=1 console=ttyS0 udevtimeout=6000 udev.event-timeout=6000 no_timer_check acpi=off printk.time=1 cgroup_disable=memory root=/dev/vdb selinux=0 guestfs_verbose=1 TERM=xterm'



