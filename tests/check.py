import subprocess
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

command = ["/home/shihab/src/rnd/build/tests/run_normal"]

result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
vals = result.stdout.decode('utf-8').split()
vals = np.array(list(map(float, vals)))   

sns.kdeplot(vals)
plt.savefig('normal.png')
