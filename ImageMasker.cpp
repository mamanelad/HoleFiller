#include "ImageMasker.h"

Mat ImageMasker::ApplyMask (const Mat &rgb_image, const Mat &mask)
{
  Mat gray_image_;
  // Convert the RGB image to grayscale
  cvtColor (rgb_image, gray_image_, cv::COLOR_BGR2GRAY);

  // Create a new floating point matrix to store the masked values
  Mat masked_image = Mat::zeros (gray_image_.size (), CV_32F);

  for (int i = 0; i < mask.rows; i++)
    {
      for (int j = 0; j < mask.cols; j++)
        {
          if (mask.at<float> (i, j) < MASK_THRESHOLD)
            {
              masked_image.at<float> (i, j) = static_cast<float>(HOLE_VALUE);
            }
          else
            {
              masked_image.at<float> (i, j) =
                  static_cast<float>(gray_image_.at<uchar> (i, j));
            }
        }
    }

  return masked_image;
}

