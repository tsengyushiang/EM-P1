#include"DataManager.h"
#define EQUALZERO(x) (((x<0.00001)&&(x>-0.00001))?true:false)
double Matrix::sort()
{
	double det = 1;
	// sort by zero count
	for (int row = 0; row < rowCount; row++)
	{
		int ZeroCount = 0;
		while ((ZeroCount < colCount) && EQUALZERO(Data[row][ZeroCount]))
				ZeroCount++;
		

		for (int i = row + 1; i< rowCount; i++)
		{
			int tempZeroCount = 0;
			while ((tempZeroCount<colCount) && EQUALZERO(Data[i][tempZeroCount]))
				tempZeroCount++;

			if (ZeroCount > tempZeroCount)
			{
				std::vector<double> temp;
				temp = Data[row];
				Data[row] = Data[i];
				Data[i] = temp;

				det *= -1;

				ZeroCount = tempZeroCount;
			}
		}
	}

	return det;

}

double Matrix::LeadingOne()
{
	double det = 1;
	for (int row = 0; row < rowCount; row++)
	{
		int col = 0;
		while ((col < colCount) && EQUALZERO(Data[row][col]))
			col++;

		for (int k = col + 1; k < colCount; k++)
		{
			Data[row][k] /= Data[row][col];						

		}
		if (col < colCount)
		{
			det *= Data[row][col];
			Data[row][col] = 1;
		}
		
	}

	return det;
	
}

Matrix::Matrix()
{
	Error = 0;
	rowCount = 0;
	colCount = 0;
	Name = "";
	Data.clear();
}

Matrix operator-(const Matrix& a, const Matrix& b)
{
	Matrix result;

	//維度判斷 不同則回傳Error
	if ((a.colCount != b.colCount) || (a.Error) || (b.Error) || (a.rowCount != b.rowCount))
	{
		result.Error = 1;
		return result;
	}

	result.colCount = a.colCount;
	result.rowCount = a.rowCount;

	for (int i = 0; i < result.rowCount; i++)
	{
		std::vector<double> tempData;
		for (int j = 0; j < result.colCount; j++)
		{
			double value = a.Data[i][j] - b.Data[i][j];
			tempData.push_back(value);

		}
		result.Data.push_back(tempData);
		tempData.clear();
	}

	return result;


}
Matrix operator+(const Matrix& a,const Matrix& b)
{
	Matrix result;

   //維度判斷 不同則回傳Error
	if ((a.colCount != b.colCount)||(a.Error)||(b.Error)||(a.rowCount!=b.rowCount))
	{
		result.Error = 1;
		return result;
	}

	result.colCount = a.colCount;
	result.rowCount = a.rowCount;

	for (int i = 0; i < result.rowCount; i++)
	{
		std::vector<double> tempData;
		for (int j = 0; j < result.colCount; j++)
		{
			double value = a.Data[i][j] + b.Data[i][j];
			tempData.push_back(value);

		}
		result.Data.push_back(tempData);
		tempData.clear();
	}

	return result;
}

bool operator==(const Matrix& a, const Matrix& b)
{
	if (a.rowCount != b.rowCount) return false;
	if (a.colCount != b.colCount) return false;

	for (int row =0; row < a.rowCount; row++)
	{
		for (int col = 0; col < a.colCount; col++)
		{
			if (abs(a.Data[row][col] -b.Data[row][col]) >0.00001)return false;
		}		
	}	

	return true;
}

std::istream& operator>>(std::istream& input, Matrix& Data)
{
	//讀取矩陣資料
   input >> Data.rowCount >> Data.colCount;

	for (int row = 0; row < Data.rowCount; row++)
	{
		std::vector<double> tempDatas;
		for (int col = 0; col < Data.colCount; col++)
		{
			double value;
			input >> value;
			tempDatas.push_back(value);
		}
		Data.Data.push_back(tempDatas);
		tempDatas.clear();
	}

	return input;

}


std::ostream& operator<<(std::ostream& output,const Matrix& Data)
{
	if (Data.Error)
	{
		output << "Error\r\n";
		return output;
	}
	output << std::fixed << std::showpoint;
	output << std::setprecision(5);
	for (int row = 0; row < Data.rowCount; row++)
	{
		for (int col = 0; col < Data.colCount; col++)
		{
			output << Data.Data[row][col];
			if (col != Data.colCount - 1)
					output << " ";
			
				
		}
		output <<"\r\n";
	}

	return output;
}


int Matrix::Rank()
{
	Matrix temp=RowReduction();	
	int rankCount = temp.rowCount;
	for (int row = 0; row < temp.rowCount; row++)
	{
		int col = 0;
		while ((col < colCount) && EQUALZERO(temp.Data[row][col]))col++;

		if (col==colCount)rankCount--;
	}

	return rankCount;
}

Matrix Matrix::RowReduction()
{
	Matrix result=*this;	

	//Gaussian Elimination
	for (int row = 0,col=0;(row < rowCount)&&(col<colCount);row++,col++)
	{		
		result.sort();
		result.LeadingOne();
		
		
		if (result.Data[row][col]==0)
		{
			col++;
			if (col < colCount)break;
		}
		for (int i = row + 1; (i<rowCount)&&(result.Data[i][col]==1); i++)
		{
			for (int j =col; j < colCount; j++)
			{
				result.Data[i][j] -= result.Data[row][j];
			}
		}
	}

	//REDUCED ROW ECHELON FORM
	int lastElement = result.rowCount > result.colCount ? result.colCount-1 : result.rowCount-1;

	for (int row = lastElement; row >= 0; row--)
	{
		if (result.Data[row][row] != 1)continue;
		for (int iterativeRow = row - 1; iterativeRow >= 0; iterativeRow--)
		{
			if (EQUALZERO(result.Data[iterativeRow][row]))continue;

			for (int col = row + 1; col < colCount; col++)
				result.Data[iterativeRow][col] -= result.Data[row][col] * result.Data[iterativeRow][row];
						
			result.Data[iterativeRow][row] = 0;
		}
	}	

	return result;

}

Matrix Matrix::Transpose()
{
	Matrix result;
	result.rowCount = colCount;
	result.colCount = rowCount;

	for (int row = 0; row < colCount; row++)
	{
		std::vector<double> temp;
		for (int col = 0; col <rowCount; col++)
		{
			temp.push_back(Data[col][row]);
		}
		result.Data.push_back(temp);
		temp.clear();
	}

	return result;
}

Matrix operator*(const Matrix& a, const Matrix& b)
{
	Matrix result;
	result.rowCount = a.rowCount;
	result.colCount = b.colCount;

	if (a.colCount != b.rowCount)
	{
		result.Error = true;
		return result;
	}

	for (int row = 0; row < result.rowCount; row++)
	{
		std::vector<double> temp;
		for (int col = 0; col < result.colCount; col++)
		{
			double accumulator=0;
			for (int index = 0; index < a.colCount; index++)
			{				
				accumulator += a.Data[row][index] * b.Data[index][col];				
			}			
			temp.push_back(accumulator);
			
		}				
		result.Data.push_back(temp);
		temp.clear();
	}

	return result;
}

Matrix operator*(double a, const Matrix& b)
{
	Matrix result=b;
	
	for (int row = 0; row < result.rowCount; row++)
	{
		for (int col = 0; col < result.colCount; col++)
		{		
			result.Data[row][col] *= a;
		}
	}

	return result;
}

Matrix SolveLinarSystem(const Matrix& a,const Matrix& b)
{
	Matrix AugmentedMatrix;
	Matrix result;

	AugmentedMatrix.rowCount = a.rowCount;
	AugmentedMatrix.colCount = a.colCount + 1;

	if ((a.colCount != b.rowCount) || (b.colCount != 1))
	{
		result.Error = true;
		return result;
	}

	for (int row = 0; row < AugmentedMatrix.rowCount; row++)
	{
		std::vector<double> temp;
		temp = a.Data[row];
		temp.push_back(b.Data[row][0]);
		AugmentedMatrix.Data.push_back(temp);
		temp.clear();
	}
	AugmentedMatrix=AugmentedMatrix.RowReduction();

	// check if rank< n
	for (int index = 0; index < a.rowCount; index++)
	{
		if (AugmentedMatrix.Data[index][index] != 1)
		{
			result.Error = true;
			return result;
		}
	}

	// 把x搬到result矩陣
	result.rowCount = AugmentedMatrix.rowCount;
	result.colCount = 1;

	for (int row = 0; row < result.rowCount; row++)
	{
		std::vector<double> temp;
		temp.push_back( AugmentedMatrix.Data[row][a.colCount]);
		result.Data.push_back(temp);
		temp.clear();
	}

	return result;
}

double Matrix::det(bool& error)
{
	if (rowCount != colCount)
	{
		error = true;
		return 0;
	}

	Matrix result = *this;
	double det = 1;
	//Gaussian Elimination
	for (int row = 0, col = 0; (row < rowCount) && (col<colCount); row++, col++)
	{
		det*=result.sort();
		det*=result.LeadingOne();

		if (result.Data[row][col] == 0)
		{
			col++;
			if (col < colCount)break;
		}
		for (int i = row + 1; (i<rowCount) && (result.Data[i][col] == 1); i++)
		{
			for (int j = col; j < colCount; j++)
			{
				result.Data[i][j] -= result.Data[row][j];
			}
		}
	}	

	for (int index = 0; index < rowCount; index++)
	{
		det *= result.Data[index][index];
	}

	return det;

}

Matrix Matrix::Adjoint()
{
	Matrix result;
	bool error = 0;
	result.rowCount = rowCount;
	result.colCount = colCount;

	for (int col = 0; col < colCount; col++)
	{
		std::vector<double> tempArray;
		for (int row = 0; row < rowCount; row++)
		{
			Matrix temp;
			temp.rowCount = rowCount - 1;
			temp.colCount = colCount - 1;
			//-------------------------------------填要計算det的矩陣
			for (int i = 0; i < rowCount; i++)
			{
				std::vector<double> tempArray2;
				for (int j = 0; j < colCount; j++)
				{
					//矩陣為row列col行不填
					if ((i != row) && (j != col))
					{
						tempArray2.push_back(Data[i][j]);
					}
				}
				if (!tempArray2.empty())
				{
					temp.Data.push_back(tempArray2);
					tempArray2.clear();	
				}			
			}
			//算出det後填入結果矩陣
			tempArray.push_back(temp.det(error)*std::pow(-1,row+col));
		}
		result.Data.push_back(tempArray);
		tempArray.clear();
	}

	return result;
}

Matrix Matrix::Inverse()
{
	double zero = 0;

	Matrix result;
	result.rowCount = rowCount;
	result.colCount = colCount;

	if (rowCount != colCount)
	{
		result.Error = true;
		return result;
	}
	if (Rank() != rowCount)
	{
		result = *this;
		for (int row = 0; row < rowCount; row++)
		{
			for (int col = 0; col < colCount; col++)
			{
				result.Data[row][col] = double(1.0/zero);
			}
		}
		return result;
	}

	Matrix AugmentedMatrix;

	AugmentedMatrix.rowCount = rowCount;
	AugmentedMatrix.colCount = colCount*2;

	for (int row = 0; row < rowCount; row++)
	{	
		std::vector<double> tempArray;
		int col = 0;
		for (; col < colCount; col++)
		{
			tempArray.push_back(Data[row][col]);

		}

		for (; col < AugmentedMatrix.colCount; col++)
		{
			tempArray.push_back(col-colCount==row?1:0);
		}
		AugmentedMatrix.Data.push_back(tempArray);
		tempArray.clear();
	}
	AugmentedMatrix = AugmentedMatrix.RowReduction();
	
	for (int row = 0; row < rowCount; row++)
	{
		std::vector<double> tempArray;	
		for (int col = 0; col < colCount; col++)
		{
			tempArray.push_back(AugmentedMatrix.Data[row][col+colCount]);
		}		
		result.Data.push_back(tempArray);
		tempArray.clear();
	}

	return result;
}


//解一元三次式
double f(double x,double a, double b, double c, double d)
{
	double temp;
	temp = ((a*x + b)*x + c)*x + d;
	return temp;
}
std::vector<double> solveEquation(double a, double b, double c, double d)
{			
		std::vector<double> x{ 0,0,0 };
		int t = -1;
		int i = 0;
		double u=0, v = 0;
		
		for (i = -10000; i <= 10000; i++)
		{
			u = double(i);
			v = u + 0.99999;
			if (fabs(f(u,a,b,c,d))<0.00001 || f(u,a,b,c,d)*f(v,a,b,c,d) <= 0)
			{
				t++;
				if (fabs(f(u,a,b,c,d))<0.00001)
					x[t] = u;
				else
				{
					while ((u + 0.001<v) && fabs(f((u + v) / 2,a,b,c,d)) >= 0.00001)
					{
						if (f(u,a,b,c,d)*f((u + v) / 2,a,b,c,d)<0)
							v = (u + v) / 2;
						else
							u = (u + v) / 2;
					}
					x[t] = (u + v) / 2;
				}
			}
		}

		return x;
}
Matrix Matrix::eigen()
{
	Matrix result;

	if (rowCount != colCount)
	{
		result.Error = true;
		return result;
	}

	/*
	
	a-x b 
	c  d-x
	 (a-x)(d-x)-b*c =(a*d-b*c)+(-a-d)x+(1)x^2
	 
	 a=1
	 b=-a-d
	 c=a*d-b*c
	
	*/

	if (rowCount == 2)
	{
		result.rowCount = 4;
		result.colCount = 2;

		//-----------------------------------------------------------------------count eigen value
		double a = 1;
		double b = -(Data[0][0] + Data[1][1]);
		double c = Data[0][0] * Data[1][1] - Data[0][1] * Data[1][0];
		double delta = std::pow(b, 2) - 4 * a*c;

		if (delta < 0)
		{
			result.Error = true;
			return result;
		}

		double eigenValue1 = (-b + std::pow(delta,0.5)) / (2 * a);
		double eigenValue2 = (-b - std::pow(delta, 0.5)) / (2 * a);
		//-----------------------------------------------------------------------sovle ax=0
		Matrix temp1 = *this;
		Matrix temp2 = *this;

		//create 0
		Matrix allZeroMatrix;
		allZeroMatrix.rowCount = rowCount;
		allZeroMatrix.colCount = 1;
		
		allZeroMatrix.Data.push_back(std::vector<double>{0});
		allZeroMatrix.Data.push_back(std::vector<double>{0});
		
		temp1.Data[0][0] -= eigenValue1;
		temp1.Data[1][1] -= eigenValue1;
		temp1=temp1.RowReduction();


		temp2.Data[0][0] -= eigenValue2;
		temp2.Data[1][1] -= eigenValue2;
		temp2 = temp2.RowReduction();
		//----------------------------------------------------------------------
		double length = 0;
		for (int i = 0; i < 2; i++)
		{
			if (!i)length = 0;
			for (int row = 0; row < result.colCount; row++)
			{
				if (!i)length += powl(temp1.Data[0][row], 2);
				else temp1.Data[0][row] /= powl(length, 1.0 / 2);
			}
		}

		for (int i = 0; i < 2; i++)
		{	
			if (!i)length = 0;
			for (int row = 0; row < result.colCount; row++)
			{				
				if (!i)length += powl(temp2.Data[0][row],2);
				else temp2.Data[0][row] /= powl(length,1.0/2);

			}
		}
		//-----------------------------------------------------------------------------
		result.Data.push_back(std::vector<double>{temp1.Data[0][1],temp2.Data[0][1]});
		result.Data.push_back(std::vector<double>{-temp1.Data[0][0],-temp2.Data[0][0]});
		result.Data.push_back(std::vector<double>{eigenValue1, 0});
		result.Data.push_back(std::vector<double>{0, eigenValue2});		
		
		return result;
	}
	else if (rowCount == 3)
	{
		/*
		1-x 2 3
		4 5-x 6
		7 8  9-x
	
		(1-x)(5-x)(9-x)+2*6*7+3*4*8-((1-x)*6*8+(5-x)*3*7+(9-x)*2*4)
		=(-x)^3+(1+5+9)x^2-(9*5+9*1+5*1)x+1*5*9-(1*6*8-6*8x+5*3*7-3*7x+9*2*4-2*4x)
		=(-x)^3+(1+5+9)x^2-(9*5+9*1+5*1+6*8+3*7+2*4)x+1*5*9++2*6*7+3*4*8-1*6*8-5*3*7-9*2*4
		*/
		result.rowCount = 6;
		result.colCount = 3;
		//-----------------------------------------------------------------------count eigen value
		double a = -1;
		double b =Data[0][0]+Data[1][1]+Data[2][2];
		double c = -(Data[2][2]*Data[1][1]+Data[2][2]*Data[0][0]+ Data[0][0] * Data[1][1] - Data[1][2] * Data[2][1] \
					- Data[0][2] * Data[2][0] - Data[0][1] * Data[1][0]);
		double d =Data[0][0]*Data[1][1]*Data[2][2]+ Data[0][1] * Data[1][2] * Data[2][0] + Data[0][2] * Data[1][0] * Data[2][1] \
			- Data[0][0] * Data[1][2] * Data[2][1]-	Data[1][1] * Data[0][2] * Data[2][0]- Data[2][2] * Data[0][1] * Data[1][0];

		std::vector<double> eigenValues;

		eigenValues=solveEquation(a, b, c, d);
		//-----------------------------------------------------------------------sovle ax=0
		std::vector<Matrix> temps{*this,*this, *this};
		
		//create 0
		Matrix allZeroMatrix;
		allZeroMatrix.rowCount = rowCount;
		allZeroMatrix.colCount = 1;

		allZeroMatrix.Data.push_back(std::vector<double>{0});
		allZeroMatrix.Data.push_back(std::vector<double>{0});
		allZeroMatrix.Data.push_back(std::vector<double>{0});

		for (int index = 0; index < 3; index++)
		{
			temps[index].Data[0][0] -= eigenValues[index];
			temps[index].Data[1][1] -= eigenValues[index];
			temps[index].Data[2][2] -= eigenValues[index];
			temps[index] = temps[index].RowReduction();
		}			

		for (int index = 0; index < 3; index++)
		{

			std::vector<double> temp;
			for (int i = 0; i < 3; i++)
			{
				if ((index == temps[i].colCount - 1)&&temps[i].Data[index][index]==1)
				{
					temp.push_back(0.0);
				}
				else if (EQUALZERO(temps[i].Data[index][index]))
				{
					temp.push_back(1.0);
				}
				else
				{
					temp.push_back(-temps[i].Data[index][2]);
				}			
			}	
			result.Data.push_back(temp);
			temp.clear();
		}



		for (int i = 0; i < 3; i++)
		{
			double length = 0;
			for (int j = 0; j < 2; j++)
			{
				if (!j)length = 0;
				for (int index = 0; index < 3; index++)
				{
					if (!j)length += powl(result.Data[index][i], 2);
					else result.Data[index][i] /= powl(length, 1.0 / 2);
				}
			}
		}

		//----------------------------------------------------------------------		
		result.Data.push_back(std::vector<double>{eigenValues[0],0,0});
		result.Data.push_back(std::vector<double>{0, eigenValues[1],0});
		result.Data.push_back(std::vector<double>{0, 0, eigenValues[2]});

		return result;
	}

	return result;
}

Matrix Matrix::PowerMethod()
{
	Matrix result;

	if (rowCount != colCount)
	{
		result.Error = true;
		return result;
	}

	result.rowCount = rowCount*2;
	result.colCount = colCount;
	for (int row = 0; row < result.rowCount; row++)
	{
		result.Data.push_back(std::vector<double>{});
	}

	Matrix zeroMatrix;
	Matrix initialGuess;
	Matrix processing;
	zeroMatrix.rowCount = rowCount;
	zeroMatrix.colCount = 1;
	processing.rowCount = rowCount;
	processing.colCount = 1;
	initialGuess.rowCount = rowCount;
	initialGuess.colCount = 1;

	for (int row = 0; row < rowCount; row++)
	{
		initialGuess.Data.push_back(std::vector<double>{1});
		zeroMatrix.Data.push_back(std::vector<double>{0});
		processing.Data.push_back(std::vector<double>{1});
	}

	Matrix B = *this;

	for (int i = 0; i < rowCount; i++)
	{
		do
		{
			initialGuess = processing;
			processing = B * initialGuess;

			double length = 0;
			double LeadingPositive = (processing.Data[0][0] < 0 ? -1 : 1);
			for (int i = 0; i < 2; i++)
			{
				if (!i)length = 0;
				for (int row = 0; row < rowCount; row++)
				{
					if (!i)length += powl(processing.Data[row][0], 2);
					else
					{
						processing.Data[row][0] /= powl(length, 1.0 / 2);
						processing.Data[row][0] *= LeadingPositive;
					}
				}
			}

		} while (!((processing - initialGuess) == zeroMatrix));

		double eigenvalue = ((B*processing).Transpose()*processing).Data[0][0] / (processing.Transpose()*processing).Data[0][0];
	
		B = B - (eigenvalue*(processing*processing.Transpose()));
		
		
		for (int row = 0; row < B.rowCount; row++)
		{
			result.Data[row].push_back(processing.Data[row][0]);
		}
		for (int row = rowCount; row < result.rowCount; row++)
		{
			if (row - rowCount == i)
				result.Data[row].push_back(eigenvalue);
			else
				result.Data[row].push_back(0.0);
		}

	}
	
	return result;
}

Matrix LeastSquare(Matrix&  a, Matrix& b)
{
	Matrix x = a.Transpose() * a;
	
	Matrix result=x.Inverse()*a.Transpose()*b;

	result.colCount = 1;
	result.rowCount = a.colCount;

	return result;
}