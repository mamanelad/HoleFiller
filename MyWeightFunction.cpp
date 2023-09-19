#include "MyWeightFunction.h"

double MyWeightFunction::GetWeight(const Pixel p1, const Pixel p2, const int z, const double epsilon) {
  double euclideanDistance = EuclideanDistance(p1, p2);
  double eCPowByZ = std::pow(euclideanDistance, z);
  double returnValue = (1.0/ (eCPowByZ + epsilon));
  return returnValue;
}

double MyWeightFunction::EuclideanDistance(const Pixel& p1, const Pixel& p2) {
  double dx = p2.first - p1.first;
  double dy = p2.second - p1.second;
  double distance = std::sqrt((dx * dx) + (dy * dy));
  return distance;
}

