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
  double hypot = std::hypot(x1 - x2, y1 - y2);
  return hypot;
}

// bool hasDuplicate(const vector<Coordinate> &coordinates, int size) {
//   int index = 0;
//   while(index < size) {
//     if (coordinates[index].x == coordinates[index+1].x && coordinates[index].y == coordinates[index + 1].y) {
//       return true;
//     }
//     index++;
//     if(index == size - 1) {
//       return false;
//     }
//   }

//   return false;
// }

// double bruteForce(const vector<Coordinate> &coordinates, int size) {
//   double result = MAXFLOAT;
//   for(int i = 0; i < size; i++) {
//     for(int j = i; j < size; j++) {
//       if(i == j) { break; }
//       double distance = calculateDistance(coordinates[i].x, coordinates[i].y, coordinates[j].x, coordinates[j].y);
//       if(distance < result) {       
//         result = distance;
//       }
//     }
//   }
//   return result;
// }

double stripDistance(const vector<Coordinate> &coordinates, double distance, int size) {
  double result = distance;

  for(int i = 0; i < size; i++) {
    for(int j = i + 1; j < size && abs((coordinates[j].y - coordinates[i].y) < distance); j++) 
    {
      double distance = calculateDistance(coordinates[i].x, coordinates[i].y, coordinates[j].x, coordinates[j].y);
      if(distance < result) {       
        result = distance;
      }
    }
  }

  return result;
}

double findClosestPair(const vector<Coordinate> &coordinatesXSorted, int left, int right, const vector<Coordinate> coordinatesYSorted) {
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
  double distance_leftSubArr = findClosestPair(coordinatesXSorted, left, midX_index, coordinatesYSorted);
  double distance_rightSubArr = findClosestPair(coordinatesXSorted, midX_index + 1, right, coordinatesYSorted);
  result = min(distance_leftSubArr, distance_rightSubArr);

  int midX = coordinatesXSorted[midX_index].x;
  vector<Coordinate> stripCoordinates;
  for(int i = 0; i < coordinatesYSorted.size(); i++) {
    Coordinate coordinate = coordinatesYSorted[i];
    if(abs(coordinate.x - midX) < result) {
      stripCoordinates.push_back(coordinate);
    }
  }
  int stripCoordinatesSize = stripCoordinates.size();
  if( stripCoordinatesSize >= 2) {
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
  
  // if(hasDuplicate(coordinates, coordinates.size())) { return 0; }

  result = findClosestPair(coordinatesXSorted, 0, coordinatesXSorted.size() - 1, coordinatesYSorted);
  
  // limiting coordinates to <-- -distance --- midPoint on X-axis -- +distance -->
  // int midX_index = (coordinates.size() - 1) / 2;
  // int midX = coordinates[midX_index].x;
  // vector<Coordinate> stripCoordinates;
  // for(int i = 0; i < coordinates.size(); i++) {
  //   Coordinate coordinate = coordinates[i];
  //   if(abs(coordinate.x - midX) < result) {
  //     stripCoordinates.push_back(coordinate);
  //   }
  // }
  // if(stripCoordinates.size() >= 2) {
  //   sort(stripCoordinates.begin(), stripCoordinates.end(), sortByY);
  //   double resultForYAxis = stripDistance(stripCoordinates, result, stripCoordinates.size());
  //   result = min(result, resultForYAxis);
  // }
  
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
    int inputSize = rand() % 5000 + 2;
    std::cout << inputSize << "\n";

    vector<int> x;
    vector<int> y;

    for(int i = 0; i < inputSize; i++) {
      int randomX = (rand() % 5000) - (rand() % 5000);
      x.push_back(randomX);

      int randomY = (rand() % 5000) - (rand() % 5000);
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
