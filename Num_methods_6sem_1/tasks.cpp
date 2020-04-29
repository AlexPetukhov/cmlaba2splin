#include "tasks.h"
#include <cmath>

Task_test::Task_test(int _N)
{
	N = _N;
	Xgrid.resize(N+1);
	Ygrid.resize(N+1);
	a = -1;
	b = 1;
	Mu1 = Mu2 = 0;
	h = (double)(b - a) / N;
	for (int i = 0; i < N; i++)
	{
		Xgrid[i] = a+i*h;
		Ygrid[i] = Function(a + i * h);
	}
	Xgrid[N] = b;
	Ygrid[N] = Function(b);

	a_coef.resize(N + 1);
	b_coef.resize(N + 1);
	c_coef.resize(N + 1);
	d_coef.resize(N + 1);

}

double Task_test::Function(double x) const
{
	if (x >= -1 && x <= 0)
		return (x*x*x+3.0*x*x);
	else 
		return (-x*x*x + 3.0*x*x);
}

double Task_base::S_xi(int i, double x)
{
	return a_coef[i] + b_coef[i] * (x - Xgrid[i]) + (c_coef[i] / 2.0) * (x - Xgrid[i])*(x - Xgrid[i]) +
		+ (d_coef[i] / 6.0) * (x - Xgrid[i])*(x - Xgrid[i])*(x - Xgrid[i]);
}

void Task_base::Init_coef_a()
{
	for (int i = 0; i < N+1;i++)
		a_coef[i] = Ygrid[i];
}

void Task_base::Init_coef_c()
{
	std::vector<double> D_1(N+1);
	std::vector<double> D_2(N+1);
	std::vector<double> D_3(N+1);
	std::vector<double> B(N+1);

	D_1[0] = D_1[N] = 0;
	D_2[0] = D_2[N] = 1;
	D_3[0] = D_3[N] = 0;
	B[0] = Mu1;
	B[N] = Mu2;
	for (int i = 1; i < N; i++)
	{
		D_1[i] = h;
		D_2[i] = 4 * h;
		D_3[i] = h;
		B[i] = 6 * ((Ygrid[i + 1] - Ygrid[i]) / h - (Ygrid[i] - Ygrid[i - 1])/h);
	}

	c_coef = Running_method(D_1,D_2,D_3,B,N);

}

std::vector<double> Task_base::Running_method(std::vector<double> D_1, std::vector<double> D_2, std::vector<double> D_3, std::vector<double> B, int size)
{
	std::vector<double> V(size+1);
	std::vector<double> alfa(size+1);
	std::vector<double> beta(size+1);
	alfa[1] = 0;
	beta[1] = Mu1;
	V[0] = Mu1;
	V[size] = Mu2;

	for (int i = 2; i < size+1; i++)
	{
		alfa[i] = D_3[i - 1] / (-D_2[i - 1] - alfa[i - 1] * D_1[i - 1]);
		beta[i] = (-B[i-1]+beta[i-1]*D_1[i-1]) / (-D_2[i-1]-alfa[i-1]*D_1[i-1]);
	}

	for (int i = size - 1; i > 0; i--)
	{
		V[i] = alfa[i + 1] * V[i + 1] + beta[i + 1];
	}


	return V;
}

void Task_base::Init_coef_b()
{
	
	for (int i = 0; i < N; i++)
	{
		b_coef[i + 1] = (Ygrid[i + 1] - Ygrid[i]) / h + c_coef[i + 1] * h / 3.0 + c_coef[i] * h / 6.0;
	}
}

void Task_base::Init_coef_d()
{
	for (int i = 0; i < N; i++)
	{
		d_coef[i + 1] =  (c_coef[i + 1] - c_coef[i]) / h;
	}
}

Task_main_1::Task_main_1(int _N)
{
	N = _N;
	Xgrid.resize(N + 1);
	Ygrid.resize(N + 1);
	a = 1;
	b = PI;
	Mu1 = Mu2 = 0;
	h = (double)(b - a) / N;
	for (int i = 0; i < N; i++)
	{
		Xgrid[i] = a + i * h;
		Ygrid[i] = Function(a + i * h);
	}
	Xgrid[N] = b;
	Ygrid[N] = Function(b);

	a_coef.resize(N + 1);
	b_coef.resize(N + 1);
	c_coef.resize(N + 1);
	d_coef.resize(N + 1);

}

double Task_main_1::Function(double x) const
{
	return sin(x + 1) / (x + 1);
	//return sqrt(x)* sin(x);
}

double Task_test::FunctionDerivative_1(double x) const
{
	if (x >= -1 && x <= 0)
		return 3.0 * x * x + 6 * x;
	else
		return -3.0 * x * x + 6 * x;
}

double Task_main_1::FunctionDerivative_1(double x) const
{
	return ((1 + x) * cos(1 + x) - sin(1 + x)) / (1 + x) / (1 + x);
	//return 0.5*sin(x)/sqrt(x) + sqrt(x)*cos(x);
}

double Task_test::FunctionDerivative_2(double x) const
{
	if (x >= -1 && x <= 0)
		return 6.0 * x + 6;
	else
		return -6.0 * x + 6;
}

double Task_main_1::FunctionDerivative_2(double x) const
{
	return -((x * x + 2 * x - 1) * sin(1 + x) + 2 * (1 + x) * cos(1 + x)) / (1 + x) / (1 + x) / (1 + x);
	//return -0.25*sin(x)/sqrt(x*x*x)+cos(x)/sqrt(x)-sqrt(x)*sin(x);
}

double Task_base::S_xi_derivative_1(int i, double x)
{
	return (b_coef[i] + c_coef[i] * (x-Xgrid[i]) + d_coef[i]*(x-Xgrid[i])*(x - Xgrid[i]) /2.0);
}

double Task_base::S_xi_derivative_2(int i, double x)
{
	return (c_coef[i] + d_coef[i] * (x-Xgrid[i]));
}

void Task_base::Init_coefs(int choise)
{
	
	if (choise == 1)
	{
		Mu1 = -(5.0 / 4.0)*sin(1) + cos(1);
		Mu2 = -1.0 / sqrt(PI);
	}
	else if (choise == 2)
	{
		//Mu1 = -1.0/4.0 + cos(1.0)-sin(1.0)-100.0*cos(10.0);
		Mu1 = cos(1.0) - (5.0/4.0)*(sin(1.0) + 80.0*cos(10.0));
		Mu2 = -100 - 1.0 / sqrt(PI);
	}
	else if (choise == 3)
	{
		Mu1 = -5.0 * sin(1) / 4.0 + cos(1.0) - 10000.0*cos(100.0);
		Mu2 = -10000 - 1.0 / sqrt(PI);
	}

	Init_coef_a();
	Init_coef_c();
	Init_coef_b();
	Init_coef_d();
}

Task_main_2::Task_main_2(int _N)
{
	N = _N;
	Xgrid.resize(N + 1);
	Ygrid.resize(N + 1);
	a = 1;
	b = PI;
	Mu1 = Mu2 = 0;
	h = (double)(b - a) / N;
	for (int i = 0; i < N; i++)
	{
		Xgrid[i] = a + i * h;
		Ygrid[i] = Function(a + i * h);
	}
	Xgrid[N] = b;
	Ygrid[N] = Function(b);

	a_coef.resize(N + 1);
	b_coef.resize(N + 1);
	c_coef.resize(N + 1);
	d_coef.resize(N + 1);

}

double Task_main_2::Function(double x) const
{
	return sin(x + 1) / (x + 1) + cos(10 * x);
	//return sqrt(x)* sin(x) + cos(10*x);
}

double Task_main_2::FunctionDerivative_1(double x) const
{
	return ((1 + x) * cos(1 + x) - sin(1 + x)) / (1 + x) / (1 + x) - 10 * sin(10 * x);
	//return 0.5*sin(x) / sqrt(x) + sqrt(x)*cos(x) - 10*sin(10*x);
}

double Task_main_2::FunctionDerivative_2(double x) const
{
	return -((x * x + 2 * x - 1) * sin(1 + x) + 2 * (1 + x) * cos(1 + x)) / (1 + x) / (1 + x) / (1 + x) - 100 * cos(10 * x);
	//return -0.25*sin(x) / sqrt(x*x*x) + cos(x) / sqrt(x) - sqrt(x)*sin(x) - 100*cos(10*x);
}

Task_main_3::Task_main_3(int _N)
{
	N = _N;
	Xgrid.resize(N + 1);
	Ygrid.resize(N + 1);
	a = 1;
	b = PI;
	Mu1 = Mu2 = 0;
	h = (double)(b - a) / N;
	for (int i = 0; i < N; i++)
	{
		Xgrid[i] = a + i * h;
		Ygrid[i] = Function(a + i * h);
	}
	Xgrid[N] = b;
	Ygrid[N] = Function(b);

	a_coef.resize(N + 1);
	b_coef.resize(N + 1);
	c_coef.resize(N + 1);
	d_coef.resize(N + 1);

}

double Task_main_3::Function(double x) const
{
	return sin(x + 1) / (x + 1) + cos(100 * x);
	//return sqrt(x)* sin(x) + cos(100 * x);
}

double Task_main_3::FunctionDerivative_1(double x) const
{
	return ((1 + x) * cos(1 + x) - sin(1 + x)) / (1 + x) / (1 + x) - 100 * sin(100 * x);
	//return 0.5*sin(x) / sqrt(x) + sqrt(x)*cos(x) - 100 * sin(100 * x);
}

double Task_main_3::FunctionDerivative_2(double x) const
{
	return -((x * x + 2 * x - 1) * sin(1 + x) + 2 * (1 + x) * cos(1 + x)) / (1 + x) / (1 + x) / (1 + x) - 10000 * cos(100 * x);
	//return -0.25*sin(x) / sqrt(x*x*x) + cos(x) / sqrt(x) - sqrt(x)*sin(x) - 10000 * cos(100 * x);
}
