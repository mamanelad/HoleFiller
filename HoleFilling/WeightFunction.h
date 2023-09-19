#ifndef WEIGHT_FUNCTION_H
#define WEIGHT_FUNCTION_H

#include <utility>

/**
 * @brief Alias for a pair of integers representing an (x, y) coordinate.
 */
using Pixel = std::pair<int, int>;

/**
 * @brief Abstract class representing a weight function.
 *
 * The WeightFunction abstract class defines the interface for a weight
 * function, which assigns weights to pairs of pixels with depth values in a
 * depth map.
 * Subclasses of WeightFunction implement specific weight functions.
 */
class WeightFunction {
 public:
 /**
  * @brief Computes the weight of two pixels with given depth values.
  *
  * @param p1 The first pixel.
  * @param p2 The second pixel.
  * @param z The absolute difference between the depth values of p1 and p2.
  * @param epsilon A small constant value to avoid division by zero.
  *
  * @return The weight of the two pixels.
  */
  virtual double GetWeight (const Pixel p1, const Pixel p2, const int z,
                            const long epsilon) const = 0;

/**
  * @brief Virtual destructor for the WeightFunction class.
  */
  virtual ~WeightFunction () = default;
};

#endif // WEIGHT_FUNCTION_H
