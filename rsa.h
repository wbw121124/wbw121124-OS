#include<cmath>
#include<cstdlib>
#include<time.h>
#include<string>
#include<vector>
using namespace std;

namespace RSA {
	#define int long long
	// 快速幂
	long long pow(long long base, long long exponent, long long mod) {
		long long result = 1;
		while (exponent > 0) {
			if (exponent % 2 == 1) {
				result = result * base % mod;
			}
			base = base * base % mod;
			exponent = exponent / 2;
		}
		return result;
	}

	// 求最大公约数
	int gcd(int a, int b) {
		if (b == 0) {
			return a;
		}
		return gcd(b, a % b);
	}

	// 求模逆
	int modInverse(int a, int m) {
		a = a % m;
		for (int x = 1; x < m; x++) {
			if ((a * x) % m == 1) {
				return x;
			}
		}
		return 1;
	}

	// 求欧拉函数
	int euler(int n) {
		int result = 1;
		for (int i = 2; i * i <= n; i++) {
			if (n % i == 0) {
				result *= (i - 1);
				while (n % i == 0) {
					n /= i;
				}
			}
		}
		if (n > 1) {
			result *= (n - 1);
		}
		return result;
	}

	// 求素数
	bool isPrime(int n) {
		if (n <= 1) {
			return false;
		}
		for (int i = 2; i * i <= n; i++) {
			if (n % i == 0) {
				return false;
			}
		}
		return true;
	}

	vector<string> split(string command, char c)
	{
		vector<string> ans;
		int last = 0;
		for (int i = 0; i < command.size(); i++)
		{
			if (command[i] == c)
			{
				ans.push_back(command.substr(last, i - last));
				last = i + 1;
			}
		}
		ans.push_back(command.substr(last, command.size() - last));
		return ans;
	}

	// 生成RSA密钥对
	void generateRSAKeys(int& p, int& q, int& n, int& e, int& d, bool pqisprime = false) {
		srand(time(0));
		if (!pqisprime) {
			do {
				p = rand() % 100 + 2;
			} while (!isPrime(p));
			do {
				q = rand() % 100 + 2;
			} while (!isPrime(q));
		}
		n = p * q;
		int phi = euler(p) * euler(q);
		do {
			e = rand() % phi + 1;
		} while (gcd(e, phi) != 1);
		d = modInverse(e, phi);
	}

	// RSA加密
	int rsaEncrypt(int m, int e, int n) {
		return (int)(pow(m, e, n)) % n;
	}
	string rsaEncryptstring(string m, int e, int n) {
		string result = "";
		for (int i = 0; i < m.length(); i++) {
			result += to_string(rsaEncrypt(m[i], e, n)) + " ";
		}
		return result;
	}

	// RSA解密
	int rsaDecrypt(int c, int d, int n) {
		return (int)(pow(c, d, n)) % n;
	}
	string rsaDecryptstring(string c, int d, int n) {
		string result = "";
		for (auto x : split(c, ' '))
		{
			if (x != "")
				result += (char)(rsaDecrypt(stoi(x), d, n));
		}
		return result;
	}
	#undef int
}