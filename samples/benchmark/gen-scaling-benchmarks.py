#
# Copyright 2024 - 2025 Johannes Pekkilä
# Copyright 2024 - 2025 Aalto University
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#!/usr/bin/env python3

def gen_sbatch_preamble(nprocs, devices_per_node, account, time_limit, partition):
    ntasks_per_node = min(nprocs, devices_per_node)
    nnodes = nprocs // ntasks_per_node
    assert(ntasks_per_node * nnodes == nprocs)

    return f"""#!/usr/bin/env bash
#SBATCH --account={account}
#SBATCH --time={time_limit}
#SBATCH --partition={partition}
#SBATCH --gpus-per-node={ntasks_per_node}
#SBATCH --ntasks-per-node={ntasks_per_node}
#SBATCH --nodes={nnodes}
"""

def gen_srun_command(cpu_bind = ""):
    return f'srun --cpu-bind="{cpu_bind}"' if cpu_bind else "srun"

def gen_run_information():
    return """
module list
cmake -LAH >> system_info-$SLURM_JOB_ID.txt
"""

def gen_run_command():
    return f"""./benchmark $SLURM_JOB_ID
"""


class System:
    def __init__(self, account, partition, devices_per_node, modules, env_vars, cpu_bind):
        self.account = account
        self.partition = partition
        self.devices_per_node = devices_per_node
        self.modules = modules
        self.env_vars = env_vars
        self.cpu_bind = cpu_bind

    def gen_preamble(self, nprocs, time_limit):
        return gen_sbatch_preamble(nprocs, self.devices_per_node, self.account, time_limit, self.partition) + \
               self.modules + \
               self.env_vars + \
               gen_run_information()
    
    def gen_srun_command(self, nprocs):
        if nprocs >= self.devices_per_node:
            return gen_srun_command(self.cpu_bind)
        else:
            return gen_srun_command()


lumi = System(
    account = "project_462001062",
    # partition = "dev-g",
    partition = "standard-g",
    devices_per_node = 8,
    modules = """
# Modules
module load PrgEnv-cray
module load craype-accel-amd-gfx90a
module load rocm
module load cray-python
module load cray-hdf5
module load LUMI/24.03 buildtools/24.03
module load craype-accel-amd-gfx90a # Must be loaded after LUMI/24.03
""",
    env_vars = """
# Environment variables
export MPICH_GPU_SUPPORT_ENABLED=1
""",
    cpu_bind = "map_cpu:33,41,49,57,17,25,1,9",
)

def gen_benchmark(system, nprocs, time_limit):
    return system.gen_preamble(nprocs, time_limit) + \
           f'{system.gen_srun_command(nprocs)} {gen_run_command()}'
        
def gen_strong_scaling_benchmarks(system):
    
    time_limit = "00:15:00"

    nprocs = 1
    max_nprocs = 512
    while nprocs <= max_nprocs:
        with open(f'bm-mhd-strong-scaling-{nprocs}.sh', 'w') as f:
            print(gen_benchmark(system, nprocs, time_limit), file=f)
        nprocs *= 2

gen_strong_scaling_benchmarks(lumi)

