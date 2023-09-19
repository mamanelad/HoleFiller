#include <opencv2/core.hpp>     // Core functionality of OpenCV
#include <opencv2/imgcodecs.hpp> // Reading and writing image files

#include <iostream>
#include <string>

#include "ImageMasker.h"
#include "MyWeightFunction.h"
#include "HoleFiller.h"

#define MSG_ERR_ARG_AMOUNT \
"Error: Please provide the following command-line arguments:\n\
- Input image path\n\
- Mask image path\n\
- Value of z (integer)\n\
- Value of epsilon (positive float)\n\
- Connectivity type (4, or 8)\n\
- Algorithem type (1, 2, or 3)"

#define MSG_ERR_OPEN_IMAGE "Error: Could not open the image file"
#define MSG_ERR_OPEN_MASK_IMAGE "Error: Could not open the mask image file"
#define MSG_ERR_IMAGE_SIZE "Error: Images have different sizes"
#define MSG_ERR_Z_VALUE "Error: z value is not a valid integer."
#define MSG_ERR_EPSILON_NOT_POSITIVE "Error: eps should be a positive number."
#define MSG_ERR_CONNECTIVITY_VALUE \
                              "Error: Invalid value for connectivity number."
#define MSG_ERR_ALGORITHM_TYPE "Error: Invalid value for Algorithm type."

#define DISPLAY_IMAGE_NAME "Float Image"
#define SAVING_IMAGE_NAME "filledImage.png"
#define NULL_CHARACTER '\0'

#define ARGUMENTS_AMOUNT 7

#define ARGUMENT_VALUE_RGB_IMAGE 1
#define ARGUMENT_VALUE_MASK_IMAGE 2
#define ARGUMENT_VALUE_Z 3
#define ARGUMENT_VALUE_EPSILON 4
#define ARGUMENT_VALUE_CONNECTIVITY 5
#define ARGUMENT_VALUE_ALGORITHM_TYPE 6

#define STRTOL_BASE 10

/**
 * @brief This function checks if the number of command-line arguments
 * is equal to a pre-defined value - ARGUMENTS_AMOUNT.
 */

bool ArgumentAmountCheck (int argc)
{
  if (argc != ARGUMENTS_AMOUNT)
    {
      std::cerr << MSG_ERR_ARG_AMOUNT << std::endl;
      return false;
    }

  return true;
}

/**
 * @brief This function checks if two input images, 'image' and 'maskImage',
 * were successfully opened and have the same size.
 */
bool ArgumentImagesCheck (const Mat &image, const Mat &maskImage)
{
  if (image.empty ())
    {
      std::cerr << MSG_ERR_OPEN_IMAGE << std::endl;
      return false;
    }
  if (maskImage.empty ())
    {
      std::cerr << MSG_ERR_OPEN_MASK_IMAGE << std::endl;
      return false;
    }

  if (image.size () != maskImage.size ())
    {
      std::cerr << MSG_ERR_IMAGE_SIZE << std::endl;
      return false;
    }

  return true;
}
/**
 * @brief This function checks if a given string has been successfully
 * converted to a number.
 *
 * @param endPtr A pointer to the first character in the
 * string that failed to convert.
 * @param errorMassage A string message to print to the
 * standard error stream if the check fails.
 *
 */
bool NumbersCheck (const char *endPtr, const std::string &errorMassage)
{
  if (*endPtr != NULL_CHARACTER)
    {
      std::cerr << errorMassage << std::endl;
      return false;
    }
  return true;
}

/**
 * @brief This function checks if the input arguments for z value,
 * epsilon value, connectivity type, and algorithm type are valid.
 *
 * @param endPtrZ Pointer to string representing the z value.
 * @param e Value of epsilon.
 * @param endPtrE Pointer to string representing the epsilon value.
 * @param connectivity Connectivity type (4 or 8).
 * @param endPtrC Pointer to string representing the connectivity value.
 * @param algorithmType Algorithm type (1, 2).
 * @param endPtrA Pointer to string representing the algorithm type.
 *
 * @return True if all the input arguments are valid, false otherwise.
 */
bool
ArgumentNumbersCheck (const char *endPtrZ, const float e,
                      const int connectivity, const char *endPtrC, int algorithmType,
                      const char *endPtrA)
{

  if (!NumbersCheck (endPtrZ, MSG_ERR_Z_VALUE))
    return false;

  if (e <= 0)
    {
      std::cerr << MSG_ERR_EPSILON_NOT_POSITIVE << std::endl;
      return false;
    }

  if (!NumbersCheck (endPtrC, MSG_ERR_CONNECTIVITY_VALUE))
    return false;

  if ((connectivity != CONNECTIVITY_OPTION_1
       && connectivity != CONNECTIVITY_OPTION_2))
    {
      std::cerr << MSG_ERR_CONNECTIVITY_VALUE << std::endl;
      return false;

    }

  if (!NumbersCheck (endPtrA, MSG_ERR_ALGORITHM_TYPE))
    return false;

  if (algorithmType != ALGORITHM_OPTION_ONE
      && algorithmType != ALGORITHM_OPTION_TWO)
    {
      std::cerr << MSG_ERR_ALGORITHM_TYPE << std::endl;

    }

  return true;
}

/**
 * Displays a given float image as an 8-bit unsigned integer image.
 *
 * @param float_image The input float image to display.
 */
void DisplayFloatImage(const Mat &float_image) {
  // Convert the float image to an 8-bit unsigned integer image
  Mat display_image;
  float_image.convertTo(display_image, CV_8U);

  // Display the converted image
  imshow(DISPLAY_IMAGE_NAME, display_image);
  waitKey(0);
}


/**
 * The main function of the program.
 * It reads in an image file and a mask file from the user-specified command
 * line arguments, performs a series of checks on those arguments,
 * and then applies a hole-filling algorithm to the image to fill any holes
 * that are present in the masked area. The resulting image is
 * saved as "filledImage.png" in the current directory.
 *
 * @param argc The number of arguments passed in from the command line.
 * @param argv An array of character strings containing the
 * command line arguments.
 * @return An integer representing the success or failure of the program
 * (0 for success, 1 for failure).
 */
int main (int argc, char **argv)
{

  //Argument Handling
  if (!(ArgumentAmountCheck (argc))) return 1;

  Mat rgb_image = imread (argv[ARGUMENT_VALUE_RGB_IMAGE], IMREAD_COLOR);
  Mat maskImage = imread (argv[ARGUMENT_VALUE_MASK_IMAGE], IMREAD_COLOR);

  if (!(ArgumentImagesCheck (rgb_image, maskImage))) return 1;

  char *endPtrZ;
  int z = (int) std::strtol (argv[ARGUMENT_VALUE_Z], &endPtrZ, STRTOL_BASE);

  float epsilon = std::stof (argv[ARGUMENT_VALUE_EPSILON]);

  char *endPtrC;
  int connectivity = (int) std::strtol (argv[ARGUMENT_VALUE_CONNECTIVITY],
                                        &endPtrC, STRTOL_BASE);

  char *endPtrA;
  int algorithmType = (int) std::strtol
      (argv[ARGUMENT_VALUE_ALGORITHM_TYPE], &endPtrA, STRTOL_BASE);

  if (!(ArgumentNumbersCheck (endPtrZ, epsilon, connectivity, endPtrC,
                              algorithmType, endPtrA)))
    return 1;


  //Preprocess on the rgb_image
  Mat imageAfterMask = ImageMasker::ApplyMask (rgb_image, maskImage);

  // Define a std::function object that takes four parameters and returns a
  // double value, and set the function to point to the GetWeight method of
  // the MyWeightFunction class.
  std::function<double (Pixel, Pixel, int, double)> weightFunction;
  weightFunction = &MyWeightFunction::GetWeight;

  //Filling the hole.
  HoleFiller holeFiller(z, epsilon, connectivity, algorithmType, weightFunction);
  Mat filledImage = holeFiller.FillImage (imageAfterMask);
  //Saving the filled hole Image
  imwrite (SAVING_IMAGE_NAME, filledImage);

  return 0;
}
