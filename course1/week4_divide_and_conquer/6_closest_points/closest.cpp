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

double calculateDistance(int x1, int y1, int x2, int y2) {
  int x1_minus_x2 = x1 - x2;
  int x1_minus_x2_squared = x1_minus_x2 * x1_minus_x2;

  int y1_minus_y2 = y1 - y2;
  int y1_minus_y2_squared = y1_minus_y2 * y1_minus_y2;

  int sum_of_sqaured_x_y = x1_minus_x2_squared + y1_minus_y2_squared;
  double pythogorem = sqrt(sum_of_sqaured_x_y);
  return pythogorem;
}

vector<Coordinate> findElementsLessThanDistance(double distance, vector<Coordinate> coordinates, int midX) {
  vector<Coordinate> result = {};

  for(int i = 0; i < coordinates.size(); i++) {
    Coordinate coordinate = coordinates[i];
    if(abs(coordinate.x - midX) < distance) {
      result.push_back(coordinate);
    }
  }
  return result;
}

double stripDistance(vector<Coordinate> &coordinates, double distance) {
  double result = distance;
  double size = coordinates.size();
  
  sort(coordinates.begin(), coordinates.end(), sortByY);

  for(int i = 0; i < size; ++i) {
    int currentX = coordinates[i].x;
    int currentY = coordinates[i].y;

    for(int j = i + 1; j < size && ((coordinates[j].y - currentY) < distance); ++j) 
    {
      int nextX = coordinates[j].x;
      int nextY = coordinates[j].y;
      
      double distance = calculateDistance(currentX, currentY, nextX, nextY);
      if(distance < result) {       
        result = distance;
      }
    }
  }

  return result;
}

double findClosestPair(const vector<Coordinate> &coordinates, int left, int right) {
  double result = MAXFLOAT;
  
  if(right <= left + 3) {
    for(int i = left; i <= right - left; i++) {
      int currentX = coordinates[i].x;
      int currentY = coordinates[i].y;

      for(int j = i; j <= right - left; j++) {
        if(i == j) { break; }
        int nextX = coordinates[j].x;
        int nextY = coordinates[j].y;
        
        double distance = calculateDistance(currentX, currentY, nextX, nextY);
        if(distance < result) {       
          result = distance;
        }
      }
    }

    return result;
  }

  if(right > left) {
    int midX_index = left + ((right - left) / 2);
    int midX = coordinates[midX_index].x;
    double distance_leftSubArr = findClosestPair(coordinates, left, midX_index);
    double distance_rightSubArr = findClosestPair(coordinates, midX_index + 1, right);
    result = min(distance_leftSubArr, distance_rightSubArr);
  }

  return result;
}

double minimal_distance(vector<int> x, vector<int> y) {
  double result = MAXFLOAT;
  vector<Coordinate> coordinates;

  for(int i = 0; i < x.size(); i++) {
    Coordinate coordinate = Coordinate();
    coordinate.x = x[i];
    coordinate.y = y[i];
    coordinates.push_back(coordinate);
  }

  sort(coordinates.begin(), coordinates.end(), sortByX);
  result = findClosestPair(coordinates, 0, coordinates.size() - 1);

  // limiting coordinates to <-- -distance --- midPoint on X-axis -- +distance -->
  int midX_index = (coordinates.size() - 1) / 2;
  int midX = coordinates[midX_index].x;
  vector< Coordinate > xCoordinatesLessThanResult;
  for(int i = 0; i < coordinates.size(); i++) {
    Coordinate coordinate = coordinates[i];
    if(abs(coordinate.x - midX) < result) {
      xCoordinatesLessThanResult.push_back(coordinate);
    }
  }
  if(xCoordinatesLessThanResult.size() >= 2) {
    double resultForYAxis = stripDistance(xCoordinatesLessThanResult, result);
    result = min(result, resultForYAxis);
  }
  
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
  vector<int> x = {2, -4, 2, 3, -1, -2};
  vector<int> y = {-2, 0, 0, -3, 3, 0};  
  double res = minimal_distance(x, y);
  std::cout << std::fixed;
  std::cout << std::setprecision(9) << res << "\n";
}

int main() {

  test_solution();
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
