#include <opencv2/core.hpp>
#include <vector>
#include <unordered_map>
#include <functional>
#include <unordered_set>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>

#define CONNECTIVITY_OPTION_1 4
#define CONNECTIVITY_OPTION_2 8
#define HOLE_VALUE -1
#define ALGORITHM_OPTION_ONE 1
#define ALGORITHM_OPTION_TWO 2

#define APPROXIMATE_ALGORITHM_ROUTINE_AMOUNT 100

#define INDEX(i, j, cols) (((i) * (cols))+ (j))
using namespace cv;


/**
 * @brief Alias for a pair of integers representing an (x, y) coordinate.
 */
using Pixel = std::pair<int, int>;
typedef std::function<double (Pixel, Pixel, int, double)> WeightFunctionType;

/**
 * HoleFiller class is used for filling the holes in an image using different
 * techniques.
 */
class HoleFiller {
 private:
  int z_;
  double epsilon_;
  int connectivity_;
  int algorithmType;
  WeightFunctionType weightFunc_;

  //Data structures
  std::unordered_set<int> visitedSet;
  std::vector<Pixel> holePixelsVector_;
  std::vector<Pixel> boundaryPixelsCoordinatesVector_;
  std::vector<float> boundaryPixelsValuesVector_;
  std::unordered_map<int, int> layerMap;
  std::map<int, std::vector<Pixel>> layerMapReverse;
  std::vector<Pixel> curLayerVector;
  std::vector<Pixel> tempLayerVector;

 public:

  /**
   * @brief Constructor for the HoleFiller class.
   */
   HoleFiller (const int z, const double epsilon, const int connectivity,
              const int algorithm_type, const WeightFunctionType &weight_func);
  /**
   * @brief This function fills the hole region in the input image.
   */
   Mat FillImage (const Mat &image);

 private:
  /**
   * @brief This function returns the coordinates of a neighbor pixel
   * of a given pixel based on its index.
   *
   * @param currentPixel The coordinates of the current pixel.
   * @param index The index of the neighbor pixel to be returned.
   */
   Pixel GetNeighborPixel (const Pixel currentPixel, int index);

  /**
   * @brief This function finds the pixels belonging to the hole region and
   * the boundary region.
   * @param image The input image containing a hole that needs to be filled.
   */
   void FindHoleAndBoundaryPixels (const Mat &image);

  /**
   * @brief This function finds the first pixel belonging to the hole region
   * and returning it.
   * @param image The input image containing a hole that needs to be filled.
   */
   Pixel FindFirstHolePixel (const Mat &image);

  /**
   * @brief FloodFill - A function that recursively searches for hole and boundary pixels
   *                     using the flood fill algorithm and saves them in different vectors.
   *
   * @param image The input image to search for hole and boundary pixels.
   * @param currentPixel The current pixel being evaluated during the recursive search.
   */
   void FloodFill (const Mat &image, Pixel currentPixel);

  /**
   * @brief This function fills a hole in an image using the regular algorithm.
   *
   * @param image The input image containing a hole that needs to be filled.
   * @param filledImage The output image with the hole filled.
   */
   void RegularAlgorithm (const Mat &image, Mat &filledImage);

  /**
   * @brief This function sets layers for the hole pixels using boundary pixels.
   * It saves the pixels of each layer to the `layerMapReverse` and `layerMap`
   * data structures.
   *
   * @param image The input image
   */
   void SetLayers (const Mat &image);

  /**
   * @brief SetLayerHelper - A helper function for the SetLayer function that sets the layer of
   *                         the pixels connected to the current pixel recursively.
   *
   * @param image The input image to set the layer of connected pixels.
   * @param currentPixel The current pixel being evaluated during the recursive search.
   * @param curLayer The current layer to assign to the pixels being evaluated.
   */
   void SetLayerHelper (const Mat &image, Pixel currentPixel, int curLayer);

  /**
   * @brief SetLayer - Sets the layer of the current pixel if it is a hole and
   * has not been assigned a layer before.
   *
   * @param image The input image to set the layer of the current pixel.
   * @param currentPixel The current pixel being evaluated for layer assignment.
   * @param currentLayer The current layer to assign to the current pixel.
   */
   void SetLayer (const Mat &image, Pixel currentPixel, int currentLayer);

  /**
   * @brief Fills the hole in the input image using the Approximate Algorithm.
   *
   * The Approximate Algorithm performs several iterations of the following steps:
   *
   * 1. Loop through the layers of boundary pixels in reverse order.
   * 2. For each hole pixel in the current layer, calculate the average of its non-hole neighbors
   *    within a specified maximum layer number.
   * 3. Set the filled value of the hole pixel to be the average calculated in step 2.
   *
   * @param image The input image with holes to be filled.
   * @param filledImage The output image with holes filled using the Approximate Algorithm.
   */
   void ApproximateAlgorithm (const Mat &image, Mat &filledImage);

  /**
   * @brief This function calculates the affect of a boundary pixel on a hole pixel, up to a specified layer number.
   *
   * @param image The input image matrix.
   * @param holePixel The coordinates of the hole pixel.
   * @param maximumLayerNumber The maximum layer number up to which to check for pixel affectation.
   * @param boundaryPixel The coordinates of the boundary pixel.
   * @param dividendSum A reference to the sum of dividend values for the current hole pixel.
   * @param divisorSum A reference to the sum of divisor values for the current hole pixel.
   */
   void CalculatePixelAffect (const Mat &image,
                             const Pixel &holePixel, int maximumLayerNumber,
                             const Pixel &boundaryPixel, double &dividendSum,
                             double &divisorSum);
  /**
   * @brief This function determines if a given pixel affects a hole area
   * up to a specified layer number.
   *
   * @param image The input image matrix.
   * @param pixel The coordinates of the pixel to be checked.
   * @param maximumLayerNumber The maximum layer number up to which to check for
   * pixel affectation.
   */
   bool IsPixelAffecting (const Mat &image, Pixel pixel, int maximumLayerNumber);

  /**
   * @brief Clears all internal data structures used by the hole filling algorithm.
   */
    void ClearFields ();

};
