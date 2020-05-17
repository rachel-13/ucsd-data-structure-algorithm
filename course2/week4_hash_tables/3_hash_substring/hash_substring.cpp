#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>

using std::cin;
using std::string;
typedef unsigned long long ull;

struct Data
{
    string pattern, text;
};

long long hash_string(const string &S)
{
    // hashing formula
    // sum from i = 0 to S.length() - 1
    // S[i] * (x ^ i) mod p
    long long result = 0;
    long long L = pow(10, 8); // maximum length of data to hash, in this question it is 8
    long long p = L + 19;     // prime number bigger than 10 ^ L
    long long x = 31;         // random number 1 < x < p - 1

    for (int i = S.length() - 1; i >= 0; i--)
    {
        result = (result * x + S[i]) % p;
    }

    return result;
}

Data read_input()
{
    // std::fstream file("./tests/03");
    // if (file.is_open())
    // {
    //     cin.rdbuf(file.rdbuf());
    // }

    Data data;
    std::cin >> data.pattern >> data.text;
    return data;
}

void print_occurrences(const std::vector<int> &output)
{
    for (size_t i = 0; i < output.size(); ++i)
        std::cout << output[i] << " ";
    std::cout << "\n";
}

std::vector<int> get_occurrences_naive(const Data &input)
{
    const string &s = input.pattern, t = input.text;
    std::vector<int> ans;
    for (size_t i = 0; i + s.size() <= t.size(); ++i)
        if (t.substr(i, s.size()) == s)
            ans.push_back(i);
    return ans;
}

bool are_equal(string s1, string s2)
{
    if (s1.length() != s2.length())
    {
        return false;
    }

    for (int i = 0; i < s1.length() - 1; i++)
    {
        if (s1[i] != s2[i])
        {
            return false;
        }
    }

    return true;
}

std::vector<int> get_occurrences(const Data &input)
{
    const string &s = input.pattern, t = input.text;
    std::vector<int> ans;

    long long patternHash = hash_string(s);

    int textPatternLengthDiff = t.length() - s.length();
    string textToCompare = "";
    int startIndex = 0;

    for (int i = 0; i <= textPatternLengthDiff; i++)
    {
        int endIndex = i + s.length() - 1;
        if (textToCompare.length() == s.length())
        {
            textToCompare.erase(0, 1);
            textToCompare = textToCompare + t[endIndex];
        }
        else
        {
            textToCompare = t.substr(startIndex, endIndex + 1);
        }

        startIndex = startIndex + 1;

        long long textHash = hash_string(textToCompare);
        if (textHash != patternHash)
        {
            continue;
        }

        if (are_equal(textToCompare, s))
        {
            ans.push_back(i);
        }
    }

    return ans;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    print_occurrences(get_occurrences(read_input()));
    return 0;
}
