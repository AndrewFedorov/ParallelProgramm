#include<iostream>
#include<omp.h>
#include<random>
#include<ctime>

using namespace std;

const long long int N = 1200;

template<typename T>
void giveMemory(T**& variable)
{
	variable = new T*[N];
	for (int i = 0; i < N; i++)
		variable[i] = new T[N];
}

template<typename T>
void printMatrix(T**& matrix)
{
	if (N > 10) // don't print so many
		return;

	cout << "Matrix:" << endl;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

template<typename T>
void multiplicationMatrices(T**& matrixA, T**& matrixB, T**& matrixC)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			for (int r = 0; r < N; r++)
				matrixC[i][j] += matrixA[i][r] * matrixB[r][j];
}

template<typename T>
void multiplicationMatricesOnProcessor(T**& matrixA, T**& matrixB, T**& matrixC)
{
	#pragma omp parallel
	{
		#pragma omp for
		for (int i = 0; i < N; i++)
			for (int j = 0; j < N; j++)
				for (int r = 0; r < N; r++)
					matrixC[i][j] += matrixA[i][r] * matrixB[r][j];
	}
}

//__global__ void mykernel(void) {
//}


int main()
{
	time_t current_time = time(NULL);
	srand(current_time);

	long long int **matrixA, **matrixB, **matrixC;
	giveMemory(matrixA);
	giveMemory(matrixB);
	giveMemory(matrixC);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
		{
			matrixA[i][j] = rand();
			matrixB[i][j] = rand();
			matrixC[i][j] = 0;
		}

	//mykernel << <1, 1 >> >();

	
	time_t just_time = time(NULL);
	multiplicationMatrices(matrixA, matrixB, matrixC);
	cout << "Time elapsed: " << time(NULL) - just_time << " seconds" << endl;
	time_t proc_time = time(NULL);
	multiplicationMatricesOnProcessor(matrixA, matrixB, matrixC);
	cout << "Time elapsed on processor: " << time(NULL) - proc_time << " seconds" << endl;

	printMatrix(matrixA);
	printMatrix(matrixB);
	printMatrix(matrixC);

	cout << "Time elapsed at all: " << time(NULL) - current_time << " seconds" << endl;

	system("pause");
}