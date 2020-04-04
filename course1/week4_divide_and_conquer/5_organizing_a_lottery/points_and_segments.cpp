#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;
using std::make_pair;

vector<int> findIndexForElement(int point, vector<int> points) {
  vector<int> result;

  for(int j = 0; j < points.size(); j++) {
    if (points[j] == point) {
      result.push_back(j);
    }
  }

  return result;
}

vector<int> fast_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size());
  vector< pair<int, char> > mapOfSegmentsAndPoints;

  for(int i = 0; i < starts.size(); i++) {
    mapOfSegmentsAndPoints.push_back(make_pair(starts[i], 'l'));
    mapOfSegmentsAndPoints.push_back(make_pair(ends[i], 'r'));
  }

  for(int i = 0; i < points.size(); i++) {
    mapOfSegmentsAndPoints.push_back(make_pair(points[i], 'p'));
  }

  sort(mapOfSegmentsAndPoints.begin(), mapOfSegmentsAndPoints.end());

  int index = 0;
  int openSegmentCount = 0;
  while(index < mapOfSegmentsAndPoints.size()) {
    
    pair<int, char> currentPair = mapOfSegmentsAndPoints[index];

    if (currentPair.second == 'l') {
      openSegmentCount++;
    } else if (currentPair.second == 'r') {
      openSegmentCount--;
    }

    if(currentPair.second == 'p' && openSegmentCount > 0) {
      vector<int> pointsIndices = findIndexForElement(currentPair.first, points);
      for(int i = 0; i < pointsIndices.size(); i++) {
        int originalPointIndex = pointsIndices[i];
        cnt[originalPointIndex] = openSegmentCount;
      }
    } else if(currentPair.second == 'p' && openSegmentCount <= 0) {
      vector<int> pointsIndices = findIndexForElement(currentPair.first, points);
      for(int i = 0; i < pointsIndices.size(); i++) {
        int originalPointIndex = pointsIndices[i];
        cnt[originalPointIndex] = 0;
      }
    }

    index++;
  }

  return cnt;
}

vector<int> naive_count_segments(vector<int> starts, vector<int> ends, vector<int> points) {
  vector<int> cnt(points.size());
  for (size_t i = 0; i < points.size(); i++) {
    for (size_t j = 0; j < starts.size(); j++) {
      cnt[i] += starts[j] <= points[i] && points[i] <= ends[j];
    }
  }
  return cnt;
}

void test_solution() {
  while(true) {
    int segmentInputSize = rand() % 10 + 1;
    int pointInputSize = rand() % 10 + 1;
    std::cout << segmentInputSize << " " << pointInputSize << "\n";

    vector<int> starts(segmentInputSize), ends(segmentInputSize);
    for(int i = 0; i < segmentInputSize; i++) {
      int start = (rand() % 10) - (rand() % 10);
      int end = start + (rand() % 10 + 1);
      starts[i] = start;
      ends[i] = end;
      std::cout << start << " " << end << "\n";
    }

    vector<int> points(pointInputSize);
    for(int i = 0; i < pointInputSize; i++) {
      int point = (rand() % 10) - (rand() % 10);
      points[i] = point;
      std::cout << point << " ";
    }  

    vector<int> res1 = naive_count_segments(starts, ends, points);
    vector<int> res2 = fast_count_segments(starts, ends, points);

    if(res1 == res2) {
      std::cout << "\n\nOK!\n";
      for(int i = 0; i < pointInputSize; i++) {
        std::cout << res1[i] << " ";
      }
      std::cout << "\nvs\n";
      for(int i = 0; i < pointInputSize; i++) {
        std::cout << res2[i] << " ";
      }

      std::cout << "\n==========================\n";
    } else {

      std::cout << "\n\nWrong result \n";
      for(int i = 0; i < pointInputSize; i++) {
        std::cout << res1[i] << " ";
      }
      std::cout << "\nvs\n";
      for(int i = 0; i < pointInputSize; i++) {
        std::cout << res2[i] << " ";
      }
      std::cout << "\n==========================\n";
      break;
    }
  }
}

void test_one_solution() {
  vector<int> starts = {5, -3, 7}; 
  vector<int> ends = {13, 7, 13};
  vector<int> points = {1, -5, -3, 8};

  vector<int> res = naive_count_segments(starts, ends, points);
  for(int i = 0; i < res.size(); i++) {
    std::cout << res[i] << " ";
  }
}

int main() {
  // test_solution();
  // test_one_solution();

  int n, m;
  std::cin >> n >> m;
  vector<int> starts(n), ends(n);
  for (size_t i = 0; i < starts.size(); i++) {
    std::cin >> starts[i] >> ends[i];
  }
  vector<int> points(m);
  for (size_t i = 0; i < points.size(); i++) {
    std::cin >> points[i];
  }
  //use fast_count_segments
  vector<int> cnt = fast_count_segments(starts, ends, points);
  for (size_t i = 0; i < cnt.size(); i++) {
    std::cout << cnt[i] << ' ';
  }
}
