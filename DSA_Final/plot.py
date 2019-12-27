import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("output.txt")
num_particles = 100000

data_reshape = data.reshape(-1, num_particles, 4)
print ("loaded and ready")
for i in data_reshape:
    a = np.sqrt(i[:,2]**2 + i[:,3]**2)
    plt.hist(a[a<10000], bins=50)
    plt.pause(1e-3)
    plt.clf()

# for i in data_reshape:
#     plt.xlim(-1,1)
#     plt.ylim(-1,1)
#     plt.scatter(i[1,0], i[1,1])
#     plt.pause(1e-3)
#     plt.clf()