#!/usr/bin/env bash
##
# Draco job script
##
#SBATCH --job-name=mlp_training
#SBATCH --output=mlp_training_%j.out
#SBATCH -p short
#SBATCH -N 1
#SBATCH --cpus-per-task=96
#SBATCH --time=01:00:00
#SBATCH --mail-type=all
#SBATCH --mail-user=farin.lippmann@uni-jena.de

echo "submit host:"
echo $SLURM_SUBMIT_HOST
echo "submit dir:"
echo $SLURM_SUBMIT_DIR
echo "nodelist:"
echo $SLURM_JOB_NODELIST

# activate conda env
module load tools/anaconda3/2021.05
source "$(conda info -a | grep CONDA_ROOT | awk -F ' ' '{print $2}')"/etc/profile.d/conda.sh
conda activate pytorch_src

cd $HOME/eml/ex04
export PYTHONUNBUFFERED=TRUE
python ex04.py