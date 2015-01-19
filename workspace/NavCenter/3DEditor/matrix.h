// This is a samle program on C/C++ language.
// Demonstrate OOP
// Description of Matrix class.
// I used this class for my own graduate work, and temp paper.
#ifndef __matrix_h__
#define __matrix_h__
#include "afx.h"
#include <math.h>

//Вывод отладочных сообщений
#define debug
class CMatrixException{
	char ne;
public:
	CMatrixException(char c){ne=c;}
	CString PrintMessage(){
		switch(ne){
		case 'a':return "Невозможно выделить память";
		case '+':return "Сложение не возможно";
		case '-':return"Вычитание не возможно";
		case '*':return"Уможение не возможно";
		case '/':return"Деление невезможно";
		case 'd':return"Конвертация в тип double не возможна";
		case '=':return"Невозможно прировнять матрицу";
		case '0':return"Деление на ноль";
		case 'i':return"Неправильный индекс";
		case 'T':return"Траспонирование матрицы выполнить невозможно";
		case 'I':return"Обращение матрицы невозможно";
		default	:return"Неизвестная ошибка";
		}
	}
};
class CMatrix 
{
private:
	int rows, cols;
	double* p;
	int matr_inv_gause(double *a, int n);
public:
	
	//Конструктор
	CMatrix(int, int);
	CMatrix(int, int,double,...);
	CMatrix(int, int,   int,...);
	CMatrix(double);
			// Функция создает матрицу размеров 1 Х 1, m(0,0)=i
	CMatrix(int);
			// Функция создает матрицу размеров 1 Х 1, m(0,0)=i
	CMatrix(CMatrix const &);
	//Деструктор
	~CMatrix(){if(p!=NULL)delete[]p;}
	//
	CMatrix const& operator=(CMatrix const&);
	//Конвертация в тип double
	operator double()const;
	//Опирация сложения
	friend double operator+(double,CMatrix const&);
	double operator+(double d){return d+*this;}
	double operator+(   int i){return double(i)+*this;}
	friend double operator+( int i,CMatrix const&m){return double(i)+m;}
	CMatrix const& operator+()const{return*this;}//unary +
	CMatrix  operator+(CMatrix const&)const;//binary +
//	CMatrix& operator+=(CMatrix const&a){*this=*this+a;return*this;}

	//Опирация вычитания
	friend double operator-(double,CMatrix const&);
	double operator-(double d)const{return -(d-*this);}
	double operator-(   int i)const{return *this-double(i);}
	friend double operator-(int i,CMatrix const&m){return -(m-i);}
	CMatrix  operator-(CMatrix const&)const;//binary -
	CMatrix& operator-=(CMatrix const&a){*this=*this-a;return*this;}
	CMatrix  operator-()const;//unary -
	//Опирация умножения
	CMatrix& CMatrix::operator*=(CMatrix const&a){*this=(*this)*a;return*this;}
	CMatrix  operator*(double)const;
	friend CMatrix operator*(double d,CMatrix const&a){return a*d;}
	CMatrix& operator*=(double d){*this=*this*d;return*this;}
	CMatrix operator*(CMatrix const&)const;
	//Division operators
	CMatrix  operator/(double d)const{
		if(d==0)throw CMatrixException('d');
		return (1./d)*(*this);
	}
	CMatrix& operator/=(double d){*this=*this/d;return*this;}
	//Transposition operator
	CMatrix  operator*()const;
//	friend Matrix T(Matrix const&a){return *a;}
	//Доступ к элементам матрицы
	double& CMatrix::operator()(int i, int j)const;
	//Транспонирование матрицы
	CMatrix T(CMatrix const&a)const;
	//Обращение матрицы
	CMatrix Inv(CMatrix &a);
	//Возврат кол-ва столбцов и строк матрицы
	int GetRows()const{return rows;}
	int GetCols()const{return cols;}
	void SetElem(int m,int n,double value);
	void SetElem(int ind,double value);
	void WriteToFile(CString FileName,bool ReWrite);
};
#endif


