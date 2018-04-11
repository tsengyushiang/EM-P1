#include"DataManager.h"

DataManager::DataManager()
{
	VectorVariableIndex = 0;
	MatrixVariableIndex = 0;
}

bool DataManager::LoadData()
{
	std::fstream fin;
	//開啟檔案，傳入open函數的參數有兩個，欲開起的檔案名稱，開啟檔案的模式參數(這邊std::ios::in為讀取(輸入)狀態)
	fin.open(FileName, std::ios::in);
	//讀取失敗回傳false
	if (!fin)
	{
		return false;
	}
	else
	{
		//標記當前讀取向量ID
		int currentLoadVectorID = 0;

		//定義讀取檔案字串暫存變數
		int DataCount=0;
		//從檔案讀取字串，解析掉向量總數
		fin >> DataCount;


		for (int i = 0; i < DataCount; i++)
		{
			char type = '\0';
			fin >> type;
			if (type == 'V')
			{
				//定義暫存向量資料結構
				Vector tempVector;				
				//定義向量變數名稱，依VectorVariableIndex變數作名稱的控管
				std::string vectorVariableTemp = "$v" + std::to_string(VectorVariableIndex);
				//存入向量變數名稱
				tempVector.Name = vectorVariableTemp;

				fin >> tempVector;

				//存入向量
				Vectors.push_back(tempVector);
				//遞增VectorVariableIndex，以確保變數名稱不重複
				VectorVariableIndex++;
				//清除向量資料暫存
				tempVector.Data.clear();

			}
			else if (type == 'M')
			{
				//定義暫存向量資料結構
				Matrix tempMatrix;
				//定義向量變數名稱，依VectorVariableIndex變數作名稱的控管
				std::string MatrixVariableTemp = "$m" + std::to_string(MatrixVariableIndex);
				//存入向量變數名稱
				tempMatrix.Name = MatrixVariableTemp;

				//讀取資料
				fin >> tempMatrix;				

				//存入矩陣
				Matrixs.push_back(tempMatrix);
				//遞增VectorVariableIndex，以確保變數名稱不重複
				MatrixVariableIndex++;
				//清除向量資料暫存
				tempMatrix.Data.clear();
			}					
			
		}
	
		//讀取成功回傳false
		return true;
	}
}

 Vector& DataManager::GetVector(std::string name)
{
	for (Vector& v : Vectors)
	{
		if (v.Name == name)
			return v;
	}
}

 Matrix& DataManager::GetMatrix(std::string name)
{
	for (Matrix& m : Matrixs)
	{
		if (m.Name == name)
			return m;
	}

	Matrix error;
	error.Error = true;
	return error;
}

void DataManager::SetFileName(std::string fileName)
{
	FileName = fileName;
}

std::string DataManager::ExeCommand(std::string command)
{
	const std::vector<std::string> unaryoperators = {"Rank","trans","det","Adj","Inverse","eigen","PM","LeastSquare"\
		,"Norm","Normal","Cross","Com","Proj","Area","isParallel","isOrthogonal","angle","pN","IsLI","Ob"};
	const std::vector<std::string> binaryoperators = {"+","-","*","\\"};

	std::string opr;	
	std::vector<std::string> postfixCommand;
	//--------------------------------------------------------------------------------convert infix to postfix
	std::vector<std::string> oprstack;

	std::string temp;
	for (int index = 0; index < command.size(); index++)
	{	

		// Unary operator
		if ((temp == "Norm") && (command[index] = 'a'))
		{
			temp += command[index];
		}
		else if (std::find(unaryoperators.begin(), unaryoperators.end(),temp) != unaryoperators.end())
		{
			while ((!oprstack.empty())&&(priority(oprstack.back()) >= priority(temp)))
			{
				postfixCommand.push_back(oprstack.back());
				oprstack.pop_back();
			}
			oprstack.push_back(temp);
			temp.clear();
		}
		// binary operator
		else if(std::find(binaryoperators.begin(), binaryoperators.end(), command.substr(index,1)) != binaryoperators.end())
		{
			if(!temp.empty())
			postfixCommand.push_back(temp);
			while ((!oprstack.empty()) && (priority(oprstack.back()) >= priority(command.substr(index, 1))))
			{
				postfixCommand.push_back(oprstack.back());
				oprstack.erase(oprstack.end() - 1);
			}
			oprstack.push_back(command.substr(index, 1));				
			temp.clear();
		}
		//左括號
		else if (command.substr(index, 1) == "(")
		{
			oprstack.push_back(command.substr(index, 1));
		}
		//右括號
		else if(command.substr(index, 1) == ")")
		{
			if (!temp.empty())
			postfixCommand.push_back(temp);
			while ((!oprstack.empty()) && (oprstack.back()!="("))
			{
				postfixCommand.push_back(oprstack.back());
				oprstack.pop_back();
			}
			if(!oprstack.empty())
			oprstack.pop_back();
			temp.clear();
		}
		else if (command.substr(index,1)==",")
		{
			postfixCommand.push_back(temp);
			temp.clear();
		}
		else
		{
			temp += command[index];
		}
		

	}
	//剩餘的operators operands
	if (!temp.empty())
	postfixCommand.push_back(temp);
	while ((!oprstack.empty()))
	{
		postfixCommand.push_back(oprstack.back());
		oprstack.pop_back();
	}
	//--------------------------------------------------------------------------------------------  calc
	std::vector<Vector> tempVectors;// $tv0 $tv1 $tv2 $tv3 ......
	std::vector<Matrix> tempMatrixs;// $tm0 $tm1 $tm2 $tm3 ......
	for (int i = 0; i < postfixCommand.size(); )
	{
		Vector resultVector;
		Matrix resultMatrix;
		std::vector<Vector> tempOperationVectors;
		std::vector<Matrix> tempOperationMatrixs;
		// Unary operation
		if ((std::find(unaryoperators.begin(), unaryoperators.end(), postfixCommand[i]) != unaryoperators.end())||(std::find(binaryoperators.begin(), binaryoperators.end(), postfixCommand[i]) != binaryoperators.end()))
		{	
			//用Name找到對應的matrix
			for (int index = 1; (index<=i)&&(operandCount(postfixCommand[i])); index++)
			{
				if (postfixCommand[i - index].substr(1, 1) == "m")
				{
					if (GetMatrix(postfixCommand[i - index]).Error) return "Error";
					tempOperationMatrixs.push_back(GetMatrix(postfixCommand[i - index]));					
				}
				else if(postfixCommand[i - index].substr(1, 1) == "v")
				{
					if (GetVector(postfixCommand[i - index]).Error) return "Error";
					tempOperationVectors.push_back(GetVector(postfixCommand[i - index]));
				}
				else if (postfixCommand[i - index].substr(1, 2) == "tm")
				{
					for (const Matrix& m : tempMatrixs)
					{
						if (m.Name == postfixCommand[i - index])
							tempOperationMatrixs.push_back(m);
					}
				}
				else if (postfixCommand[i - index].substr(1, 2) == "tv")
				{
					for (const Vector& v : tempVectors)
					{
						if (v.Name == postfixCommand[i - index])
							tempOperationVectors.push_back(v);
					}
				}

			}			
			if (tempOperationMatrixs.size() == operandCount(postfixCommand[i]))
				calcMatrix(tempOperationMatrixs, i, postfixCommand, tempMatrixs, resultMatrix);
			else if (tempOperationVectors.size() == operandCount(postfixCommand[i]))
				calcVector(tempOperationVectors, i, postfixCommand, tempVectors, resultVector);
			else if(tempOperationVectors.size() != 0)//Ob
				calcVector(tempOperationVectors, i, postfixCommand, tempVectors, resultVector);
			else
				return "Error";
		}
		//operands
		else
		{
			i++;
		}		

	}		
	//------------------------------------------------------------------------------------------------convert result to string
	std::stringstream str;	

	//result=Matrix
	for (int i = 0; i < tempMatrixs.size(); i++)
	{
		if (tempMatrixs[i].Name == postfixCommand[0])
		{			
			str << tempMatrixs[i];
			return str.str();
		}
	}

	//result=Vector
	for (int i = 0; i < tempVectors.size(); i++)
	{
		if (tempVectors[i].Name == postfixCommand[0])
		{
			str << tempVectors[i];
			return str.str();
		}
	}


	if (postfixCommand[0].substr(0, 1) == "$m")
	{		
		str << GetMatrix(postfixCommand[0]);
		return str.str();
	}
	else if (postfixCommand[0].substr(0, 1) == "$V")
	{
		str << GetVector(postfixCommand[0]);
		return str.str();
	}
	else
	{
		return postfixCommand[0];
	};


	//--------------------------------------------------------------------------------------------------
}