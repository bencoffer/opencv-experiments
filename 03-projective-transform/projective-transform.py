
input_filename = 'Greist_Building.JPG'
output_width = 1000
output_height = 850


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


# Generate output image

output_image = np.zeros((output_height,output_width,3), dtype=np.uint8)

def generate_output_image():
       
    global output_image
    
    # Get four input points of the transform
    X = quadrilateral.xdata.astype(np.float32).reshape(4,1)
    Y = quadrilateral.ydata.astype(np.float32).reshape(4,1)
    
    # Get corresponding output points of the transform
    x = np.array((0, output_width,  output_width,    0     ), dtype=np.float32).reshape(4,1)
    y = np.array((0,    0    , output_height, output_height), dtype=np.float32).reshape(4,1)
    
    # Get projective transform matrix M which maps the input points (X,Y)
    # to the output points (x,y)
    M = cv2.getPerspectiveTransform( src = np.hstack((X, Y)),
                                     dst = np.hstack((x, y)) )

    # Map input_image to output_image using the projective transform matrix M.
    # For every pixel in the output_image, OpenCV will use the inverse of M
    # to obtain the corresponding subpixel position in the input_image.
    # The input_image will then be sampled using linear interpolation.
    cv2.warpPerspective( src = input_image,
                         M = M,
                         dsize = (output_width, output_height),
                         dst = output_image,
                         flags = cv2.INTER_LINEAR )

generate_output_image()


# Figure 2 - Display Output Image
fig2 = plt.figure(2)
ax2 = fig2.add_subplot(1, 1, 1)
img2 = ax2.imshow(output_image, animated=True)
ax2.set_xticks(())
ax2.set_yticks(())
ax2.set_aspect('equal')
fig2.tight_layout(pad=0, w_pad=0, h_pad=0)


def update_figures():
    generate_output_image()
    img2.set_array(output_image)
    img2.axes.draw_artist(img2)
    img2.axes.figure.canvas.blit(img2.axes.bbox)
    
quadrilateral.on_changed(update_figures)


plt.show()


