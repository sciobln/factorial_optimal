#include <iostream>
#include <omp.h>
#include <locale.h>
#include <string>
#include <iomanip>
using namespace std;


// расчет факториала, распределяя числа между потоками равным шагом 
double factorial_parallel_step(int NumTreads, int a)
{
    double factorial = 1;
    int i;
#pragma omp parallel shared(a) 
    {
#pragma omp for private(i) reduction(*:factorial)
        for (int j = 1; j <= NumTreads; ++j) // разделяю между потоками числа 
        {
            for (int i = j; i <= a; i += NumTreads) {
                factorial = factorial * i;
            }
        }
    }
    return factorial;
};

// расчет факториала, разбивая факториал на промежутки равной длины между потоками
double factorial_parallel_segment(int NumTreads, int a)
{
    double factorial = 1;
    int i;
#pragma omp parallel shared(a) 
    {
#pragma omp for private(i) reduction(*:factorial)
        for (int j = 1; j <= NumTreads; ++j) // разделяю между потоками числа 
        {
            for (int i = a * (j - 1) / NumTreads + 1; i < a * j / NumTreads + 1; ++i) {
                factorial = factorial * i;
            }
        }
    }
    return factorial;
};

// расчет факториала последовательно 
double factorial_consistently(int a)
{
    double factorial = 1;
    for (int i = 1; i <= a; ++i)
    {
        factorial = factorial * i;
    }
    return factorial;
};

int main()
{
    setlocale(LC_ALL, "RUS");

    // оптимальное число потоков
    cout << "Число ядер на устройстве: " << omp_get_num_procs() << endl;

    // задание факториала и числа потоков
    int a, NumTreads;

    cout << "Введите число для подсчета факториала" << endl;
    cin >> a;
    cout << "Введите число потоков" << endl;
    cin >> NumTreads;
    omp_set_num_threads(NumTreads);

    double start1, finsh1, start2, finsh2, start3, finsh3;


    start1 = omp_get_wtime();
    double f_1 = factorial_consistently(a);
    finsh1 = omp_get_wtime();

    start2 = omp_get_wtime();
    double f_2 = factorial_parallel_step(NumTreads, a);
    finsh2 = omp_get_wtime();

    start3 = omp_get_wtime();
    double f_3 = factorial_parallel_segment(NumTreads, a);
    finsh3 = omp_get_wtime();

    // проверка на верность вычислений
    //cout << fixed << f_1 << endl;
    //cout << fixed << f_2 << endl;
    //cout << fixed << f_3 << endl;

//        cout << setprecision(numeric_limits<double>::max_digits10) << fixed << "время последовательного подсчета           " << finsh1 - start1 << endl;
//        cout << setprecision(numeric_limits<double>::max_digits10) << fixed << "время параллельного подсчета c шагом       " << finsh2 - start2 << endl;
//        cout << setprecision(numeric_limits<double>::max_digits10) << fixed << "время параллельного подсчета промежуткоми  " << finsh3 - start3 << endl;

    cout << fixed << "время последовательного подсчета           " << finsh1 - start1 << endl;
    cout << fixed << "время параллельного подсчета c шагом       " << finsh2 - start2 << endl;
    cout << fixed << "время параллельного подсчета промежуткоми  " << finsh3 - start3 << endl;

}