import numpy as np
import pandas as pd
from common import read_sat
from matplotlib import animation, cm
from matplotlib import pyplot as plt

D = 3.844e8
d = 4.669e6

if __name__ == "__main__":
    t, x, y = read_sat("output/lagrange-stability-long.csv")
    x: np.ndarray = x
    r_start = (x[0,:]**2 + y[0,:]**2)**0.5
    r_start_min = np.nanmin(r_start)
    r_start_max = np.nanmax(r_start)
    print("r_start_min:", r_start_min)
    print("r_start_max:", r_start_max)
    phi_start = np.angle(x[0,:] + y[0,:] * 1j)
    # phi-colored
    c_phi = cm.rainbow((phi_start + np.pi) / (2 * np.pi))
    # radius-colored
    c_r = cm.rainbow((r_start - r_start_min) / (r_start_max - r_start_min))
    fig, ax_r = plt.subplots(1,2)
    ax_r = plt.subplot(121)
    ax_phi = plt.subplot(122)
    fig.set_size_inches(15, 10)
    scat_r = ax_r.scatter(x[0,:], y[0,:], s=20, color=c_r)
    scat_phi = ax_phi.scatter(x[0,:], y[0,:], s=20, color=c_phi)
    ax_r.set_aspect('equal')
    ax_phi.set_aspect('equal')

    def animate(i):
        data = np.array(np.transpose(np.array((x[i,:], y[i,:]))))
        scat_r.set_offsets(data)
        scat_phi.set_offsets(data)
        return scat_r,scat_phi
    
    def anim_save_callback(current_frame: int, total_frames: int):
        print(current_frame / total_frames * 100, "%")

    for ax in (ax_r, ax_phi):
        ax.scatter(-d, 0, s=120, label="Earth")
        ax.scatter(D -d, 0, s=80, label="Moon")
        ax.legend(loc="upper right")

    plt.xlim(-6e8, 6e8)
    plt.ylim(-6e8, 6e8)
        
    ani = animation.FuncAnimation(fig, animate, interval=30, frames=range(x.shape[0]), repeat=True)
    FFwriter = animation.FFMpegWriter(fps=30)
    # ani.save("output/lagrange-stability-cr.mp4", writer=FFwriter, dpi=150, progress_callback=anim_save_callback)
    plt.show()