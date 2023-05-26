Draco workflow
==============
salloc --cpus-per-task=96 -p short -t 03:00:00
cd mini_dnn

module load use.intel-oneapi
module load compiler/2023.0.0

git clone https://github.com/libxsmm/libxsmm.git
cd libxsmm
make BLAS=0 -j
cd ..

mkdir catch2
wget https://github.com/catchorg/Catch2/releases/download/v2.13.9/catch.hpp -O ./catch2/catch.hpp

module load tools/python
export PATH=/cluster/spack/opt/spack/linux-almalinux8-cascadelake/gcc-10.2.1/anaconda3-2021.05-trsyvrlok6matyfg34yk4crlghaenpgp/bin/:${PATH}
source "$(conda info -a | grep CONDA_ROOT | awk -F ' ' '{print $2}')"/etc/profile.d/conda.sh
conda activate pytorch_src

CXX=icpc make all -j