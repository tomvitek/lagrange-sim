import numpy as np
import pandas as pd
from common import read_sat
from matplotlib import cm
from matplotlib import pyplot as plt

D = 3.844e8
d = 4.669e6

if __name__ == "__main__":
    t, x, y = read_sat("../output/lagrange-stability.csv")
    x: np.ndarray = x
    r_start = (x[0,:]**2 + y[0,:]**2)**0.5
    r_start_min = np.nanmin(r_start)
    r_start_max = np.nanmax(r_start)
    print("r_start_min:", r_start_min)
    print("r_start_max:", r_start_max)

    plt.figure()
    for i in range(x.shape[1]):
        plt.plot(x[:,i], y[:,i], '-', color=cm.brg((r_start[i] - r_start_min) / (r_start_max - r_start_min)))

    print(x.shape[1])
    plt.plot(-d, 0, 'x', label="Earth")
    plt.plot(D -d, 0, 'x', label="Moon")
    plt.xlim(-6e8, 6e8)
    plt.ylim(-6e8, 6e8)
    plt.legend(loc="upper right")
    plt.show()