#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>

using std::vector;
using std::string;
using std::pair;
using std::min;
using std::make_pair;

struct Coordinate {
  public:
  int x, y;
};

bool sortByX(const Coordinate &a, const Coordinate &b) { 
  return (a.x < b.x); 
} 

bool sortByY(const Coordinate &a, const Coordinate &b) { 
  return (a.y < b.y); 
} 

double calculateDistance(const int &x1, const int &y1, const int &x2, const int &y2) {
  double hypot = sqrt(((x1-x2) * (x1-x2)) + ((y1-y2) * (y1-y2))); // std::hypot(x1 - x2, y1 - y2);
  return hypot;
}

double stripDistance(const vector<Coordinate> &stripCoordinates, double distance, int size) {
  double result = distance;

  for(int i = 0; i < size; i++) {
    for(int j = i + 1; j < size && ((stripCoordinates[j].y - stripCoordinates[i].y) < distance); j++) 
    {
      double distance = calculateDistance(stripCoordinates[i].x, stripCoordinates[i].y, stripCoordinates[j].x, stripCoordinates[j].y);
      if(distance < result) {       
        result = distance;
      }
    }
  }

  return result;
}

double findClosestPair(const vector<Coordinate> &coordinatesXSorted, int left, int right, const vector<Coordinate> &coordinatesYSorted) {
  double result = -1;
  
  if(right <= left + 3) {
    for(int i = left; i <= right; i++) {
      for(int j = i + 1; j <= right; j++) {
        double distance = calculateDistance(coordinatesXSorted[i].x, coordinatesXSorted[i].y, coordinatesXSorted[j].x, coordinatesXSorted[j].y);
        if(distance < result || result < 0) {       
          result = distance;
        }
      }
    }
    return result;
  }
  
  int midX_index = left + ((right - left) / 2);
  int midX = coordinatesXSorted[midX_index].x;

  vector<Coordinate> sortedYLeftOfMidX;
  vector<Coordinate> sortedYRightOfMidX;
  for(int k = 0; k < coordinatesYSorted.size(); k++) {
    if(coordinatesYSorted[k].x <= midX) {
      sortedYLeftOfMidX.push_back(coordinatesYSorted[k]);
    } else {
      sortedYRightOfMidX.push_back(coordinatesYSorted[k]);
    }
  }

  double distance_leftSubArr = findClosestPair(coordinatesXSorted, left, midX_index, sortedYLeftOfMidX);
  double distance_rightSubArr = findClosestPair(coordinatesXSorted, midX_index + 1, right, sortedYRightOfMidX);
  result = min(distance_leftSubArr, distance_rightSubArr);

  
  vector<Coordinate> stripCoordinates;
  for(int i = 0; i < coordinatesYSorted.size(); i++) {
    Coordinate coordinate = coordinatesYSorted[i];
    if(abs(coordinate.x - midX) < result) {
      stripCoordinates.push_back(coordinate);
    }
  }
  int stripCoordinatesSize = stripCoordinates.size();
  if( stripCoordinatesSize > 1) {
    double resultForYAxis = stripDistance(stripCoordinates, result, stripCoordinatesSize);
    result = min(result, resultForYAxis);
  }

  return result;
}

double minimal_distance(vector<int> x, vector<int> y) {
  double result = MAXFLOAT;
  vector<Coordinate> coordinatesXSorted(x.size());
  vector<Coordinate> coordinatesYSorted(y.size());

  for(int i = 0; i < x.size(); i++) {
    Coordinate coordinate = { x[i], y[i] };
    coordinatesXSorted[i] = coordinate;
    coordinatesYSorted[i] = coordinate;
  }

  sort(coordinatesXSorted.begin(), coordinatesXSorted.end(), sortByX);
  sort(coordinatesYSorted.begin(), coordinatesYSorted.end(), sortByY);
  
  result = findClosestPair(coordinatesXSorted, 0, coordinatesXSorted.size() - 1, coordinatesYSorted);
  
  return result;
}

double naive_minimal_distance(vector<int> x, vector<int> y) {
  double result = -1.0000;
  
  for(int i = 0; i < x.size(); i++) {
    int currentX = x[i];
    int currentY = y[i];

    for(int j = 0; j < x.size(); j++) {
      int nextX = x[j];
      int nextY = y[j];
      if(i == j) {
        break;
      }

      double distance = calculateDistance(currentX, currentY, nextX, nextY);
      if(distance < result || result < 0.000) {
        if (distance == 0.00) {
          return 0.000;
        } else {
          result = distance;
        }
      }
    }
  }

  return result * 1.0000;
}

void test_solution() {
  while(true) {
    int inputSize = rand() % 10000 + 2;
    std::cout << inputSize << "\n";

    vector<int> x;
    vector<int> y;

    for(int i = 0; i < inputSize; i++) {
      int randomX = (rand() % 10000) - (rand() % 10000);
      x.push_back(randomX);

      int randomY = (rand() % 10000) - (rand() % 10000);
      y.push_back(randomY);

      // std::cout << randomX << " " << randomY << "\n";
    }

    double res1 = naive_minimal_distance(x, y);
    double res2 = minimal_distance(x, y);

    if(res1 == res2) {
        std::cout << "\n\nOK!\n";
        std::cout << std::setprecision(9) << res1 << " ";
        
        std::cout << "\nvs\n";
        std::cout << res2 << " ";
      
        std::cout << "\n==========================\n";
      } else {

        std::cout << "\n\nWrong result \n";
        std::cout << res1 << " ";
        
        std::cout << "\nvs\n";
        std::cout << std::setprecision(9) << res2 << " ";
        
        std::cout << "\n==========================\n";
        break;
    }
  }
}

void test_one_solution() {
  vector<int> x = {-5, -7, -6, 8, 0, -6, 2, 3, 8 };
  vector<int> y = {-9, 6, 0, -4, -4, -8, -2, -4, 7 };  
  double res = minimal_distance(x, y);
  std::cout << std::fixed;
  std::cout << std::setprecision(9) << res << "\n";
}

int main() {

  // test_solution();
  // test_one_solution();

  size_t n;
  std::cin >> n;
  vector<int> x(n);
  vector<int> y(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> x[i] >> y[i];
  }
  std::cout << std::fixed;
  std::cout << std::setprecision(9) << minimal_distance(x, y) << "\n";
}
