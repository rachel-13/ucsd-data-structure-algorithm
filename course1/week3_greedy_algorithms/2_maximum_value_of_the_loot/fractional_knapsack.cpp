#include <iostream>
#include <vector>
#include <algorithm>  
#include <cmath>

using std::vector;

double get_optimal_value(int capacity, vector<int> weights, vector<int> values) {
  double total_value = 0.0000;

  vector<double> value_per_weight(weights.size());
  for (int i = 0; i < weights.size(); i++) {
    value_per_weight[i] = (double) values[i] / (double) weights[i];
  }
   
  std::vector<double>::iterator max_value_weight;
  int max_value_weight_index = -1;

  while(capacity > 0) {
    max_value_weight = max_element(value_per_weight.begin(), value_per_weight.end());
    max_value_weight_index = distance(value_per_weight.begin(), max_value_weight);

    if(weights[max_value_weight_index] <= capacity) {
      total_value = total_value + (double)(weights[max_value_weight_index] * *max_value_weight);
      capacity = capacity - weights[max_value_weight_index];
    } else {
      total_value = total_value + (double)(capacity * *max_value_weight);
      capacity = 0;
    }

    value_per_weight[max_value_weight_index] = 0;
  }

  return total_value;
}

int main() {
  int n;
  int capacity;
  std::cin >> n >> capacity;
  vector<int> values(n);
  vector<int> weights(n);
  for (int i = 0; i < n; i++) {
    std::cin >> values[i] >> weights[i];
  }

  double optimal_value = get_optimal_value(capacity, weights, values);

  printf("%.4f\n", optimal_value);
  return 0;
}
