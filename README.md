# HoleFiller
Home assignmrnt by Lightricks

## Description

This library fills holes in images. Steps:

Convert the source image to a grayscale format by taking the mean value of (r,g,b) values.
Normalises color values from range [0-255] to [0.0-1.0]
Assign -1 value to every pixel where defined as hole in the mask
Find the the boundary of the hole (for either 8 or 4 connectivity)
Applies the algorithm for every pixel in the hole
Save the filled image in source image directory
NOTE: Mask image is the same height and width of the source image. A pixel is defined as 'hole pixel' iff its color value in the mask satisfies value > 0.5
