#include "HoleFiller.h"

HoleFiller::HoleFiller (const int z, const double epsilon, const int connectivity, const int algorithm_type, const WeightFunctionType &weight_func)
    : z_ (z), epsilon_ (epsilon), connectivity_ (connectivity), algorithmType (algorithm_type), weightFunc_ (weight_func)
{}

Mat HoleFiller::FillImage (const Mat &image)
{

  Mat filledImage = image.clone ();
  FindHoleAndBoundaryPixels (image);

  switch (algorithmType)
    {
      case ALGORITHM_OPTION_ONE:
        RegularAlgorithm (image, filledImage);
      break;

      case ALGORITHM_OPTION_TWO:
        SetLayers (image);
      ApproximateAlgorithm (image, filledImage);
      break;
    }

  ClearFields ();
  return filledImage;
}

Pixel HoleFiller::GetNeighborPixel (const Pixel currentPixel, const int index)
{
  int x = currentPixel.first;
  int y = currentPixel.second;

  switch (index)
    {
      case 0:
        return Pixel (x + 1, y);

      case 1:
        return Pixel (x - 1, y);

      case 2:
        return Pixel (x, y + 1);

      case 3:
        return Pixel (x, y - 1);

      case 4:
        return Pixel (x + 1, y + 1);

      case 5:
        return Pixel (x + 1, y - 1);

      case 6:
        return Pixel (x - 1, y + 1);

      case 7:
        return Pixel (x - 1, y - 1);
    }

  return currentPixel;
}

void HoleFiller::FindHoleAndBoundaryPixels (const Mat &image)
{
  Pixel firstHolePixel = FindFirstHolePixel (image);
  FloodFill (image, firstHolePixel);
}

Pixel HoleFiller::FindFirstHolePixel (const Mat &image)
{
  for (int x = 0; x < image.cols; x++)
    {
      for (int y = 0; y < image.rows; y++)
        {
          if (image.at<float> (x, y) == HOLE_VALUE)
            {
              return Pixel (x, y);
            }
        }
    }

  Pixel notFoundPixel (HOLE_VALUE, HOLE_VALUE);
  return notFoundPixel;
}

void HoleFiller::FloodFill (const Mat &image, Pixel currentPixel)
{
  int x = currentPixel.first;
  int y = currentPixel.second;

  int curPixelIndexVal = INDEX(y, x, image.cols);

  if (visitedSet.count (curPixelIndexVal) > 0) return;

  visitedSet.insert (curPixelIndexVal);

  if (x < 0 || x >= image.cols || y < 0 || y >= image.rows) return;

  float currentPixelValue = image.at<float> (x, y);
  if (currentPixelValue != HOLE_VALUE)
    {
      float currentValue = image.at<float> (x, y);
      boundaryPixelsValuesVector_.push_back (currentValue);
      boundaryPixelsCoordinatesVector_.push_back (currentPixel);
      return;
    }

  holePixelsVector_.push_back (currentPixel);

  for (int i = 0; i < 8; ++i)
    {
      if ((i < 4) || (i >= 4 && connectivity_ == CONNECTIVITY_OPTION_2))
        {
          FloodFill (image, GetNeighborPixel (currentPixel, i));
        }
    }
}

void HoleFiller::RegularAlgorithm (const Mat &image, Mat &filledImage)
{
  for (Pixel holePixel : holePixelsVector_)
    {
      double dividendSum = 0;
      double divisorSum = 0;

      for (int i = 0; i < boundaryPixelsCoordinatesVector_.size (); ++i)
        {
          Pixel boundaryPixel = boundaryPixelsCoordinatesVector_[i];
          float boundaryPixelValue = boundaryPixelsValuesVector_[i];

          double currWeightValue =
              weightFunc_ (holePixel, boundaryPixel, z_, epsilon_);

          dividendSum += (boundaryPixelValue * currWeightValue);
          divisorSum += currWeightValue;
        }

      int x = holePixel.first;
      int y = holePixel.second;
      filledImage.at<float> (x, y) = (dividendSum / divisorSum);
    }
}

void HoleFiller::SetLayers (const Mat &image)
{

  int curLayer = 0;
  curLayerVector = boundaryPixelsCoordinatesVector_;
  while (!(curLayerVector.empty ()))
    {

      for (Pixel holePixel : curLayerVector)
        {
          //Entering the boarder pixels to the map
          if (curLayer == 1)
            {
              int x = holePixel.first;
              int y = holePixel.second;
              int curPixelIndexVal = INDEX(y, x, image.cols);
              layerMap[curPixelIndexVal] = curLayer;
            }

          SetLayerHelper (image, holePixel, curLayer);
        }

      curLayerVector = tempLayerVector;
      tempLayerVector.clear ();
      curLayer += 1;
    }
}

void HoleFiller::SetLayerHelper (const Mat &image, const Pixel currentPixel,
                            const int currentLayer)
{
  for (int i = 0; i < 8; ++i)
    {
      if ((i < 4) || (i >= 4 && connectivity_ == CONNECTIVITY_OPTION_2))
        {
          SetLayer (image, GetNeighborPixel (currentPixel, i), currentLayer);
        }
    }
}
void HoleFiller::SetLayer (const Mat &image, const Pixel currentPixel,
                           const int currentLayer)
{
  int x = currentPixel.first;
  int y = currentPixel.second;
  int curPixelIndexVal = INDEX(y, x, image.cols);
  float currentPixelValue = image.at<float> (x, y);

  if (currentPixelValue == HOLE_VALUE)
    {
      if (!(layerMap.count (curPixelIndexVal)))
        {
          tempLayerVector.push_back (currentPixel);
          layerMap[curPixelIndexVal] = (currentLayer + 1);
          layerMapReverse[currentLayer + 1].push_back (currentPixel);
        }
    }
}

void HoleFiller::ApproximateAlgorithm (const Mat &image, Mat &filledImage)
{

  for (int j = 0; j < APPROXIMATE_ALGORITHM_ROUTINE_AMOUNT; ++j)
    {
      for (auto it = layerMapReverse.begin ();
           it != layerMapReverse.end (); ++it)
        {
          for (Pixel holePixel : it->second)
            {
              double dividendSum = 0;
              double divisorSum = 0;

              int x = holePixel.first;
              int y = holePixel.second;
              int curPixelIndexVal = INDEX(y, x, image.cols);

              int myLayerNumber = layerMap[curPixelIndexVal];

              for (int i = 0; i < 8; ++i)
                {
                  if ((i < 4)
                      || (i >= 4 && connectivity_ == CONNECTIVITY_OPTION_2))
                    {
                      CalculatePixelAffect (filledImage, holePixel,
                                            myLayerNumber,
                                            GetNeighborPixel (holePixel, i),
                                            dividendSum, divisorSum);
                    }
                }

              filledImage.at<float> (x, y) = (dividendSum / divisorSum);
            }
        }
    }
}

void HoleFiller::CalculatePixelAffect (const Mat &image, const Pixel &holePixel,
                                  int maximumLayerNumber, const Pixel
                                  &boundaryPixel, double &dividendSum,
                                  double &divisorSum)
{

  int x = boundaryPixel.first;
  int y = boundaryPixel.second;

  if (IsPixelAffecting (image, boundaryPixel, maximumLayerNumber))
    {

      float boundaryPixelValue = image.at<float> (x, y);

      double currWeightValue =
          weightFunc_ (holePixel, boundaryPixel, z_, epsilon_);
      dividendSum += (boundaryPixelValue * currWeightValue);
      divisorSum += currWeightValue;

    }
}

bool
HoleFiller::IsPixelAffecting (const Mat &image, Pixel pixel,
                              int maximumLayerNumber)
{
  int x = pixel.first;
  int y = pixel.second;

  if (image.at<float> (x, y) != HOLE_VALUE)
    {

      int curPixelIndexVal = INDEX(y, x, image.cols);
      int currentPixelLayer = layerMap[curPixelIndexVal];
      if (currentPixelLayer <= maximumLayerNumber)
        {
          return true;
        }
    }

  return false;
}

void HoleFiller::ClearFields ()
{
  visitedSet.clear ();
  holePixelsVector_.clear ();
  boundaryPixelsCoordinatesVector_.clear ();
  boundaryPixelsValuesVector_.clear ();
  layerMap.clear ();
  layerMapReverse.clear ();
  curLayerVector.clear ();
  tempLayerVector.clear ();
}