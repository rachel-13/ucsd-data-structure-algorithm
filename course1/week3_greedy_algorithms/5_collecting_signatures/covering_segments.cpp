#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>

using std::vector;

struct Segment {
  int start, end;
};

bool compareByEndValue(const Segment &a, const Segment &b)
{
    return a.end < b.end;
}

vector<int> optimal_points(vector<Segment> &segments) {
  vector<int> points;
  vector<int> results;
  //write your code here
  sort(segments.begin(), segments.end(), compareByEndValue);

  int point = -1;
  for (size_t i = 0; i < segments.size(); ++i) {
    Segment currentSegment = segments[i];
    Segment nextSegment = segments[i + 1];

    if(i == 0 && currentSegment.end >= nextSegment.start) {
      points.push_back(currentSegment.end);
      point = currentSegment.end;
    } 

    if (currentSegment.start > point || currentSegment.end < point) {
      points.push_back(currentSegment.end);
      point = currentSegment.end;
    }
    
  }

  return points;
}

int main() {
  int n;
  std::cin >> n;
  vector<Segment> segments(n);
  for (size_t i = 0; i < segments.size(); ++i) {
    std::cin >> segments[i].start >> segments[i].end;
  }
  vector<int> points = optimal_points(segments);
  std::cout << points.size() << "\n";
  for (size_t i = 0; i < points.size(); ++i) {
    std::cout << points[i] << " ";
  }
}
