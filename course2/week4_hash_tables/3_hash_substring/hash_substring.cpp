#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <math.h>

using std::cin;
using std::cout;
using std::string;
using std::vector;
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

vector<long long> precompute_hashes(string text, int patternLength)
{
    long long L = pow(10, 8); // maximum length of data to hash, in this question it is 8
    long long p = L + 19;     // prime number bigger than 10 ^ L
    long long x = 31;         // random number 1 < x < p - 1

    int textPatternLengthDiff = text.length() - patternLength;
    vector<long long> hash(textPatternLengthDiff + 1);

    string textToPolyHash = text.substr(textPatternLengthDiff, text.length() - 1);
    hash[textPatternLengthDiff] = hash_string(textToPolyHash);

    long long y = 1;
    for (int i = 1; i <= patternLength; i++)
    {
        y = (y * x) % p;
    }

    for (int j = textPatternLengthDiff - 1; j >= 0; j--)
    {
        // formula:  H[i] ← (xH[i + 1] + T[i] − yT[i + |P|]) mod p
        long long formula = (x * hash[j + 1]) + text[j] - (y * text[j + patternLength]);
        hash[j] = ((formula % p) + p) % p;
    }

    return hash;
}

Data read_input()
{
    // std::fstream file("./tests/04");
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
    return s1 == s2;
}

std::vector<int> get_occurrences(const Data &input)
{
    const string &s = input.pattern, t = input.text;
    std::vector<int> ans;

    if (s == t)
    {
        ans.push_back(0);
        return ans;
    }

    vector<long long> precomputedHashes = precompute_hashes(t, s.length());
    long long patternHash = hash_string(s);

    int textPatternLengthDiff = t.length() - s.length();
    string textToCompare = "";

    for (int i = 0; i <= textPatternLengthDiff; i++)
    {
        long long textHash = precomputedHashes[i];
        if (textHash != patternHash)
        {
            continue;
        }

        textToCompare = t.substr(i, s.length());
        if (are_equal(textToCompare, s))
        {
            ans.push_back(i);
        }
    }

    return ans;
}

string printstring(int n)
{
    char letters[52] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                        'r', 's', 't', 'u', 'v', 'w', 'x',
                        'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
                        'R', 'S', 'T', 'U', 'V', 'W', 'X',
                        'Y', 'Z'};
    string ran = "";
    for (int i = 0; i < n; i++)
        ran = ran + letters[rand() % 52];
    return ran;
}

void test_solution()
{
    while (true)
    {
        int textLength = rand() % 1000 + 5;
        int patternLength = textLength - (rand() % textLength);
        int patternStartIndex = textLength % patternLength;

        // build dataset
        string text = printstring(textLength);
        string pattern = text.substr(patternStartIndex, patternStartIndex + patternLength);

        int randomIteration = rand() % 20;
        for (int i = 0; i < randomIteration; i++)
        {
            if (i % 3 == 0)
            {
                text = text + pattern;
            }
            else
            {
                text = text + printstring(rand() % 10);
            }
        }

        Data data = Data();
        data.text = text;
        data.pattern = pattern;

        vector<int> res1 = get_occurrences(data);
        vector<int> res2 = get_occurrences(data);

        // get results
        if (res1 == res2)
        {
            cout << "\n================================================\n";
            cout << "OK!"
                 << "\n";
        }
        else
        {
            cout << "\n================================================\n";
            cout << "Fail!"
                 << "\n";

            for (int i = 0; i < res1.size(); i++)
            {
                cout << res1[i] << " ";
            }

            cout << "\nvs\n";

            for (int i = 0; i < res2.size(); i++)
            {
                cout << res2[i] << " ";
            }

            break;
        }
    }
}

int main()
{
    // test_solution();
    std::ios_base::sync_with_stdio(false);
    print_occurrences(get_occurrences(read_input()));
    return 0;
}
