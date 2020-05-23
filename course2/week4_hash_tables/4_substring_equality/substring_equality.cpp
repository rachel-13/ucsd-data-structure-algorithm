#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;
using std::vector;

class Solver
{
	string s;

public:
	vector<long long> h1;
	vector<long long> h2;
	int m1 = pow(10, 9) + 7;
	int m2 = pow(10, 9) + 9;
	int x = 31;

	Solver(string s) : s(s)
	{
		// initialization, precalculation
		h1 = vector<long long>(s.length() + 1);
		h2 = vector<long long>(s.length() + 1);

		h1[0] = 0;
		h2[0] = 0;

		for (int i = 1; i <= s.length(); i++)
		{
			// formula: ℎ1[𝑖] ← (𝑥 · ℎ1[𝑖 − 1] + s𝑖) mod 𝑚1
			h1[i] = (x * h1[i - 1] + s[i - 1]) % m1;
			h2[i] = (x * h2[i - 1] + s[i - 1]) % m2;
		}
	}

	bool ask(int a, int b, int l)
	{
		// formula: 𝐻(s𝑎s𝑎+1 · · · s𝑎+𝑙−1) = ℎ[𝑎 + 𝑙] − (𝑥^𝑙)ℎ[𝑎]
		long long x_pow_l = pow(x, l);

		// calculate for mod m1
		long long substringHash_a_m1 = h1[a + l];
		long long a_m1 = h1[a];
		long long x_pow_l_a_hash_m1 = (x_pow_l * a_m1) % m1;
		long long hash_a_m1 = (((substringHash_a_m1 - x_pow_l_a_hash_m1) % m1) + m1) % m1;

		long long substringHash_b_m1 = h1[b + l];
		long long b_m1 = h1[b];
		long long x_pow_l_b_hash_m1 = (x_pow_l * b_m1) % m1;
		long long hash_b_m1 = (((substringHash_b_m1 - x_pow_l_b_hash_m1) % m1) + m1) % m1;

		// calculate for mod m2
		long long substringHash_a_m2 = h2[a + l];
		long long a_m2 = h2[a];
		long long x_pow_l_a_hash_m2 = (x_pow_l * a_m2) % m2;
		long long hash_a_m2 = (((substringHash_a_m2 - x_pow_l_a_hash_m2) % m2) + m2) % m2;

		long long substringHash_b_m2 = h2[b + l];
		long long b_m2 = h2[b];
		long long x_pow_l_b_hash_m2 = (x_pow_l * b_m2) % m2;
		long long hash_b_m2 = (((substringHash_b_m2 - x_pow_l_b_hash_m2) % m2) + m2) % m2;

		return hash_a_m1 == hash_b_m1 && hash_a_m2 == hash_b_m2;
	}

	bool ask_naive(int a, int b, int l)
	{
		return s.substr(a, l) == s.substr(b, l);
	}
};

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
		int startIndex1 = rand() % 10;
		int startIndex2 = rand() % 10;
		int textLength = rand() % (max(startIndex1, startIndex2)) + 10;
		string text =  printstring(textLength);
		int length = textLength - max(startIndex1, startIndex2);
		

		Solver solver(text);

		bool res1 = solver.ask_naive(startIndex1, startIndex2, length);
		bool res2 = solver.ask(startIndex1, startIndex2, length);

		if (res1 == res2)
		{
			cout << "\n====================================================\n";
			cout << "\nOK!\n";

			cout << res1 << " vs " << res2 << "\n";

			cout << "\n"
				 << text << "\n";
			cout << startIndex1 << " " << startIndex2 << " " << length << "\n";
		}
		else
		{
			cout << "\n====================================================\n";
			cout << "\nFail!\n";

			cout << res1 << " vs " << res2 << "\n";

			cout << "\n"
				 << text << "\n";
			cout << startIndex1 << " " << startIndex2 << " " << length << "\n";
			break;
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(0), cin.tie(0);

	fstream cin("./tests/02");
	// test_solution();

	string s;
	int q;
	cin >> s >> q;
	Solver solver(s);
	for (int i = 0; i < q; i++)
	{
		int a, b, l;
		cin >> a >> b >> l;
		cout << (solver.ask(a, b, l) ? "Yes\n" : "No\n");
	}
}
