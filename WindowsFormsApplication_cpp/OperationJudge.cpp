#include"DataManager.h"

int priority(std::string opr)
{
	if ((opr == "+") || (opr == "-"))
	{
		return 1;
	}
	else if ((opr == "*") || (opr == "/"))
	{
		return 2;
	}
	else if ((opr == "Rank") || (opr == "trans") || (opr == "\\") || (opr == "det") || (opr == "Adj") || (opr == "Inverse") || (opr == "eigen") || (opr == "PM") || (opr == "LeastSquare"))
	{
		return 3;
	}
	return INT_MIN;

}

int operandCount(std::string opr)
{
	if ((opr == "+") || (opr == "-") || (opr == "*") || (opr == "/") || (opr == "LeastSquare") || (opr == "\\"))
	{
		return 2;
	}
	else if ((opr == "Cross")||(opr=="Com")|| (opr == "Proj")|| (opr == "Area")||(opr == "isParallel")|| (opr == "isOrthogonal")||(opr == "angle")|| (opr == "pN"))
	{
		return 2;

	}
	//Matrix operators
	else if ((opr == "Rank") || (opr == "trans")  || (opr == "det") || (opr == "Adj") || (opr == "Inverse") || (opr == "eigen") || (opr == "PM"))
	{
		return 1;
	}
	//Vector operators
	else if ((opr=="Norm")|| (opr == "Normal"))
	{
		return 1;	
	}
	else if((opr == "Ob")|| (opr == "IsLI"))
	{
		return INT_MAX;
	}

	return 0;
}

void calcMatrix(std::vector<Matrix>& tempOperationMatrixs,int& i, std::vector<std::string>& postfixCommand, std::vector<Matrix>& tempMatrixs, Matrix& resultMatrix)
{
	std::string opr= postfixCommand[i];
	if (tempOperationMatrixs.size() == 1)
	{
		//執行運算
		if (opr== "Rank")
		{
			postfixCommand[i] = std::to_string(tempOperationMatrixs[0].Rank());
		}
		if (opr == "det")
		{
			bool error = false;
			double result = tempOperationMatrixs[0].det(error);
			std::stringstream str;
			str.precision(5);
			str << std::fixed << std::scientific << result;

			if (error)
				postfixCommand[i] = std::string("Error");
			else
				postfixCommand[i] = str.str();
		}
		else if (opr == "trans")
		{
			resultMatrix = tempOperationMatrixs[0].Transpose();
			resultMatrix.Name = std::string("tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
		else if (opr == "Adj")
		{
			resultMatrix = tempOperationMatrixs[0].Adjoint();
			resultMatrix.Name = std::string("tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
		else if (opr == "Inverse")
		{
			resultMatrix = tempOperationMatrixs[0].Inverse();
			resultMatrix.Name = std::string("tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
		else if (opr == "eigen")
		{
			resultMatrix = tempOperationMatrixs[0].eigen();
			resultMatrix.Name = std::string("tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
		else if (opr == "PM")
		{
			resultMatrix = tempOperationMatrixs[0].PowerMethod();
			resultMatrix.Name = std::string("tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}		
	}
	else if (tempOperationMatrixs.size() == 2)
	{
		//執行運算 注意順序:matrix2-matrix1 
		if (opr == "+")
		{
			resultMatrix = tempOperationMatrixs[1] + tempOperationMatrixs[0];
			resultMatrix.Name = std::string("$tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
		else if (opr == "-")
		{
			resultMatrix = tempOperationMatrixs[1] - tempOperationMatrixs[0];
			resultMatrix.Name = std::string("$tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
		else if (opr == "*")
		{
			resultMatrix = tempOperationMatrixs[1] * tempOperationMatrixs[0];
			resultMatrix.Name = std::string("$tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
		else if (opr == "\\")
		{
			resultMatrix = SolveLinarSystem(tempOperationMatrixs[1], tempOperationMatrixs[0]);
			resultMatrix = resultMatrix.Transpose();
			resultMatrix.Name = std::string("$tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
		else if (opr == "LeastSquare")
		{
			resultMatrix = LeastSquare(tempOperationMatrixs[1], tempOperationMatrixs[0]);
			resultMatrix.Name = std::string("$tm") + std::to_string(tempMatrixs.size());
			tempMatrixs.push_back(resultMatrix);
			postfixCommand[i] = resultMatrix.Name;
		}
	
	}
	for (int count = 1; count <= tempOperationMatrixs.size(); count++)
	{
		postfixCommand.erase(postfixCommand.begin() + i - count);
	}
	i -= (tempOperationMatrixs.size() - 1);

}

void calcVector(std::vector<Vector>& tempOperationVectors, int& i, std::vector<std::string>& postfixCommand, std::vector<Vector>& tempVectors, Vector& resultVector)
{
	std::string opr = postfixCommand[i];
	if (tempOperationVectors.size() == 1)
	{
		if (opr == "Norm")
		{
			resultVector = tempOperationVectors[0].Norm();
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "Normal")
		{
			resultVector = tempOperationVectors[0].Normal();
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}

	}
	else if (tempOperationVectors.size() == 2)
	{
		if (opr == "+")
		{
			resultVector = tempOperationVectors[1] + tempOperationVectors[0];
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "*")
		{
			resultVector = tempOperationVectors[1] * tempOperationVectors[0];
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "dot")
		{
			resultVector = dot(tempOperationVectors[1], tempOperationVectors[0]);
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "Cross")
		{
			resultVector = Cross(tempOperationVectors[1], tempOperationVectors[0]);
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "Com")
		{
			resultVector = Com(tempOperationVectors[1], tempOperationVectors[0]);
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "Proj")
		{
			resultVector = Proj(tempOperationVectors[1], tempOperationVectors[0]);
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "Area")
		{
			resultVector = Area(tempOperationVectors[1], tempOperationVectors[0]);
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "isParallel")
		{
			bool Parallel = isParallel(tempOperationVectors[1], tempOperationVectors[0]);
			if (Parallel)
				postfixCommand[i] = "Yes";
			else
				postfixCommand[i] = "No";
		}
		if (opr == "isOrthogonal")
		{
			bool Orthogonal = isOrthogonal(tempOperationVectors[1], tempOperationVectors[0]);
			if (Orthogonal)
				postfixCommand[i] = "Yes";
			else
				postfixCommand[i] = "No";
		}
		if (opr == "angle")
		{
			resultVector = angle(tempOperationVectors[1], tempOperationVectors[0]);
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
		if (opr == "pN")
		{
			resultVector = pN(tempOperationVectors[1], tempOperationVectors[0]);
			resultVector.Name = std::string("$tv") + std::to_string(tempVectors.size());
			tempVectors.push_back(resultVector);
			postfixCommand[i] = resultVector.Name;
		}
	}
	else
	{
		if (opr == "IsLI")
		{
			bool IsLearnlyIndependent =IsLI(tempOperationVectors);
			if (IsLearnlyIndependent)
				postfixCommand[i] = "Yes";
			else
				postfixCommand[i] = "No";
		}
		if (opr == "Ob")
		{			
			std::reverse(tempOperationVectors.begin(), tempOperationVectors.end());
			std::vector<Vector> result;
			result = Ob(tempOperationVectors);

			std::stringstream str;

			for (Vector v : result)
			{
				str << v << "\r\n";
			}

			postfixCommand[i] = str.str();
		}
	}

	for (int count = 1; count <= tempOperationVectors.size(); count++)
	{
		postfixCommand.erase(postfixCommand.begin() + i-1);
		i--;
	}
}
