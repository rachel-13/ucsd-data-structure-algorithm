#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <limits.h>
#include <cmath>
using namespace std;

typedef vector<vector<double>> matrix;

struct Position
{
public:
  int row, column;
};

void printTableu(matrix tableu)
{
  for (int i = 0; i < tableu.size(); i++)
  {
    for (int j = 0; j < tableu[0].size(); j++)
    {
      cout << setw(10) << tableu[i][j] << " ";
    }
    cout << "\n";
  }
}

void printVector(vector<double> v)
{
  for (int i = 0; i < v.size(); i++)
  {
    cout << setw(10) << v[i] << " ";
  }
  cout << "\n";
}

vector<Position> artificialVarsIndex;
void buildPhase1Tableu(int n, int m, matrix &tableu, matrix A, vector<double> b, vector<double> c, int numOfAritificals, vector<double> &w, vector<int> &basicVarIndex)
{
  // build decision variables
  for (int i = 0; i < tableu.size(); i++)
  {
    for (int j = 0; j < m; j++)
    {
      if (i == tableu.size() - 1)
      {
        tableu[i][j] = c[j] * -1;
      }
      else
      {
        if (b[i] < 0)
        {
          tableu[i][j] = A[i][j] * -1;
        }
        else
        {
          tableu[i][j] = A[i][j];
        }
      }
    }
  }

  // build last column (a.k.a Answer Column)
  int lastColIndex = tableu[0].size() - 1;
  for (int i = 0; i < tableu.size(); i++)
  {
    if (i == tableu.size() - 1)
    {
      tableu[i][lastColIndex] = 0;
    }
    else
    {
      if (b[i] < 0)
      {
        tableu[i][lastColIndex] = b[i] * -1;
      }
      else
      {
        tableu[i][lastColIndex] = b[i];
      }
    }
  }

  // build slack and aritificial variables
  for (int i = 0; i < tableu.size(); i++)
  {
    // exclude last row which stores the original objective function
    if (i == tableu.size() - 1)
    {
      break;
    }

    for (int j = i + m; j < tableu[0].size() - 1; j++)
    {
      if (b[i] < 0)
      {
        tableu[i][j] = -1;
        int artificalVarColIndex = lastColIndex - numOfAritificals;
        tableu[i][artificalVarColIndex] = 1;
        numOfAritificals--;
        artificialVarsIndex.push_back({i, artificalVarColIndex});
        basicVarIndex[i] = artificalVarColIndex;
        break;
      }
      else
      {
        tableu[i][j] = 1;
        basicVarIndex[i] = j;
        break;
      }
    }
  }

  // build artifical objective row
  for (int i = 0; i < artificialVarsIndex.size(); i++)
  {
    for (int j = 0; j < tableu[0].size(); j++)
    {
      Position pos = artificialVarsIndex[i];
      if (pos.column != j)
      {
        w[j] = w[j] + tableu[pos.row][j];
      }
    }
  }
}

int simplexPhase1(int n, int m, matrix &tableu, vector<double> &w, vector<int> &basicVarIndex)
{
  int result = 0;

  while (true)
  {
    // choose pivot column, look for the most positive int in artificial row
    int pivotColumn = -1;
    int pivotColumnValue = -1;
    for (int i = 0; i < w.size() - 1 - 1; i++) // last column a.k.a Answer Column doesn't count
    {
      if (w[i] > pivotColumnValue && w[i] > 0)
      {
        pivotColumn = i;
        pivotColumnValue = w[i];
      }
    }

    // choose pivot row
    int pivotRow = -1;
    int lastColIndex = tableu[0].size() - 1;
    double ratio = INT_MAX;
    for (int i = 0; i < tableu.size() - 1; i++) // last row doesn't count because it stores the original objective, hence another - 1 expression
    {
      if (tableu[i][pivotColumn] > 0)
      {
        double rowPivotRatio = tableu[i][lastColIndex] / tableu[i][pivotColumn];
        if (rowPivotRatio < ratio)
        {
          pivotRow = i;
          ratio = rowPivotRatio;
        }
      }
    }

    if (pivotColumn == -1 || pivotRow == -1)
    {
      for (int i = 0; i < artificialVarsIndex.size(); i++)
      {
        Position index = artificialVarsIndex[i];
        if (tableu[index.row][index.column] >= 1)
        {
          result = -1;
        }
      }

      break;
    }

    // pivot phase 1
    cout << "phase 1 pivoting on row " << pivotRow << " column " << pivotColumn;
    double tableuPivotPointCoeff = tableu[pivotRow][pivotColumn];
    if (tableuPivotPointCoeff != 1)
    {
      for (int i = 0; i < tableu[0].size(); i++)
      {
        tableu[pivotRow][i] = tableu[pivotRow][i] / tableuPivotPointCoeff;
      }
    }
    basicVarIndex[pivotRow] = pivotColumn;

    for (int i = 0; i < tableu.size(); i++)
    {
      if (i == pivotRow)
      {
        continue;
      }
      double rowToClearCoeff = tableu[i][pivotColumn];
      double artificialRowToClearCoeff = w[pivotColumn];

      for (int j = 0; j < tableu[0].size(); j++)
      {
        tableu[i][j] = tableu[i][j] - (rowToClearCoeff * tableu[pivotRow][j]);
        w[j] = w[j] - (artificialRowToClearCoeff * tableu[pivotRow][j]);
      }
    }
    cout << "\n===================================================\n";
    printTableu(tableu);
    cout << "\n=========== artifical vector row ====================\n";
    printVector(w);
  }

  return result;
}

int simplexPhase2(int n, int m, matrix &tableu, vector<int> &basicVarIndex, int numberOfArtificials)
{
  int result = 0;

  while (true)
  {
    // choose pivot column, look for the most positive int in artificial row
    int pivotColumn = -1;
    int pivotColumnValue = INT_MAX;
    int originalObjectiveRow = tableu.size() - 1;
    for (int i = 0; i <= tableu[0].size() - 1 - numberOfArtificials - 1; i++) // artificial column and last column a.k.a Answer Column doesn't count
    {
      if (tableu[originalObjectiveRow][i] < pivotColumnValue && tableu[originalObjectiveRow][i] < 0)
      {
        pivotColumn = i;
        pivotColumnValue = tableu[originalObjectiveRow][i];
      }
    }

    // choose pivot row
    int pivotRow = -1;

    if (pivotColumn == -1)
    {
      // if there is no pivot column selected, do nothing and proceed
    } else {
      int lastColIndex = tableu[0].size() - 1;
      double ratio = INT_MAX;
      for (int i = 0; i < tableu.size() - 1; i++) // last row doesn't count because it stores the original objective, hence another - 1 expression
      {
        if (tableu[i][pivotColumn] > 0)
        {
          double rowPivotRatio = tableu[i][lastColIndex] / tableu[i][pivotColumn];
          if (rowPivotRatio < ratio)
          {
            pivotRow = i;
            ratio = rowPivotRatio;
          }
        }
      }
    }

    if (pivotColumn == -1 || pivotRow == -1)
    {
      bool isInfinity = false;
      for (int i = 0; i < (m + n); i++)
      {
        if (tableu[tableu.size() - 1][i] < 0)
        {
          isInfinity = true;
        }
      }

      if (isInfinity)
      {
        result = 1;
      }
      break;
    }

    // pivot phase 2
    cout << "phase 2 pivoting on row " << pivotRow << " column " << pivotColumn;
    double tableuPivotPointCoeff = tableu[pivotRow][pivotColumn];
    if (tableuPivotPointCoeff != 1)
    {
      for (int i = 0; i < tableu[0].size(); i++)
      {
        tableu[pivotRow][i] = tableu[pivotRow][i] / tableuPivotPointCoeff;
      }
    }
    basicVarIndex[pivotRow] = pivotColumn;

    for (int i = 0; i < tableu.size(); i++)
    {
      if (i == pivotRow)
      {
        continue;
      }
      double rowToClearCoeff = tableu[i][pivotColumn];

      for (int j = 0; j < tableu[0].size(); j++)
      {
        tableu[i][j] = tableu[i][j] - (rowToClearCoeff * tableu[pivotRow][j]);
      }
    }
    cout << "\n===================================================\n";
    printTableu(tableu);
  }

  return result;
}

pair<int, vector<double>> allocate_ads(
    int n,
    int m,
    matrix A,
    vector<double> b,
    vector<double> c)
{
  int solution = 0;
  vector<double> result(m, 0);

  int tableuPhase1Row = n + 1;     //number of constraints + Objective row
  int tableuPhase1Col = m + n + 1; // 1 column for every constraint, 1 column for every decision variable, 1 column for Answer
  int numberOfArtificials = 0;

  for (int i = 0; i < b.size(); i++)
  {
    if (b[i] < 0)
    {
      tableuPhase1Col = tableuPhase1Col + 1; // adding a new column for every artifical variable for >= constraint
      numberOfArtificials++;
    }
  }

  matrix tableuPhase1(tableuPhase1Row, vector<double>(tableuPhase1Col, 0));
  vector<double> w(tableuPhase1[0].size(), 0); // aritifical objective function w
  vector<int> basicVarIndex(n);
  buildPhase1Tableu(n, m, tableuPhase1, A, b, c, numberOfArtificials, w, basicVarIndex);

  printTableu(tableuPhase1);
  cout << "\n=========== artifical vector row ====================\n";
  printVector(w);
  cout << "\n===================================================\n";

  int phase1Res = simplexPhase1(n, m, tableuPhase1, w, basicVarIndex);
  if (phase1Res == 0)
  {
    cout << "\n===================================================\n";
    int phase2Res = simplexPhase2(n, m, tableuPhase1, basicVarIndex, numberOfArtificials);
    if (phase2Res == 0)
    {
      for (int i = 0; i < basicVarIndex.size(); i++)
      {
        int indexOfBasicVar = basicVarIndex[i];
        if (indexOfBasicVar < m)
        {
          result[indexOfBasicVar] = tableuPhase1[i][tableuPhase1[0].size() - 1];
        }
      }
    }
    else
    {
      solution = phase2Res;
    }
  }
  else
  {
    solution = phase1Res;
  }

  return {solution, result};
}

int main()
{
  std::fstream file("./tests/own_04");

  if (file.is_open())
  {
    cin.rdbuf(file.rdbuf());
  }

  int n, m;
  cin >> n >> m;
  matrix A(n, vector<double>(m));
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      cin >> A[i][j];
    }
  }
  vector<double> b(n);
  for (int i = 0; i < n; i++)
  {
    cin >> b[i];
  }
  vector<double> c(m);
  for (int i = 0; i < m; i++)
  {
    cin >> c[i];
  }

  pair<int, vector<double>> ans = allocate_ads(n, m, A, b, c);

  switch (ans.first)
  {
  case -1:
    printf("No solution\n");
    break;
  case 0:
    printf("Bounded solution\n");
    for (int i = 0; i < m; i++)
    {
      printf("%.18f%c", ans.second[i], " \n"[i + 1 == m]);
    }
    break;
  case 1:
    printf("Infinity\n");
    break;
  }
  return 0;
}
