import sys
from argparse import ArgumentParser

import numpy as np
import pandas as pd
from common import plot_important_points, read_sat, animate_base, render_plt_video
from matplotlib import animation, cm
from matplotlib import pyplot as plt

D = 3.844e8
d = 4.669e6

if __name__ == "__main__":
    argParser = ArgumentParser(
        prog="body-movements",
        description="Visualize body movements and optionally export them as an mp4"
    )
    argParser.add_argument("inputFile")
    argParser.add_argument("--save")
    argParser.add_argument("--vid-fps", default="30")
    argParser.add_argument("--vid-dur", default="60")
    args = argParser.parse_args()
    print(args)
    filepath = args.inputFile
    vid_dur = float(args.vid_dur)
    vid_fps = float(args.vid_fps)
    
    t, x, y = read_sat(filepath)
    x: np.ndarray = x
    r_start = (x[0,:]**2 + y[0,:]**2)**0.5
    r_start_min = np.nanmin(r_start)
    r_start_max = np.nanmax(r_start)
    print("r_start_min:", r_start_min)
    print("r_start_max:", r_start_max)
    phi_start = np.angle(x[0,:] + y[0,:] * 1j)
    phi_start_min = np.nanmin(phi_start)
    phi_start_max = np.nanmax(phi_start)

    # phi-colored
    c_phi = cm.rainbow((phi_start - phi_start_min) / (phi_start_max - phi_start_min))
    # radius-colored
    c_r = cm.rainbow((r_start - r_start_min) / (r_start_max - r_start_min))

    # Plot setup
    fig, ax_r = plt.subplots(1,2)
    ax_r = plt.subplot(121)
    ax_r.set_aspect(1)
    ax_r.set_xlim([-6e8, 6e8])
    ax_r.set_ylim([-6e8, 6e8])
    ax_phi = plt.subplot(122)
    ax_phi.set_aspect(1)
    ax_phi.set_xlim([-6e8, 6e8])
    ax_phi.set_ylim([-6e8, 6e8])
    fig.set_size_inches(15, 10)

    scat_r = ax_r.scatter(x[0,:], y[0,:], s=20, color=c_r)
    scat_phi = ax_phi.scatter(x[0,:], y[0,:], s=20, color=c_phi)
        
    plt.suptitle('0.00 years')

    def animate(frame_i):
        i = int(frame_i / vid_dur / vid_fps * len(t))
        data = np.array(np.transpose(np.array((x[i,:], y[i,:]))))
        scat_r.set_offsets(data)
        scat_phi.set_offsets(data)
        
        animate_base(i, t)
        
        return scat_r,scat_phi
    
    

    for ax in (ax_r, ax_phi):
        plot_important_points(ax)
        ax.legend(loc="upper right")

    plt.xlim(-6e8, 6e8)
    plt.ylim(-6e8, 6e8)
        
    ani = animation.FuncAnimation(fig, animate, interval=int(1000 / vid_fps), frames=range(int(vid_dur * vid_fps)), repeat=True)
    if not args.save is None:
        render_plt_video(args, ani)
    plt.show()