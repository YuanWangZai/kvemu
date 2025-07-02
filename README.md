# KVEMU

Hello Wo**RL**d!! :hand: **KVEMU** is an open-source key-value SSD emulator built on top of [FEMU](https://github.com/MoatSysLab/FEMU). This repository was opened to support KV-SSD researchers, and there are also plans to port various KV-SSD FTLs in the future. However, please note that the owner of this repository has already graduated and is maintaining it purely for leisure purposes, so responses might be a bit slow!

## :label: Citation

```
@inproceedings{anykey.asplos25,
  title={AnyKey: A Key-Value SSD for All Workload Types},
  author={Park, Chanyoung and Lee, Jungho and Liu, Chun-Yi and Kang, Kyungtae and Kandemir, Mahmut Taylan and Choi, Wonil},
  booktitle={Proceedings of the 30th ACM International Conference on Architectural Support for Programming Languages and Operating Systems, Volume 1},
  pages={47--63},
  year={2025}
}
```

## :fire: List of supported KV-SSD FTLs
* [PinK (ATC '20)](https://www.usenix.org/conference/atc20/presentation/im)
* [AnyKey (ASPLOS '25)](https://dl.acm.org/doi/10.1145/3669940.3707279)

## :rocket: Getting started

1. Make sure you have installed necessary libraries for building QEMU. The
dependencies can be installed by following instructions below:

```bash
cd ~
git clone https://github.com/chanyoung/kvemu.git
cd kvemu
mkdir build-femu
# Switch to the FEMU building directory
cd build-femu
# Copy femu script
cp ../femu-scripts/femu-copy-scripts.sh .
./femu-copy-scripts.sh .
# only Debian/Ubuntu based distributions supported
sudo ./pkgdep.sh
```

2. Compile & Install FEMU:

```bash
./femu-compile.sh
```
FEMU binary will appear as ``x86_64-softmmu/qemu-system-x86_64``

3. Prepare the VM image

[Download link](https://drive.google.com/file/d/1DJfaHnQpUn0pv0Tk2maNcmnwE8CFuKzN/view?usp=drive_link)

```bash
mkdir -p ~/images/
cd ~/images
pip install gdown
gdown --id 1DJfaHnQpUn0pv0Tk2maNcmnwE8CFuKzN
gzip -d asplos.ubuntu.qcow2.gz
```

4. After guest OS is installed, boot it with
```bash
cd ~/kvemu/build-femu
./run-pink.sh
# Or
# ./run-lksv3.sh
```

5. After accessing the Guest OS, run the workload
```bash
# Password: liu
ssh liu@localhost -p 8080

cd uNVMe; sudo NRHUGE=256 ./script/setup.sh

cd app/fio_plugin
sudo ./fio-3.3 main/etc_load.fio
sudo ./fio-3.3 main/etc_ramp.fio
sudo ./fio-3.3 main/etc_run_20.fio

# Complete the execution by shutting down the VM
sudo shutdown -h now
```

6. After the workload execution is complete, you can analyze the generated log files using the extract script.
```bash
# Assume users are in the build-femu directory
./extract.sh log
```

