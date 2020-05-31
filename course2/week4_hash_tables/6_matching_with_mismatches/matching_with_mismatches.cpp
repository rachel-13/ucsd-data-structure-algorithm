#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

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

vector<long long> h1_t;
vector<long long> h1_p;
vector<long long> t_partial_sum;
vector<long long> p_partial_sum;
vector<long long> x_pow_l_m1;
int m1 = pow(10, 9) + 7;
int x = 31;

void precompute_power_for_length(int length)
{
	long long value_m1 = 1;

	x_pow_l_m1.clear();

	x_pow_l_m1.push_back(1);

	while (length > 0)
	{
		value_m1 *= x;
		value_m1 %= m1;
		x_pow_l_m1.push_back(value_m1);

		length--;
	}
}

void precompute_prefix(string t, string p)
{
	h1_t = vector<long long>(t.length() + 1);
	h1_p = vector<long long>(p.length() + 1);
	t_partial_sum = vector<long long>(t.length() + 1);
	p_partial_sum = vector<long long>(p.length() + 1);

	t_partial_sum[0] = 0;
	p_partial_sum[0] = 0;
	h1_t[0] = 0;
	h1_p[0] = 0;

	for (int i = 1; i <= t.length(); i++)
	{
		// formula: ℎ1[𝑖] ← (𝑥 · ℎ1[𝑖 − 1] + s𝑖) mod 𝑚1
		long long x_h1_t = ((x % m1) * (h1_t[i - 1] % m1)) % m1;
		h1_t[i] = (x_h1_t + t[i - 1]) % m1;
		t_partial_sum[i] = t_partial_sum[i - 1] + t[i - 1];
	}

	for (int i = 1; i <= p.length(); i++)
	{
		long long x_h1_p = ((x % m1) * (h1_p[i - 1] % m1)) % m1;
		h1_p[i] = (x_h1_p + p[i - 1]) % m1;
		p_partial_sum[i] = p_partial_sum[i - 1] + p[i - 1];
	}
}

vector<int> result;
void find_next_mismatch(int t_left, int t_right, int p_left, int p_right)
{
	int length = t_right - t_left;
	int t_mid = (t_left + t_right) / 2;
	int p_mid = (p_left + p_right) / 2;

	if (t_right <= t_left + 1)
	{
		if (t_right == t_left)
		{
			long long substringHash_t_m1 = h1_t[t_right] % m1;
			long long x_pow_l_t_hash_m1 = (x_pow_l_m1[length] * (h1_t[t_right - 1] % m1)) % m1;
			long long hash_t_m1 = (((substringHash_t_m1 - x_pow_l_t_hash_m1) % m1) + m1) % m1;

			long long substringHash_p_m1 = h1_p[p_right] % m1;
			long long x_pow_l_p_hash_m1 = (x_pow_l_m1[length] * (h1_p[p_right - 1] % m1)) % m1;
			long long hash_p_m1 = (((substringHash_p_m1 - x_pow_l_p_hash_m1) % m1) + m1) % m1;

			if (hash_t_m1 != hash_p_m1)
			{
				result.push_back(t_right - 1);
			}
		}
		else
		{
			long long substringHash_t_m1 = h1_t[t_right] % m1;
			long long x_pow_l_t_hash_m1 = (x_pow_l_m1[length] * (h1_t[t_right - length] % m1)) % m1;
			long long hash_t_m1 = (((substringHash_t_m1 - x_pow_l_t_hash_m1) % m1) + m1) % m1;

			long long substringHash_p_m1 = h1_p[p_right] % m1;
			long long x_pow_l_p_hash_m1 = (x_pow_l_m1[length] * (h1_p[p_right - length] % m1)) % m1;
			long long hash_p_m1 = (((substringHash_p_m1 - x_pow_l_p_hash_m1) % m1) + m1) % m1;

			if (hash_t_m1 != hash_p_m1)
			{
				result.push_back(t_left);
			}
		}
	}
	else
	{
		find_next_mismatch(t_left, t_mid, p_left, p_mid);
		find_next_mismatch(t_mid, t_right, p_mid, p_right);
	}
}

vector<int> solve(int k, const string &text, const string &pattern)
{
	vector<int> pos;
	precompute_prefix(text, pattern);
	precompute_power_for_length(text.length());

	int difference = text.length() - pattern.length();
	int left = 0;
	int right = left + pattern.length();
	int found_mismatch = 0;

	while (left <= difference)
	{
		find_next_mismatch(left, right, 0, pattern.length());

		if (result.size() <= k)
		{
			pos.push_back(left);
		}

		left = left + 1;
		right = right + 1;
		result.clear();
	}

	return pos;
}

int main()
{
	ios_base::sync_with_stdio(false), cin.tie(0);

	// fstream cin("./tests/01");

	int k;
	string t, p;
	while (cin >> k >> t >> p)
	{
		auto a = solve(k, t, p);
		cout << a.size();
		for (int x : a)
			cout << " " << x;
		cout << "\n";
	}
}
