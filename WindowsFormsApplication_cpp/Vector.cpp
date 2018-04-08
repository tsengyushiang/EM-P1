#include "DataManager.h"
#include"sstream"
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

double Dot(Vector &a, Vector &b)
{
	
		double ans=0;
		for (int i = 0; i <a.Data.size(); i++)
		{
			ans += a.Data[i] * b.Data[i];
		}
		return ans;
	
}
Vector Add(Vector &a, Vector &b)
{
	std::vector<double>data;
		double ans = 0;
		for (int i = 0; i <a.Data.size(); i++)
		{
			data.push_back(a.Data[i] + b.Data[i]);
		}
		return Vector(data);
}
Vector ScarlarMul(Vector &a, Vector &b)
{
	std::vector<double>data;
	if (a.Data.size() == 1)
	{
		for (int i = 0; i <b.Data.size(); i++)
		{
			data.push_back(a.Data[0] * b.Data[i]);
		}
	}
	else if (b.Data.size() == 1)
	{
		for (int i = 0; i < a.Data.size(); i++)
		{
			data.push_back(a.Data[i] * b.Data[0]);
		}
	}
	return Vector(data);
}
double Norm(Vector &a)
{
	double ans = 0;
	for (int i = 0; i <a.Data.size(); i++)
	{
		ans += a.Data[i] * a.Data[i];
	}
	return sqrt(ans);
}
Vector Normalize(Vector &a)
{
	double nor = Norm(a);
	std::vector<double>data;
	for (int i = 0; i <a.Data.size(); i++)
	{
		data.push_back(a.Data[i] / nor);
	}
	return Vector(data);
}
Vector CrossProduct(Vector& a, Vector &b)
{
	
		std::vector<double>data;
		data.push_back(a.Data[1] * b.Data[2]- a.Data[2] * b.Data[1]);
		data.push_back(-a.Data[0] * b.Data[2]+a.Data[2] * b.Data[0]);
		data.push_back(a.Data[0] * b.Data[1]-a.Data[1] * b.Data[0]);
		return Vector(data);
}
double Component(Vector &a, Vector &b)
{
	return Dot(a, b) / Norm(b);
}
Vector Projection(Vector &a, Vector &b)
{
	double k = Dot(a, a) / Dot(b, b);
	std::vector<double>data;
	for (int i = 0; i < a.Data.size(); i++)
	{
		data.push_back(b.Data[i] * k);
	}
	return Vector(data);
}
double TriArea(Vector &a,Vector &b)
{
	Vector k = CrossProduct(a, b);
	return 0.5*Norm(k);
}
bool Parallel(Vector &a, Vector &b)
{
	bool allzero = false;
	Vector k = CrossProduct(a, b);
	for (int i = 0; i <k.Data.size(); i++)
	{
		if (k.Data[i] == 0)
		{
			allzero = true;
		}
	}
	if (allzero)
		return 1;
	else
		return 0;
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
