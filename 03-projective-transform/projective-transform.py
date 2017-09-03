
input_filename = 'Greist_Building.JPG'

import numpy as np
import matplotlib.pyplot as plt
import cv2

import benswidgets    # local import


# Get input image
input_image = cv2.imread(input_filename, cv2.IMREAD_UNCHANGED)
input_image = cv2.cvtColor(input_image, cv2.COLOR_BGR2RGB)


# Figure 1 - Select Quadrilateral from Input Image
fig1 = plt.figure(1)
ax1 = fig1.add_subplot(1, 1, 1)
img1 = ax1.imshow(input_image)
ax1.set_xticks(())
ax1.set_yticks(())
ax1.set_aspect('equal')
quadrilateral = benswidgets.InteractiveQuadrilateral(
                                axes = ax1,
                                xdata = np.array([100, 900, 900, 100]),
                                ydata = np.array([100, 100, 900, 900]) 
                            )
fig1.tight_layout(pad=0, w_pad=0, h_pad=0)


def update_figures():
    print(quadrilateral.xdata)
    print(quadrilateral.ydata)
    print('')
    
quadrilateral.on_changed(update_figures)

plt.show()




