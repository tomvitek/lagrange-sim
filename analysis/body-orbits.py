import sys
from argparse import ArgumentParser

import numpy as np
import pandas as pd
from common import plot_important_points, read_sat
from matplotlib import cm
from matplotlib import pyplot as plt

if __name__ == "__main__":
    argParser = ArgumentParser(
        prog="body-movements",
        description="Visualize body movements and optionally export them as an mp4"
    )
    argParser.add_argument("inputFile")
    argParser.add_argument("--save")
    argParser.add_argument("--x-range", default="-6e8;6e8")
    argParser.add_argument("--y-range", default="-6e8;6e8")
    args = argParser.parse_args()
    print(args)

    filepath = args.inputFile

    t, x, y = read_sat(filepath)
    x: np.ndarray = x
    r_start = (x[0,:]**2 + y[0,:]**2)**0.5
    r_start_min = np.nanmin(r_start)
    r_start_max = np.nanmax(r_start)
    print("r_start_min:", r_start_min)
    print("r_start_max:", r_start_max)

    plt.figure()
    plt_xrange_from = float(args.x_range.split(";")[0])
    plt_xrange_to = float(args.x_range.split(";")[1])
    plt_yrange_from = float(args.y_range.split(";")[0])
    plt_yrange_to = float(args.y_range.split(";")[1])
    plt.xlim(plt_xrange_from, plt_xrange_to)
    plt.ylim(plt_yrange_from, plt_yrange_to)
    for i in range(x.shape[1]):
        plt.plot(x[:,i], y[:,i], '-', color=cm.brg((r_start[i] - r_start_min) / (r_start_max - r_start_min)))

    print(x.shape[1])
    ax = plt.gca()
    plot_important_points(ax)
    
    plt.xlabel("x [m]")
    plt.ylabel("y [m]")
    plt.legend(loc="upper right")
    ax.set_aspect(1)
    plt.show()