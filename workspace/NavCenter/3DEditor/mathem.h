///////////**математическая библиотека**/////////////////////////////

#include <math.h>
#include "stdafx.h"



//****functions
    static bool ludecomposition(double **a,int m,int n,int** PivotsPt);
	static void ludecompositionunpacked(double** a,int m,int n, double **LPt,double **UPt, int *pivotsPt);
	static bool solvesystem(double** a,
        double* b,
        int n,
        double* x);
	static bool solvesystemlu(double** a,
        int* pivots,
        double * b,
        int n,
        double* x);

 /*   static     float Min(float a,float b);
	static     int   Min(int a,int b);
	static     float Max(float a,float b);
	static     int   Max(int a,int b);*/
//===================================================================================================================
//===================================================================================================================
/*
    LU-разложение матрицы общего вида размера M x N

    Подпрограмма вычисляет LU-разложение прямоугольной матрицы общего  вида  с
    частичным выбором ведущего элемента (с перестановками строк).

    Входные параметры:
        A       -   матрица A. Нумерация элементов: [1..M, 1..N]
        M       -   число строк в матрице A
        N       -   число столбцов в матрице A

    Выходные параметры:
        A       -   матрицы L и U в компактной форме (см. ниже).
                    Нумерация элементов: [1..M, 1..N]
        Pivots  -   матрица перестановок в компактной форме (см. ниже).
                    Нумерация элементов: [1..Min(M,N)]
                    
    Матрица A представляется, как A = P * L * U, где P - матрица перестановок,
    матрица L - нижнетреугольная (или нижнетрапецоидальная, если M>N) матрица,
    U - верхнетреугольная (или верхнетрапецоидальная, если M<N) матрица.

    Рассмотрим разложение более подробно на примере при M=4, N=3:

                       (  1          )    ( U11 U12 U13  )
    A = P1 * P2 * P3 * ( L21  1      )  * (     U22 U23  )
                       ( L31 L32  1  )    (         U33  )
                       ( L41 L42 L43 )
                       
    Здесь матрица L  имеет  размер  M  x  Min(M,N),  матрица  U  имеет  размер
    Min(M,N) x N, матрица  P(i)  получается  путем  перестановки  в  единичной
    матрице размером M x M строк с номерами I и Pivots[I]

    Результатом работы алгоритма являются массив Pivots  и  следующая матрица,
    замещающая  матрицу  A,  и  сохраняющая  в компактной форме матрицы L и U
    (пример приведен для M=4, N=3):

     ( U11 U12 U13 )
     ( L21 U22 U23 )
     ( L31 L32 U33 )
     ( L41 L42 L43 )

    Как видно, единичная диагональ матрицы L  не  сохраняется.
    Если N>M, то соответственно меняются размеры матриц и расположение
    элементов.

*/
    static bool ludecomposition(double **a,
                                    int m,
                                    int n,
									int** PivotsPt)
    {
        int i = 0;
        int j = 0;
        int jp = 0;
        double* t1 = new double[0];
        double s = 0;
        int i_ = 0;
		
        int *pivots = new int[min(m, n) + 3];
        t1 = new double[max(m, n) + 3];
        //System.Diagnostics.Debug.Assert(m >= 0 & n >= 0, "Error in LUDecomposition: incorrect function arguments");

        //
        // Quick return if possible
        //
        if (m == 0 || n == 0)
        {
            return true;
        }
        for (j = 1; j <= min(m, n); j++)
        {

            // Find pivot and test for singularity.
            jp = j;
            for (i = j + 1; i <= m; i++)
            {
                if (fabs(a[i][ j]) >fabs(a[jp][ j]))
                {
                    jp = i;
                }
            }
            pivots[j] = jp;
            if (a[jp][ j] != 0)
            {
                //Apply the interchange to rows
                if (jp != j)
                {
                    for (i_ = 1; i_ <= n; i_++)
                    {
                        t1[i_] = a[j][ i_];
                    }
                    for (i_ = 1; i_ <= n; i_++)
                    {
                        a[j][ i_] = a[jp][ i_];
                    }
                    for (i_ = 1; i_ <= n; i_++)
                    {
                        a[jp][ i_] = t1[i_];
                    }
                }
                //Compute elements J+1:M of J-th column.
                if (j < m)
                {
                    // CALL DSCAL( M-J, ONE / A( J, J ), A( J+1, J ), 1 )
                    jp = j + 1;
                    s = 1.0 / a[j][ j];
                    for (i_ = jp; i_ <= m; i_++)
                    {
                        a[i_][ j] = s * a[i_][ j];
                    }
                }
            }
            if (j < min(m, n))
            {
                //Update trailing submatrix.
                //CALL DGER( M-J, N-J, -ONE, A( J+1, J ), 1, A( J, J+1 ), LDA,A( J+1, J+1 ), LDA )
                jp = j + 1;
                for (i = j + 1; i <= m; i++)
                {
                    s = a[i][ j];
                    for (i_ = jp; i_ <= n; i_++)
                    {
                        a[i][ i_] = a[i][i_] - s * a[j][ i_];
                    }
                }
            }
        }
		*PivotsPt=pivots;
		return true;
    }
//===================================================================================================================
/*    LU-разложение матрицы общего вида размера M x N

    Использует  LUDecomposition.   По  функциональности  отличается  тем,  что
    выводит  матрицы  L  и  U не в компактной форме, а в виде отдельных матриц
    общего вида, заполненных в соответствующих местах нулевыми элементами.

*/
  static void ludecompositionunpacked(double** a,
        int m,
        int n,
        double **LPt,
        double **UPt,
        int     *pivotsPt)
    {
        int i = 0;
        int j = 0;
        int minmn = 0;

        //a = (double[,])a.Clone();
		
        if (m == 0 || n == 0)
        {
            return;
        }
        minmn = min(m, n);
        double **l = new double*[m + 3];
		for(i=0;i<m+1;i++)
			l[i]=new double[ minmn + 3];
        double **u = new double*[minmn + 3];
		for(i=0;i<minmn+1;i++)
			u[i]=new double[ n + 3];

		ludecomposition(a, m, n, &pivotsPt);
        for (i = 1; i <= m; i++)
        {
            for (j = 1; j <= minmn; j++)
            {
                if (j > i)
                {
                    l[i][ j] = 0;
                }
                if (j == i)
                {
                    l[i][j] = 1;
                }
                if (j < i)
                {
                    l[i][j] = a[i][ j];
                }
            }
        }
        for (i = 1; i <= minmn; i++)
        {
            for (j = 1; j <= n; j++)
            {
                if (j < i)
                {
                    u[i][ j] = 0;
                }
                if (j >= i)
                {
                    u[i][ j] = a[i][ j];
                }
            }
        }
    LPt=l;
	UPt=u;

}
//===================================================================================================================
/*
    Решение системы  линейных  уравнений  с  матрицей  системы,  заданной  LU-
    разложением.
    Алгоритм решает систему линейных уравнений,  матрица  которой  задана  LU-
    разложением. В случае, если  в  процессе  решения  произойдет  деление  на
    ноль, возвращается значение  False,  обозначающее,  что  система  является
    вырожденной.
    Алгоритм решает только системы уравнений с квадратной матрицей.
    Входные параметры:
        A   -   LU-разложение матрицы системы в упакованной  форме  (результат
                работы подпрограммы LUDecomposition).
        Pivots- таблица  перестановок  строк  (результат  работы  подпрограммы
                LUDecomposition).
        B   -   правая часть системы. Массив с нумерацией элементов [1..N]
        N   -   размерность системы.
 
    Выходные параметры:
       X   -   решение системы. Массив с нумерацией элементов [1..N]
     
    Результат:
        True, если система не вырождена (но, возможно, близка к вырожденной).
        False, если система вырождена. В таком случае X не содержит решение.
*/
    static bool solvesystemlu(double** a,
        int* pivots,
        double * b,
        int n,
        double* x)
    {
        bool result;
        double* y = new double[n+1];
        int i = 0;
        int j = 0;
        double v = 0;
        int ip1 = 0;
        int im1 = 0;
        int i_ = 0;

        //b = (double[])b.Clone();

        //y = new double[n + 1];
        //double *x = new double[n + 1];
        for (i = 1; i <= n; i++)
		{
           x[i]=0;
		   y[i]=0;
		}
        result = true;
        for (i = 1; i <= n; i++)
        {
            if (a[i][ i] == 0)
            {
                result = false;
				x=NULL;
                return result;
            }
        }

        //
        // pivots
        //
        for (i = 1; i <= n; i++)
        {
            if (pivots[i] != i)
            {
                v = b[i];
                b[i] = b[pivots[i]];
                b[pivots[i]] = v;
            }
        }

        //
        // Ly = b
        //
        y[1] = b[1];
        for (i = 2; i <= n; i++)
        {
            im1 = i - 1;
            v = 0.0;
            for (i_ = 1; i_ <= im1; i_++)
            {
                v += a[i][ i_] * y[i_];
            }
            y[i] = b[i] - v;
        }

        //
        // Ux = y
        //
        x[n] = y[n] / a[n][ n];
        for (i = n - 1; i >= 1; i--)
        {
            ip1 = i + 1;
            v = 0.0;
            for (i_ = ip1; i_ <= n; i_++)
            {
                v += a[i][ i_] * x[i_];
            }
            x[i] = (y[i] - v) / a[i][ i];
        }
	//	xPt=x;
        return result;
    }

//===================================================================================================================
/*
    Решение системы  линейных  уравнений
    Алгоритм решает систему линейных уравнений с использованием LU-разложения.
    Алгоритм решает только системы уравнений с квадратной матрицей.
    Входные параметры:
        A   -   Матрица системы.
                Массив с нумерацией элементов [1..N, 1..N].
        B   -   Правая часть.
                Массив с нумерацией элементов [1..N, 1..N].
        N   -   размерность системы.
    Выходные параметры:
        X   -   решение системы. Массив с нумерацией элементов [1..N]
    Результат:
        True, если система не вырождена (но, возможно, близка к вырожденной).
        False, если система вырождена. В таком случае X не содержит решение.
*/
	static bool solvesystem(double** a,
        double* b,
        int n,
        double* x)
    {
        bool result;
        int* pivots=new int[n+10];
        int i = 0;

  /*      a = (double[,])a.Clone();
        b = (double[])b.Clone();
*/
        ludecomposition(a, n, n, &pivots);
        result = solvesystemlu(a, pivots, b, n, x);
        return result;
    }
//===================================================================================================================
  /*float Min(float a,float b)
{
   if(a>b) return b;
   return a;
}
float Max(float a,float b)
{
   if(a<b) return b;
   return a;
}
int Min(int a,int b)
{
   if(a>b) return b;
   return a;
}
int Max(int a,int b)
{
   if(a<b) return b;
   return a;
}*/
//===================================================================================================================
