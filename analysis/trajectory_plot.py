import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

D = 3.844e8
d = 4.669e6

def read_sat(path: str):
    df = pd.read_csv(path)
    N = int(len(df.columns) / 4)
    t = df.values[:,0]
    x = df.values[:,range(1, N * 4, 4)]
    y = df.values[:,range(2, N * 4, 4)]
    return t, x, y

if __name__ == "__main__":
    t, x, y = read_sat("output/satellite.csv")

    plt.figure()
    plt.plot(x, y, '-', label="trajectory")
    plt.plot(-d, 0, 'x', label="Earth")
    plt.plot(D -d, 0, 'x', label="Moon")
    plt.xlim(-6e8, 6e8)
    plt.ylim(-6e8, 6e8)
    plt.legend()
    plt.show()