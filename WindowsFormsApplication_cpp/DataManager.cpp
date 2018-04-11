#include"DataManager.h"

DataManager::DataManager()
{
	VectorVariableIndex = 0;
	MatrixVariableIndex = 0;
}

bool DataManager::LoadData()
{
	std::fstream fin;
	//�}���ɮסA�ǤJopen��ƪ��ѼƦ���ӡA���}�_���ɮצW�١A�}���ɮת��Ҧ��Ѽ�(�o��std::ios::in��Ū��(��J)���A)
	fin.open(FileName, std::ios::in);
	//Ū�����Ѧ^��false
	if (!fin)
	{
		return false;
	}
	else
	{
		//�аO��eŪ���V�qID
		int currentLoadVectorID = 0;

		//�w�qŪ���ɮצr��Ȧs�ܼ�
		int DataCount=0;
		//�q�ɮ�Ū���r��A�ѪR���V�q�`��
		fin >> DataCount;


		for (int i = 0; i < DataCount; i++)
		{
			char type = '\0';
			fin >> type;
			if (type == 'V')
			{
				//�w�q�Ȧs�V�q��Ƶ��c
				Vector tempVector;				
				//�w�q�V�q�ܼƦW�١A��VectorVariableIndex�ܼƧ@�W�٪�����
				std::string vectorVariableTemp = "$v" + std::to_string(VectorVariableIndex);
				//�s�J�V�q�ܼƦW��
				tempVector.Name = vectorVariableTemp;

				fin >> tempVector;

				//�s�J�V�q
				Vectors.push_back(tempVector);
				//���WVectorVariableIndex�A�H�T�O�ܼƦW�٤�����
				VectorVariableIndex++;
				//�M���V�q��ƼȦs
				tempVector.Data.clear();

			}
			else if (type == 'M')
			{
				//�w�q�Ȧs�V�q��Ƶ��c
				Matrix tempMatrix;
				//�w�q�V�q�ܼƦW�١A��VectorVariableIndex�ܼƧ@�W�٪�����
				std::string MatrixVariableTemp = "$m" + std::to_string(MatrixVariableIndex);
				//�s�J�V�q�ܼƦW��
				tempMatrix.Name = MatrixVariableTemp;

				//Ū�����
				fin >> tempMatrix;				

				//�s�J�x�}
				Matrixs.push_back(tempMatrix);
				//���WVectorVariableIndex�A�H�T�O�ܼƦW�٤�����
				MatrixVariableIndex++;
				//�M���V�q��ƼȦs
				tempMatrix.Data.clear();
			}					
			
		}
	
		//Ū�����\�^��false
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
		//���A��
		else if (command.substr(index, 1) == "(")
		{
			oprstack.push_back(command.substr(index, 1));
		}
		//�k�A��
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
	//�Ѿl��operators operands
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
			//��Name��������matrix
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