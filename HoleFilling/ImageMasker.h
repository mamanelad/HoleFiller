#ifndef IMAGEMASKER_H
#define IMAGEMASKER_H

#include <opencv2/opencv.hpp>

#define MASK_THRESHOLD 0.5
#define HOLE_VALUE -1
using namespace cv;

/**
 * The ImageMasker class provides a method for masking the hole region in an RGB image.
 */
class ImageMasker {
 public:

  /**
   * This function takes an RGB image and a mask image as inputs and returns a
   * grayscale image after masking. The mask image is used to mark
   * the pixels that belong to the hole region with low values
   * (i.e., < MASK_THRESHOLD), and these pixels are then
   * replaced with a HOLE_VALUE in the grayscale image.
   *
   * @param rgb_image The input RGB image.
   * @param mask The input mask image.
   *
   * @return A grayscale image with the masked hole region.
   */
  static Mat ApplyMask (const Mat &rgb_image, const Mat &mask);
};

#endif // IMAGEMASKER_H
