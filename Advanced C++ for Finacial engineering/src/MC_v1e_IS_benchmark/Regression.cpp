//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  Regression.cpp
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

#include <cmath>
#include <vector>
#include <stdexcept>

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  cholsl(), ex Numerical recipes.  Solves the linear system Ax = b
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void cholsl
(
   const std::vector<std::vector<double> > & A,  // pos def n x n matrix, assumed to be output by choldc
   const std::vector<double> & p,                // p is the leading diagonal of A
   const std::vector<double> & b,                // rhs vector
   std::vector<double> & x                       // results returned in x
)
{
	double sum = 0.0;
	long N = A.size();  // nuber of rows         Only lower diagonal of A is read
	
	for(long i = 0; i != N; ++i) 
    {
		for(long k = i - 1, sum = b[i]; k >= 0; --k)
        {
            sum -= A[i][k]*x[k];
        }
		x[i] = sum/p[i];
	}
	
	for(long i = N - 1; i >= 0; --i) 
    {
		for (long k = i + 1, sum = x[i]; k != N; ++k) 
        {
            sum -= A[k][i]*x[k];
        }
		x[i]=sum/p[i];
	}
}
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  choldc(),  ex Numerical recipes.
//         Cholesky decompositon of the positive definite matrix A as A = L.LT
//         A is symmetric n x n.  Only the upper triangle of A is required.
//         Output is into the lower triangle of A,  except for the leading diagonal
//         which is put into p.  The upper triangle of A is not modified
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

void choldc
(
   std::vector<std::vector<double> > & A,  // symmetric n x n.  Only upper triangle of A is read
   std::vector<double> & p                 // Output of leading diagonal
)                                          // rest of output is to lower diag of A
{
	double sum = 0.0;
	long N = A.size();  // number of rows in A
	
	for(long i = 0; i != N; ++i) 
    {
		for(long j = i; j != N; ++j) 
        {
			for(long k = i - 1, sum = A[i][j]; k >= 0; --k) 
            {
                sum -= A[i][k]*A[j][k];
            }
        
			if (i == j) 
            {
				if (sum <= 0.0) throw std::runtime_error("choldc failed");
				p[i] = std::sqrt(sum);
			} 
            else 
            {
                A[j][i] = sum/p[i];
            }
		}
	}
}

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
//  end of file
//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX


