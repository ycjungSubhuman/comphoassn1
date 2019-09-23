# -*- coding: utf-8 -*-

import numpy as np
import cv2

def align(I1, I2, search_window_radius):
    best_translation = (0,0)
    # Implement single-scale alignment search
    return best_translation

def align_multiscale(I1, I2, search_window_radius):
    best_translation = (0,0)
    # Implement single-scale alignment search
    return best_translation

imname = '00892u.tif'

# read the image
fullim = cv2.imread(imname, cv2.IMREAD_UNCHANGED)
im_depth = fullim.dtype

# convert to double matrix (might want to do this later on to same memory)
if im_depth == np.uint8:
    fullim = fullim.astype(np.float64) / 255.
else:   # TIFF images have 16-bit color information
    fullim = fullim.astype(np.float64) / 65535.

# compute the height of each part (just 1/3 of total)
height = fullim.shape[0]/3

# separate color channels
B = fullim[0:height,:]
G = fullim[height:height*2,:]
R = fullim[height*2:height*3,:]

# crop image boundaries
c = 300
B_sub = B[c:-c,c:-c]
G_sub = G[c:-c,c:-c]
R_sub = R[c:-c,c:-c]

# Align the images
# Functions that might be useful to you for aligning the images include:
# "np.sum", "np.square", "cv2.resize" with cv2.INTER_AREA, which applies pre-filtering before subsampling
# and, how to set an ROI in OpenCV-Python
aG = align_multiscale(G_sub,B_sub,128)
aR = align_multiscale(R_sub,B_sub,128)

# create a color image (3D array)
H, W = B.shape
crop_x = max(abs(aG[1]), abs(aR[1]))
crop_y = max(abs(aG[0]), abs(aR[0]))
B = B[crop_y:H-crop_y, crop_x:W-crop_x]
G = G[crop_y - aG[0]:H-crop_y - aG[0], crop_x - aG[1]:W-crop_x - aG[1]]
R = R[crop_y - aR[0]:H-crop_y - aR[0], crop_x - aR[1]:W-crop_x - aR[1]]

colorim = cv2.merge((B,G,R))

if im_depth == np.uint8:
    colorim = (colorim*255.).astype(np.uint8)
    cv2.imwrite('result_%s' % imname, colorim)
else:
    colorim = (colorim*65535.).astype(np.uint16)
    cv2.imwrite('result_%s' % imname, colorim)

