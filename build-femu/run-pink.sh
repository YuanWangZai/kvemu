#!/bin/bash
# Huaicheng Li <huaicheng@cs.uchicago.edu>
# Run FEMU as a black-box SSD (FTL managed by the device)

# image directory
IMGDIR=/opt/work/images
# Virtual machine disk image
OSIMGF=$IMGDIR/asplos.ubuntu.qcow2

if [[ ! -e "$OSIMGF" ]]; then
	echo ""
	echo "VM disk image couldn't be found ..."
	echo "Please prepare a usable VM image and place it as $OSIMGF"
	echo "Once VM disk image is ready, please rerun this script again"
	echo ""
	exit
fi

sudo x86_64-softmmu/qemu-system-x86_64 \
    -name "FEMU-BBSSD-VM" \
    -enable-kvm \
    -cpu host \
    -smp 16 \
    -m 4G \
    -device virtio-scsi-pci,id=scsi0 \
    -device scsi-hd,drive=hd0 \
    -drive file=$OSIMGF,if=none,aio=native,cache=none,format=qcow2,id=hd0 \
    -device femu,devsz_mb=65536,multipoller_enabled=0,femu_mode=7,kv_learned=off \
    -net user,hostfwd=tcp::8080-:22 \
    -net nic,model=virtio \
    -nographic \
    -qmp unix:./qmp-sock,server,nowait 2>&1 | tee log
    #-qmp unix:./qmp-sock,server,nowait 2>&1 1>/tmp/qemu-log
