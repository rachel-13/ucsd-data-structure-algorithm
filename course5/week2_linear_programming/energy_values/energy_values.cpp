#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

const double EPS = 1e-6;
const int PRECISION = 10;

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

struct Equation
{
    Equation(const Matrix &a, const Column &b) : a(a),
                                                 b(b)
    {
    }

    Matrix a;
    Column b;
};

struct Position
{
    Position(int column, int row) : column(column),
                                    row(row)
    {
    }

    int column;
    int row;
};

Equation ReadEquation()
{

    // std::fstream file("./tests/05");

    // if (file.is_open())
    // {
    //     std::cin.rdbuf(file.rdbuf());
    // }

    int size;
    std::cin >> size;
    Matrix a(size, std::vector<double>(size, 0.0));
    Column b(size, 0.0);
    for (int row = 0; row < size; ++row)
    {
        for (int column = 0; column < size; ++column)
            std::cin >> a[row][column];
        std::cin >> b[row];
    }
    return Equation(a, b);
}

Position SelectPivotElement(
    const Matrix &a,
    std::vector<bool> &used_rows,
    std::vector<bool> &used_columns)
{
    Position pivot_element(0, 0);
    while (used_rows[pivot_element.row])
        ++pivot_element.row;
    while (used_columns[pivot_element.column])
        ++pivot_element.column;

    int nonZeroPivotRow = pivot_element.row;
    if (a[pivot_element.row][pivot_element.column] == 0)
    {
        for (int i = 0; i < a.size(); i++)
        {
            if (a[i][pivot_element.column] != 0)
            {
                nonZeroPivotRow = a[i][pivot_element.column];
                break;
            }
        }
    }

    pivot_element.row = nonZeroPivotRow;
    
    return pivot_element;
}

void SwapLines(Matrix &a, Column &b, std::vector<bool> &used_rows, Position &pivot_element)
{
    std::swap(a[pivot_element.column], a[pivot_element.row]);
    std::swap(b[pivot_element.column], b[pivot_element.row]);
    std::swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column;
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element)
{
    // std::cout << "pivoting at row " << pivot_element.row << " column " << pivot_element.column << "\n";
    double pivotCoeff = a[pivot_element.row][pivot_element.column];

    if (std::abs(pivotCoeff) > 1)
    {
        for (int i = 0; i < a[0].size(); i++)
        {
            a[pivot_element.row][i] = (double)(a[pivot_element.row][i] / pivotCoeff);
        }
        b[pivot_element.row] = (double)(b[pivot_element.row] / pivotCoeff);
    }

    int pivotElementColumn = pivot_element.column;
    int pivotElementRow = pivot_element.row;

    for (int i = 0; i < a.size(); i++)
    {
        if (i == pivotElementRow)
        {
            continue;
        }

        double dividingCoeff = a[i][pivotElementColumn];
        for (int j = 0; j < a[0].size(); j++)
        {
            a[i][j] = a[i][j] - (a[pivotElementRow][j] * dividingCoeff);
        }

        b[i] = b[i] - (b[pivotElementRow] * dividingCoeff);
    }
}

void MarkPivotElementUsed(const Position &pivot_element, std::vector<bool> &used_rows, std::vector<bool> &used_columns)
{
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

Column SolveEquation(Equation equation)
{
    Matrix &a = equation.a;
    Column &b = equation.b;
    int size = a.size();

    std::vector<bool> used_columns(size, false);
    std::vector<bool> used_rows(size, false);
    for (int step = 0; step < size; ++step)
    {
        Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
        SwapLines(a, b, used_rows, pivot_element);
        ProcessPivotElement(a, b, pivot_element);
        MarkPivotElementUsed(pivot_element, used_rows, used_columns);
    }

    return b;
}

void PrintColumn(const Column &column)
{
    int size = column.size();
    std::cout.precision(PRECISION);
    std::cout << std::showpoint;
    for (int row = 0; row < size; ++row)
    {
        std::cout << column[row] << " ";
    }
}

int main()
{
    Equation equation = ReadEquation();
    Column solution = SolveEquation(equation);
    PrintColumn(solution);
    return 0;
}
