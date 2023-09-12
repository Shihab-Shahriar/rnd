import subprocess
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from datetime import datetime
import os

# Get current date and time
now = datetime.now()

# Format and print it
print("Current date and time:", now.strftime("%Y-%m-%d %H:%M:%S"))

BUILD_DIR = "/home/shihab/src/rnd/build"
RES_DIR = "/home/shihab/src/rnd/build/results"


procs = []
# create results directory if not exists
os.makedirs(RES_DIR, exist_ok=True)


# launch practrand multistream tests
PRACT_RND_EXEC = "/home/shihab/codes/PractRand/RNG_test"

for gen in ["philox", "tyche", "threefry", "squares"]:
    command  = f"{BUILD_DIR}/tests/pract_rand_multi {gen} | {PRACT_RND_EXEC} stdin32 -multithreaded -tlmax 16GB > {RES_DIR}/practrandm_{gen}.txt"

    p = subprocess.Popen(command, shell=True)
    procs.append(p)



# launch TESTU01 tests
LEVEL = "small"

for gen in ["philox", "tyche", "threefry", "squares"]:
    command  = f"{BUILD_DIR}/tests/testu01_multi {gen} {LEVEL} > {RES_DIR}/testu01m_{gen}.txt"
    print(command)
    p = subprocess.Popen(command, shell=True)
    procs.append(p)

# Wait for them to finish
for p in procs:
    p.communicate()
    now = datetime.now()
    print("Current date and time:", now.strftime("%Y-%m-%d %H:%M:%S"))
