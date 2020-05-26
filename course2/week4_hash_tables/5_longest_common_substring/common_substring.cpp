#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <unordered_map>

using namespace std;

using std::cout;
using std::vector;

struct Answer
{
	size_t i, j, len;
};

struct Hash_Result
{
	bool found;
	size_t i;
	size_t j;
	size_t l;
};

Answer solve_naive(const string &s, const string &t)
{
	Answer ans = {0, 0, 0};
	for (size_t i = 0; i < s.size(); i++)
		for (size_t j = 0; j < t.size(); j++)
			for (size_t len = 0; i + len <= s.size() && j + len <= t.size(); len++)
				if (len > ans.len && s.substr(i, len) == t.substr(j, len))
					ans = {i, j, len};
	return ans;
}

long long hash_string(const string &S, int x, long long p)
{
	// hashing formula
	// sum from i = 0 to S.length() - 1
	// S[i] * (x ^ i) mod p
	long long result = 0;

	for (int i = S.length() - 1; i >= 0; i--)
	{
		result = (result * x + S[i]) % p;
	}

	return result;
}

vector<long long> precompute_hashes(string text, int patternLength, int x, long long p)
{

	int textPatternLengthDiff = text.length() - patternLength;
	vector<long long> hash(textPatternLengthDiff + 1);

	string textToPolyHash = text.substr(textPatternLengthDiff, text.length());
	hash[textPatternLengthDiff] = hash_string(textToPolyHash, x, p);

	long long y = 1;
	for (int i = 1; i <= patternLength; i++)
	{
		y = (y * x) % p;
	}

	for (int j = textPatternLengthDiff - 1; j >= 0; j--)
	{
		// formula:  H[i] ← (xH[i + 1] + T[i] − yT[i + |P|]) mod p
		long long sub1 = (x % p) * (hash[j + 1] % p);
		long long sub2 = (y % p) * (text[j + patternLength] % p);
		long long formula = (sub1 + text[j] - sub2);
		hash[j] = ((formula % p) + p) % p;
	}

	return hash;
}

Hash_Result precompute_hash_for_s_t(const string &s, const string &t, size_t patternLength)
{
	Hash_Result result = {false, (size_t)-1, (size_t)-1};
	vector<long long> shorter_text_hash_m1;
	vector<long long> longer_text_hash_m1;
	vector<long long> shorter_text_hash_m2;
	vector<long long> longer_text_hash_m2;
	vector<long long> shorter_text_hash_m3;
	vector<long long> longer_text_hash_m3;
	std::unordered_map<long long, int> h_table_m1;
	std::unordered_map<long long, int> h_table_m2;
	std::unordered_map<long long, int> h_table_m3;

	int m1 = pow(10, 9) + 7;
	int m2 = pow(10, 9) + 9;
	int m3 = pow(10, 9) + 31;

	int x = 31;

	if (s.length() < t.length())
	{
		shorter_text_hash_m1 = precompute_hashes(s, patternLength, x, m1);
		longer_text_hash_m1 = precompute_hashes(t, patternLength, x, m1);

		shorter_text_hash_m2 = precompute_hashes(s, patternLength, x, m2);
		longer_text_hash_m2 = precompute_hashes(t, patternLength, x, m2);

		shorter_text_hash_m3 = precompute_hashes(s, patternLength, x, m3);
		longer_text_hash_m3 = precompute_hashes(t, patternLength, x, m3);
	}
	else
	{
		shorter_text_hash_m1 = precompute_hashes(t, patternLength, x, m1);
		longer_text_hash_m1 = precompute_hashes(s, patternLength, x, m1);

		shorter_text_hash_m2 = precompute_hashes(t, patternLength, x, m2);
		longer_text_hash_m2 = precompute_hashes(s, patternLength, x, m2);

		shorter_text_hash_m3 = precompute_hashes(t, patternLength, x, m3);
		longer_text_hash_m3 = precompute_hashes(s, patternLength, x, m3);
	}

	for (int i = 0; i < shorter_text_hash_m1.size(); i++)
	{
		h_table_m1[shorter_text_hash_m1[i]] = i;
		h_table_m2[shorter_text_hash_m2[i]] = i;
		h_table_m3[shorter_text_hash_m3[i]] = i;
	}

	for (size_t i = 0; i < longer_text_hash_m1.size(); i++)
	{
		if (h_table_m1.find(longer_text_hash_m1[i]) != h_table_m1.end() &&
			h_table_m2.find(longer_text_hash_m2[i]) != h_table_m2.end() &&
			h_table_m3.find(longer_text_hash_m3[i]) != h_table_m3.end())
		{
			result = {true, (size_t)h_table_m1[longer_text_hash_m1[i]], i, patternLength};
		}
	}

	return result;
}

Answer solve(const string &s, const string &t)
{
	Answer ans = {0, 0, 0};

	int left = 0;
	int right = min(s.length(), t.length());
	while (left <= right)
	{
		size_t mid = (left + right) / 2;
		Hash_Result h_res = precompute_hash_for_s_t(s, t, mid);
		if (h_res.found)
		{
			left = mid + 1;
			ans = {h_res.i, h_res.j, mid};
		}
		else
		{
			right = mid - 1;
		}
	}

	// since we swapped s and t in order to only calculate the shorter string
	// we will need to re-swap this if we swapped it while computing hashes
	if (s.length() >= t.length())
	{
		ans = {ans.j, ans.i, ans.len};
	}

	return ans;
}

string printstring(int n)
{
	char letters[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
						'r', 's', 't', 'u', 'v', 'w', 'x',
						'y', 'z'};
	string ran = "";
	for (int i = 0; i < n; i++)
		ran = ran + letters[rand() % 3];
	return ran;
}

void test_solution()
{
	while (true)
	{
		int textLength1 = rand() % 10 + 5;
		int textLength2 = rand() % 10 + 5;

		// build dataset
		string text1 = printstring(textLength1);
		string text2 = printstring(textLength2);

		Answer res1 = solve_naive(text1, text2);
		Answer res2 = solve(text1, text2);

		// get results
		if (res1.i == res2.i && res1.j == res2.j && res1.len == res2.len)
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

			cout << text1 << " " << text2 << "\n";

			cout << res1.i << " " << res1.j << " " << res1.len << "\n";

			cout << "\nvs\n";

			cout << res2.i << " " << res2.j << " " << res2.len << "\n";

			break;
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false), cin.tie(0);

	// std::fstream cin("./tests/02");
	// test_solution();

	string s, t;
	while (cin >> s >> t)
	{
		// if (s == "//")
		// 	break;
		auto ans = solve(s, t);
		cout << ans.i << " " << ans.j << " " << ans.len << "\n";
	}
}
