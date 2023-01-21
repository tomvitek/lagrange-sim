import pandas as pd
from matplotlib import pyplot as plt

MOON_A = 3.844e8
MOON_M = 7.3482e22
EARTH_M = 5.974e24

L12_DST = (MOON_A * pow(MOON_M / 3 / EARTH_M, 1.0/3.0))

def read_sat(path: str):
    df = pd.read_csv(path)
    N = int(len(df.columns) / 4)
    t = df.values[:,0]
    x = df.values[:,range(1, N * 4, 4)]
    y = df.values[:,range(2, N * 4, 4)]
    return t, x, y