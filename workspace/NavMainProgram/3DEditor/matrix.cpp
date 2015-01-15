#include "stdafx.h"
#include "matrix.h"
//==========================================================================================================================
CMatrix::CMatrix(int m,int n,double d,...) { 
	rows = m;  cols = n;
	double*s=&d;
	p=new double[rows*cols];
	if(p==NULL){
		throw CMatrixException('a');
	}
	for (int i=0;i<rows*cols;i++,++s)p[i]=*s;
}
//==========================================================================================================================
CMatrix::CMatrix(int m,int n,  int d,...) { 
	rows = m;  cols = n;
	int*s=&d;
	p=new double[(rows+1)*(cols+1)];
	if(p==NULL){
		throw CMatrixException('a');
	}
	for (int i=0;i<rows*cols;i++,++s)p[i]=*s;
}
//==========================================================================================================================
CMatrix::CMatrix(CMatrix const & a) {
	rows = a.rows;
	cols = a.cols;
	p=new double[rows*cols];
	if(p==NULL){
		throw CMatrixException('a');
	}
	for (int i=0;i<rows*cols;i++)p[i]=a.p[i];
}
//==========================================================================================================================
CMatrix::CMatrix(int m, int n){
	rows = m;
	cols = n;
	p=new double[rows*cols];
	if(p==NULL){
		throw CMatrixException('a');
	}
}
CMatrix::CMatrix(int i){
	rows = 1;
	cols = 1;
	p=new double[1];
	if(p==NULL){
		throw CMatrixException('a');
	}
	p[0]=i;
}
CMatrix::CMatrix(double d){
	rows = 1;
	cols = 1;
	p=new double[1];
	if(p==NULL){
		throw CMatrixException('a');
	}
	p[0]=d;
}
//==========================================================================================================================
CMatrix const& CMatrix::operator=(CMatrix const & a){
	if(rows!=a.rows||cols!=a.cols){
		throw CMatrixException('=');
	}
	//CMatrix();
	for (int i=0;i<rows*cols;i++)p[i]=a.p[i];
	return*this;
}
CMatrix::operator double()const{
	if(rows!=1||cols!=1){
		throw CMatrixException('d');
	}
	return (*this)(0,0);
}

double operator+(double d,CMatrix const&m){
	if(m.rows!=1||m.cols!=1){
		throw CMatrixException('+');
	}
	return d+m(0,0);
}
CMatrix CMatrix::operator+(CMatrix const&a)const{
	if(rows!=a.rows||cols!=a.cols){
		throw CMatrixException('+');
	}
	CMatrix c(*this);
	for(int i=0;i<rows*cols;i++)c.p[i]+=a.p[i];
	return c;
}
CMatrix CMatrix::operator-()const{
	CMatrix a(rows,cols);
	for(int i=0;i<rows*cols;i++)a.p[i]=-p[i];
	return a;
}
double operator-(double d,CMatrix const&m){
	if(m.rows!=1||m.cols!=1){
		throw CMatrixException('-');
	}
	return d-m(0,0);
}
CMatrix CMatrix::operator-(CMatrix const&a)const{
	if(rows!=a.rows||cols!=a.cols){
		throw CMatrixException('-');
	}
	CMatrix c(*this);
	for(int i=0;i<rows*cols;i++)c.p[i]-=a.p[i];
	return c;
}
CMatrix CMatrix::operator*(CMatrix const& a)const{
	if(cols!=a.rows){
		throw CMatrixException('*');
	}
	CMatrix c(rows,a.cols);
	int i=0,j,k;
    for(;i<rows;++i){
        for(j=0;j<a.cols;++j){
			c(i,j)=0;
            for(k=0;k<cols;++k)c(i,j)+=(*this)(i,k)*a(k,j);
        }
	}
	return c;
}
CMatrix CMatrix::operator*(double d)const{
	CMatrix c(*this);       
	for(int i=0;i<rows*cols;i++)c.p[i]*=d;
	return c;
}

double& CMatrix::operator()(int i, int j)const{
	if(i>=rows || j>=cols || i<0 || j<0){
		throw CMatrixException('i');
	}
	return p[i*cols+j];
}
CMatrix CMatrix::operator*()const{
    CMatrix a(cols,rows);
    int i=0,j;
    for(;i<rows;++i)
    for(j=0;j<cols;++j)a(j,i)=(*this)(i,j);
    return a;
}
//==========================================================================================================================
CMatrix CMatrix::T(CMatrix const&a)const{
/*	if(a.rows!=a.cols){
		throw CMatrixException('T');
	}
*/	CMatrix c(*this);
	for(int i=0;i<rows;i++)
		for(int j=0;j<cols;j++) c(i,j)=a(j,i);
	
	return c;
}
//==========================================================================================================================
int CMatrix::matr_inv_gause(double *a, int n)
{
  double e1, d, *b, *c, y, w, p; ;
  int i, j, k, *z;

  e1=1.e-10;
  d=1;

  z= new int[n];
  c= new double[n];
  b= new double[n];

  for(i=0; i<n; i++)
     *(z+i)=i;

  for(i=0; i<n; i++)
   {
    k=i;
    y=*(a+i*n+i);
    if(i+1 <= n )
      for(j=1+i; j<n; j++)
       {
        w=*(a+n*i+j);
        if(fabs(w)>fabs(y))
         {
           k=j;
           y=w;
         }
       }
    d=d*y;
//проверка на близость к вырождению матрицы  
   if(fabs(y)<e1)
	   return 2;
   y=1.00000/y;
   for(j=0; j<n; j++)
    {
      *(c+j)=*(a+n*j+k);
      *(a+n*j+k)=*(a+n*j+i);
      *(a+j*n+i)=-(*(c+j))*y;
      *(b+j)=*(a+i*n+j)*y;
      *(a+i*n+j)=*(b+j);
    }
   j=*(z+i);
   *(z+i)=*(z+k);
   *(z+k)=j;
   *(a+i*n+i)=y;

   for(k=0; k<n; k++)
    if(k != i)
      for(j=0; j<n; j++)
        if(j != i)
         *(a+k*n+j)=*(a+k*n+j)-(*(b+j))*(*(c+k));
   }
  for(i=0; i<n; i++)
    while(1)
    {
      k=*(z+i);
      if(k == i) break;
       for(j=0; j<n; j++)
        {
         w=*(a+i*n+j);
         *(a+i*n+j)=*(a+k*n+j);
         *(a+k*n+j)=w;
        }
      p=*(z+i);
      *(z+i)=*(z+k);
      *(z+k)=(int)p;
      d=-d;
    }

 delete[] z;
 delete[] b;
 delete[] c;

 return 0;
}
//==========================================================================================================================
CMatrix CMatrix::Inv(CMatrix &a){
	if(a.rows!=a.cols){
		throw CMatrixException('I');
	}
	if(matr_inv_gause(a.p,rows)!=0){
		throw CMatrixException('I');
	}
	return a;
}
//==========================================================================================================================
void CMatrix::SetElem(int m, int n,double value)
{
	if(m==0&&n==0)
	{
		m=m;
	}
     p[m*cols+n]=value;
}
void CMatrix::SetElem(int ind,double value)
{
   
	if(ind<cols*rows)
     p[ind]=value;
	else
      throw CMatrixException('i');

}
//==========================================================================================================================
void CMatrix::WriteToFile(CString FileName,bool ReWrite)
{
	CStdioFile File;
	CString g,buf;
	int i,j;
	if(ReWrite==false)
	{
	  File.Open(FileName,CFile::modeWrite);
	  File.SeekToEnd();
	}
	else
		File.Open(FileName,CFile::modeWrite|CFile::modeCreate);
	for (i=0;i<rows;i++)
	{
       g="";
		for (j=0;j<cols;++j)
		{
		  buf.Format("%f,    ",(*this)(i,j));
		  g+=buf;
		}	
		File.WriteString(g);
		File.WriteString("\n");
	}
	File.WriteString("\n \n");
	File.Close();
}
//================================================================================================================================
/*void ReportMatrix(double **M,int n,int m,CString FileName,bool rewrite)
{
	CStdioFile F; 
	CString str,buf;
	int i,j;
   if(rewrite==true)
	   F.Open(FileName,CFile::modeCreate|CFile::modeWrite);
   else
   {
	  F.Open(FileName,CFile::modeWrite);
      F.SeekToEnd();
   }
   for(i=0;i<n;i++)
   {
	   str="";
	   for(j=0;j<m;j++)
	   {
		   buf="";
		   buf.Format("%f",M[i][j]);
		   str+=" "+buf;
	   }
	   str+="\n";
	   F.WriteString((LPCSTR)str);
   } 
    F.WriteString("\n \n");
   F.Close();

}
double** GenMatrix(vector<TPointPriviazka> priviazka)
{
   int i;
   int n=priviazka.size();
   if(n>0)
   {
      double** M = new double*[6+1];
      for(i=0;i<6;i++)
          M[i]=new double[n+1];
     for(i=0;i<n;i++)
	 {
        M[0][i]=1;
		M[1][i]=priviazka[i].MapX;
		M[2][i]=priviazka[i].MapY;
		M[3][i]=priviazka[i].MapX*priviazka[i].MapX;
		M[4][i]=priviazka[i].MapX*priviazka[i].MapY;
		M[5][i]=priviazka[i].MapY*priviazka[i].MapY;
	 }
     return M;
   }
   return NULL;
}
//===========================================================================================================
double** TranspMatrix(double** M,int n,int m,bool del)//del=true-освобождает память от М
{
	  int i,j;
   if(n>0&&m>0)
   {
      double** Mt = new double*[m + 1];


      for(i=0;i<m;i++)
          Mt[i]=new double[n+1];
      for(i=0;i<n;i++)
        for(j=0;j<m;j++)
		  Mt[j][i]=M[i][j];

	   if(del==true)
       {
         for(i=0;i<n;i++)
            delete[] M[i];
	     delete[] M;
       }
	   return Mt;
   }

   if(del==true)
   {
      for(i=0;i<n;i++)
         delete[] M[i];
	  delete[] M;
   }
   return NULL;
}
//===========================================================================================================
double** ProductMatrix(double** M1,double** M2,int n1,int m1,int n2,int m2,int *nResult,int *mResult)
{
	int i,j,k;
	double sum;
   if(m1==n2)
   {
     double **Result=new double*[n1+1];
     for(i=0;i<n1;i++)
		 Result[i]=new double[m2+1];
     
	 for(k=0;k<m2;k++)
	 {
	     for(i=0;i<n1;i++)
		 {
		   sum=0;
	        for(j=0;j<m1;j++)
              sum+=M1[i][j]*M2[j][k];
			Result[i][k]=sum;
		 }

	 }
   *nResult=n1;
   *mResult=m2;
   return Result;
   }
   
 
   *nResult=0;
   *mResult=0;
   return NULL;
}
//==========================================================================================================================
*/