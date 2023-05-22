Draco workflow
==============
mkdir catch2
wget https://github.com/catchorg/Catch2/releases/download/v2.13.9/catch.hpp -O ./catch2/catch.hpp

module load tools/python
export PATH=/cluster/spack/opt/spack/linux-almalinux8-cascadelake/gcc-10.2.1/anaconda3-2021.05-trsyvrlok6matyfg34yk4crlghaenpgp/bin/:${PATH}
source "$(conda info -a | grep CONDA_ROOT | awk -F ' ' '{print $2}')"/etc/profile.d/conda.sh
conda activate pytorch_src

module load compiler/gcc
make all
