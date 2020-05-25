#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

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

	string textToPolyHash = text.substr(textPatternLengthDiff, text.length() - 1);
	hash[textPatternLengthDiff] = hash_string(textToPolyHash, x, p);

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

Hash_Result precompute_hash_for_s_t(const string &s, const string &t, size_t patternLength)
{
	Hash_Result result = {false, (size_t)-1, (size_t)-1};
	vector<long long> shorter_text_hash;
	vector<long long> longer_text_hash;
	int m1 = pow(10, 9) + 7;
	int x = 31;

	if (s.length() < t.length())
	{
		shorter_text_hash = precompute_hashes(s, patternLength, x, m1);
		longer_text_hash = precompute_hashes(t, patternLength, x, m1);
	}
	else
	{
		shorter_text_hash = precompute_hashes(t, patternLength, x, m1);
		longer_text_hash = precompute_hashes(s, patternLength, x, m1);
	}

	for (size_t i = 0; i < longer_text_hash.size(); i++)
	{
		vector<long long>::iterator it = find(shorter_text_hash.begin(), shorter_text_hash.end(), longer_text_hash[i]);
		if (it != shorter_text_hash.end())
		{
			if (s.length() < t.length())
			{	// i and j needs to correspond to the string input that was given
				// but since we swapped positions to calculate hash of shorter string, we need to re-swap it
				size_t j = std::distance(shorter_text_hash.begin(), it);
				result = {true, j, i, patternLength};
			}
			else
			{
				size_t j = std::distance(shorter_text_hash.begin(), it);
				result = {true, i, j, patternLength};
			}
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
		int l = right - left;
		Hash_Result h_res = precompute_hash_for_s_t(s, t, l);
		if (h_res.found == true && h_res.l > ans.len)
		{
			ans = {h_res.i, h_res.j, h_res.l};
		}
		left = left + 1;
	}

	return ans;
}

int main()
{
	ios_base::sync_with_stdio(false), cin.tie(0);

	// std::fstream cin("./tests/02");

	string s, t;
	while (cin >> s >> t)
	{
		// if (s == "//")
		// 	break;
		auto ans = solve(s, t);
		cout << ans.i << " " << ans.j << " " << ans.len << "\n";
	}
}
