#pragma once
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include <sstream>
#include <iomanip>
#include <cmath>
//�w�q�V�q��Ƶ��c

class Vector
{
public:
	bool Error;
	int size;
	std::vector<double>Data;
	std::string Name;
	Vector();
	Vector(std::string n, std::vector<double>Data);
	Vector(std::vector<double>Data);
	
	friend std::ostream& operator <<(std::ostream&, Vector&);
	friend std::istream& operator >>(std::istream&, Vector&);

	// �p�G���G��Scalar�ɴN�^�ǥu���@�Ӥ������V�q-�O�o�]�wSIZE ���MPRINT���X��

	friend Vector operator +(Vector&, Vector&); 
	friend Vector operator *(Vector&, Vector&); // Scalar Multi
	friend Vector dot(Vector&, Vector&);
	Vector Norm();
	Vector Normal();
	friend Vector Cross(Vector&, Vector&);
	friend Vector Com(Vector&, Vector&);
	friend Vector Proj(Vector&, Vector&);
	friend Vector Area(Vector&, Vector&);
	friend bool isParallel(Vector&, Vector&);
	friend bool isOrthogonal(Vector&, Vector&);
	friend Vector angle(Vector&, Vector&);
	friend Vector pN(Vector&, Vector&);
	friend bool IsLI(std::vector<Vector>&);
	friend std::vector<Vector> Ob(std::vector<Vector>&);
};
//�w�q�x�}��Ƶ��c
class Matrix
{
	// return change of det
	double sort();
	double LeadingOne();
public:
	int rowCount,colCount;
	Matrix();
	bool Error;
	std::string Name;
	std::vector<std::vector<double>> Data;
	friend Matrix operator+(const Matrix&,const Matrix&);
	friend Matrix operator-(const Matrix&,const Matrix&);
	friend Matrix operator*(const Matrix&, const Matrix&);
	friend Matrix operator*(double, const Matrix&);
	friend bool operator==(const Matrix&, const Matrix&);
	friend std::istream& operator>>(std::istream&, Matrix&);
	friend std::ostream& operator<<(std::ostream&,const Matrix&);
	int Rank();
	double det(bool&);
	Matrix eigen();
	Matrix PowerMethod();
	Matrix Transpose();
	Matrix RowReduction();
	Matrix Adjoint();
	Matrix Inverse();	
	friend Matrix SolveLinarSystem(const Matrix&,const Matrix&);
	friend Matrix LeastSquare(Matrix&,Matrix&);
};

//�w�q���޸��class
class DataManager
{
public:
	//�x�s�V�q���
	std::vector<Vector> Vectors;
	//�����V�qID�A�Ω󱱺�
	int  VectorVariableIndex;
	//�x�s�x�}���
	std::vector<Matrix> Matrixs;
	//�����V�qID�A�Ω󱱺�
	int  MatrixVariableIndex;

	//�����ɮ׸��|�W��
	std::string FileName;
	std::string ExeCommand(std::string);
	DataManager();
	//Ū�����
	bool LoadData();
	//���o�V�q���
	Vector& GetVector(std::string) ;
	//���o�x�}���
	Matrix& GetMatrix(std::string) ;
	//�]�m�ɮ׸��|�W��
	void SetFileName(std::string fileName);
};

int priority(std::string);
int operandCount(std::string);
void calcMatrix(std::vector<Matrix>& tempOperationMatrixs, int& i, std::vector<std::string>& postfixCommand, std::vector<Matrix>& tempMatrixs, Matrix& resultMatrix);
void calcVector(std::vector<Vector>& tempOperationVectors, int& i, std::vector<std::string>& postfixCommand, std::vector<Vector>& tempVectors,Vector& resultVector);
