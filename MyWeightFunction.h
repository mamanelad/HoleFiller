#ifndef MY_WEIGHT_FUNCTION_H
#define MY_WEIGHT_FUNCTION_H

#include <cmath>
#include "WeightFunction.h"

/**
 * MyWeightFunction is a concrete implementation of the abstract base class
 * WeightFunction. It provides a weight calculation method based on Euclidean
 * distance between two pixels, raised to a power and scaled by a factor of
 * epsilon. The power and epsilon values can be set by the user.
 */
class MyWeightFunction : public WeightFunction {
 public:
  /**
   * Calculates the weight between two pixels using Euclidean distance raised
   * to a power and scaled by a factor of epsilon. The power and epsilon
   * values are set by the user. Returns the calculated weight as a double.
   *
   * @param p1 The first pixel.
   * @param p2 The second pixel.
   * @param z The power to which Euclidean distance is raised.
   * @param epsilon The scaling factor for the weight calculation.
   *
   * @return The calculated weight as a double.
   */
  static double GetWeight (const Pixel p1, const Pixel p2,
                           const int z, const double epsilon);

 private:
  /**
   * Calculates the Euclidean distance between two pixels. Returns the distance
   * as a double.
   *
   * @param p1 The first pixel.
   * @param p2 The second pixel.
   *
   * @return The Euclidean distance between the two pixels as a double.
   */
  static double EuclideanDistance (const Pixel &p1, const Pixel &p2);
};

#endif // MY_WEIGHT_FUNCTION_H
