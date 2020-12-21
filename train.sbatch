#!/bin/bash
#SBATCH --job-name="linp"
#SBATCH --nodes=1
#SBATCH --cpus-per-task=1
#SBATCH --gres=gpu:1,VRAM:12G
#SBATCH --mem=2G
#SBATCH --time=0:05:00
#SBATCH --mail-type=ALL
#SBATCH --output=/usr/stud/linp/storage/user/linp/slurm/logs/20201221dypo.txt
#SBATCH --error=/usr/stud/linp/storage/user/linp/slurm/logs/20201221dype.txt
srun python /usr/stud/linp/storage/user/linp/DynaSLAM/src/python/Check.py
