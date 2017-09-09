
#######################################################
# Script Parameters
#######################################################

scientific_library = 'OpenCV'    # Set this to 'OpenCV' or 'SciPy'
input_filename = 'Greist_Building.JPG'
output_width = 1000
output_height = 850



#######################################################
# Imports
#######################################################

import numpy as np
import matplotlib.pyplot as plt
import benswidgets    # local import

if scientific_library == 'OpenCV':
    import cv2
    
elif scientific_library == 'SciPy':
    import scipy.ndimage
    import scipy.misc



#######################################################
# Get Input Image
#######################################################

if scientific_library == 'OpenCV':
    input_image = cv2.imread(input_filename, cv2.IMREAD_UNCHANGED)
    input_image = cv2.cvtColor(input_image, cv2.COLOR_BGR2RGB)
    
elif scientific_library == 'SciPy':
    input_image = scipy.misc.imread(input_filename)



#######################################################
# Figure 1 - Select Quadrilateral from Input Image
#######################################################

fig1 = plt.figure(1)
fig1.canvas.set_window_title('Figure 1 - Select Quadrilateral from Input Image')
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



#################################################
# Generate Output Image
#################################################

output_image = np.zeros((output_height,output_width,3), dtype=np.uint8)

def generate_output_image():
    """Get projective transform, apply it to input_image,
       and save the result to output_image.
       This function should be called immediately after it's definition
       and whenever the user makes changes that affect this function."""
       
    global output_image
    
    # Get four input points of the transform
    X = quadrilateral.xdata.astype(np.float32).reshape(4,1)
    Y = quadrilateral.ydata.astype(np.float32).reshape(4,1)
    
    # Get corresponding output points of the transform
    x = np.array((0, output_width,  output_width,    0     ), dtype=np.float32).reshape(4,1)
    y = np.array((0,    0    , output_height, output_height), dtype=np.float32).reshape(4,1)
    
    if scientific_library == 'OpenCV':
        
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
        
    elif scientific_library == 'SciPy':
        
        # Get projective transform matrix M which maps the output points (x,y)
        # to the input points (X,Y).
        # Todo: Add a description of the math.
        one = np.ones((4,1), dtype=np.float32)
        zero = np.zeros((4,1), dtype=np.float32)
        A = np.vstack((
                np.hstack((   x ,    y ,   one,  zero,  zero,  zero,  -x*X,  -y*X )) ,
                np.hstack(( zero,  zero,  zero,    x ,    y ,   one,  -x*Y,  -y*Y ))
                ))
        b = np.vstack((
                X,
                Y
                ))
        m = np.linalg.solve(A, b)    # solves A @ m = b for m
        M = np.append(m, 1.0).reshape(3,3)

        # Map input_image to output_image using projective transform matrix M.
        # For every pixel in the output_image, we will use matrix M
        # to obtain the corresponding position in the input_image.
        # The input_image will then be sampled at this position using linear interpolation.
        output_coordinates_x, output_coordinates_y = \
            np.meshgrid(np.arange(0,output_width), np.arange(0,output_height), indexing='xy')
        
        output_coordinates = np.vstack((
                                 output_coordinates_x.ravel(),
                                 output_coordinates_y.ravel(),
                                 np.ones(output_width*output_height)
                                 ))
        
        # Apply the projective transform M
        temp = M.dot(output_coordinates)
        input_coordinates_x = temp[0,:] / temp[2,:]
        input_coordinates_y = temp[1,:] / temp[2,:]
        
        # Put input coordinates into an array with shape (2, output_height, output_width)
        # This is the (strange) shape required by the map_coordinates function.
        coords = np.array((
                     input_coordinates_y.reshape(output_height, output_width),
                     input_coordinates_x.reshape(output_height, output_width)
                     ))
        
        # Subsample the input_image at the coordinates given by coords using linear interpolation.
        # We will process each channel (0=red, 1=green, 2=blue) separately.
        output_image[:,:,0] = scipy.ndimage.map_coordinates(input_image[:,:,0], coords, order=1)
        output_image[:,:,1] = scipy.ndimage.map_coordinates(input_image[:,:,1], coords, order=1)
        output_image[:,:,2] = scipy.ndimage.map_coordinates(input_image[:,:,2], coords, order=1)
        


generate_output_image()



#################################################
# Figure 2 - Display Output Image
#################################################

fig2 = plt.figure(2)
fig2.canvas.set_window_title('Figure 2 - Output Image')
ax2 = fig2.add_subplot(1, 1, 1)
img2 = ax2.imshow(output_image, animated=True)
ax2.set_xticks(())
ax2.set_yticks(())
ax2.set_aspect('equal')
fig2.tight_layout(pad=0, w_pad=0, h_pad=0)



#################################################
# Event Handlers
#################################################

def update_figures():
    generate_output_image()
    img2.set_array(output_image)
    img2.axes.draw_artist(img2)
    img2.axes.figure.canvas.blit(img2.axes.bbox)
    
quadrilateral.on_changed(update_figures)



#################################################
# GUI Loop
#################################################

# Call plt.show() to display all figures and handle user interactivity.
# This is a blocking call; this script will hault here until the user closes all windows.
plt.show()



#################################################
# Save the Output Image
#################################################

if scientific_library == 'OpenCV':
    output_image = cv2.cvtColor(output_image, cv2.COLOR_RGB2BGR)
    cv2.imwrite('output_image.jpg', output_image)

elif scientific_library == 'SciPy':
    scipy.misc.imsave('output_image.jpg', output_image)

