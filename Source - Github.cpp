#include <iostream>
#include <fstream>
#include <math.h>
#include <cstdlib>

using namespace std;

long str2long(char*);
void q_print(double*);

long NUM_TERMS;

int main(int argc, char** argv)
{

	

	if (argc != 3 && argc != 4)
	{
		cout << "Incorrect calling procedure.  Try " << argv[0] << " num_terms d (filename)." << endl;
		return 0;
	}

	ofstream output;
	if (argc == 4)
	{
		output.open(argv[3]);
		output << "n, Q, q, D\n";
	}

	cout << "Computing q_d(n): " << flush;

	NUM_TERMS = str2long(argv[1]) + 1;

	int d = str2long(argv[2]);

	long double* q = new long double[NUM_TERMS];
	long double** p_nk = new long double* [NUM_TERMS];



	long max_k = long(7 / 2 + sqrt(double(2 * NUM_TERMS) / (d)+1 / 4));
	q[0] = 1;
	q[1] = 0;

	for (long i = 0; i < NUM_TERMS; ++i)
	{
		p_nk[i] = new long double[2];

	}

	for (long i = 2; i < NUM_TERMS; ++i)
	{

		q[i] = 1;
	}

	for (long i = 0; i < NUM_TERMS; ++i)
		for (int j = 0; j < 2; ++j)
		{
			p_nk[i][j] = 0;
			p_nk[i][j] = 0;
		}

	for (long i = 0; i < NUM_TERMS; ++i)
		p_nk[i][0] = 1;

	int col = 0;

	for (long i = 2; i < max_k; ++i)
	{
		col = 1 - col;
		for (long j = 0; j < NUM_TERMS; ++j)
		{
			if (i <= j)
				p_nk[j][col] = p_nk[j - 1][1 - col] + p_nk[j - i][col];
			else
				p_nk[j][col] = 0;
		}
		long kk = (i * (i - 1) / 2) * d + i;
		for (long j = kk; j < NUM_TERMS; ++j)
			q[j] += p_nk[j - kk][col];

	}

	free(p_nk);

	cout << "Done!" << endl << "Computing Q_d(n): " << flush;



	long max_kQ = 2 * NUM_TERMS / (d + 3) + 2;

	long double** Q_k = new long double* [NUM_TERMS];
	for (long i = 0; i < NUM_TERMS; ++i)
		Q_k[i] = new long double[2];

	for (long i = 0; i < NUM_TERMS; ++i)
		for (long j = 0; j < 2; ++j)
			Q_k[i][j] = 0;

	for (long i = 0; i < NUM_TERMS; ++i)
	{
		if (i % 2 == 0)
		{
			Q_k[i][0] = 1;
		}
		if (i % 2 == 1)
		{
			Q_k[i][0] = 0;
		}
	}
	col = 0;

	long double* Q = new long double[NUM_TERMS];

	long double round_err = 0;
	long double* last_used = new long double[NUM_TERMS];

	for (long j = 0; j < NUM_TERMS; ++j) last_used[j] = 0;

	long kk = 0;
	long kk2 = 0;

	for (long k = 1; k < max_kQ; ++k)
	{
		col = 1 - col;
		kk2 = (k / 2) * (d + 3) + (k % 2) * (d + 1) + 2*(1 - (k % 2));
		for (long j = kk; j < kk2 && j < NUM_TERMS; ++j)
		{
			Q_k[j][col] = Q_k[j][1 - col];
			Q[j] = Q_k[j][col];
			
			if (j % 10000 == 0 && j != 0)
				cout << j << "..." << flush;
		}
		kk = kk2;
		
		for (long i = kk; i < NUM_TERMS; ++i)
		{

			Q_k[i][col] = Q_k[i][1 - col] + Q_k[i - kk][col];
			
			round_err = Q_k[i - kk][col] - (Q_k[i][col] - Q_k[i][1 - col]);
			if (round_err != 0)
			{
				if (round_err < 0) round_err *= -1;
				//				if (round_err/Q_k[i][col]<1e-16) round_err=Q_k[i][col]*1e-16;
				if (round_err < last_used[i]) round_err = last_used[i];
				while (Q_k[i][col] + round_err == Q_k[i][col]) round_err *= 2;
				Q_k[i][col] += round_err;
				last_used[i] = round_err;
			}
		}
	}

	if (argc == 4)
		for (long j = 1; j < NUM_TERMS; ++j)
			output << j << "," << Q[j] << "," << q[j] << "," << q[j] - Q[j] << endl;

	cout << "Done!" << endl;



	if (argc == 4)
		output.close();

	free(Q);
	free(q);

	return 0;
}

long str2long(char* str)
{
	long val = 0;
	int sign = 1;
	int i = 0;
	if (str[i] == '-')
	{
		sign = -1;
		++i;
	}
	while (str[i])
		val = 10 * val + str[i++] - 48;
	return val * sign;
}

void q_print(double* q1)
{
	if (q1[0] != 0)
		cout << q1[0] << " + ";
	for (int i = 1; i < NUM_TERMS; ++i)
		if (q1[i] != 0)
			cout << q1[i] << " q^" << i << " + ";
	cout << "O(q^" << NUM_TERMS << ")" << endl;
}
