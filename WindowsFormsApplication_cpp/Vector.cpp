#include "DataManager.h"

Vector::Vector()
{
	Error = false;
	size = 0;
	Name="";
	Data.clear();
}
Vector::Vector(std::string n, std::vector<double>D)
{
	Name = n;
	for (int i = 0; i < D.size(); i++)
	{
		Data.push_back(D[i]);
	}
}
Vector::Vector(std::vector<double>D)
{
	Name = "V";
	for (int i = 0; i < D.size(); i++)
	{
		Data.push_back(D[i]);
	}
}

Vector operator +(Vector& a, Vector& b)
{
	return Vector();
}
Vector operator *(Vector& a, Vector& b)
{
	return Vector();
}
Vector dot(Vector& a, Vector& b)
{
	return Vector();
}
Vector Vector::Norm()
{
	return Vector();
}
Vector Vector::Normal()
{
	return Vector();
}
Vector Cross(Vector& a, Vector& b)
{
	return Vector();
}
Vector Com(Vector& a, Vector& b)
{
	return Vector();
}
Vector Proj(Vector& a, Vector& b)
{
	return Vector();
}
Vector Area(Vector& a, Vector& b)
{
	return Vector();
}
bool isParallel(Vector& a, Vector& b)
{
	return true;
}
bool isOrthogonal(Vector& a, Vector& b)
{
	return true;
}
Vector angle(Vector& a, Vector& b)
{
	return Vector();
}
Vector pN(Vector& a, Vector& b)
{
	return Vector();
}
bool IsLI(std::vector<Vector>& vecotrs)
{
	return true;
}
std::vector<Vector> Ob(std::vector<Vector>& vecotrs)
{
	return std::vector<Vector>();
}
std::ostream& operator<<(std::ostream& output, Vector &a)
{
	if (a.Error)
	{
		output << "Error";
		return output;
	}
		for (int i = 0; i < a.Data.size(); i++)
		{
			if (i == a.Data.size() - 1)
				output << a.Data[i]<<"\r\n";
			else
			output << a.Data[i] <<" ";
	    }
	return output;
}
std::istream&operator >>(std::istream& input, Vector &a)
{
	input >> a.size;

	for (int count = 0; count < a.size; count++)
	{
		double value;
		input >> value;
		a.Data.push_back(value);
	}

	return input;
}
