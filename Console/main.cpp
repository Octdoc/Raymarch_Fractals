#include <complex>
#include <iostream>
#include <cmath>

bool inline IsOperator(char ch)
{
	return
		ch == '+' ||
		ch == '-' ||
		ch == '*' ||
		ch == '/';
}

class Mandelbrot_PSBuilder
{
	std::string m_ps;
	size_t m_pos;

private:
	void BuildDefaultShader()
	{
		//m_ps = g_MandelbrotFractalCode1 + "ComplexMul(z, z) + c" + g_MandelbrotFractalCode2;
	}

	char NextOp()
	{
		for (size_t i = m_pos; i < m_ps.length(); i++)
			if (IsOperator(m_ps[i]))
				return m_ps[i];
		return '\0';
	}
	void BuildShader()
	{
		std::string dest;
		size_t open = 0;
		m_pos = 0;
		while (m_pos < m_ps.length())
		{
			if (m_ps[m_pos] == '-')
				m_pos++;
			switch (NextOp())
			{
			case '*':
				dest += "ComplexMul(";
				break;
			case '/':
				dest += "ComplexDiv(";
				break;
			}
			if (m_pos && m_ps[m_pos - 1] == '-')
				m_pos--;
			while (m_pos < m_ps.length())
			{
				bool opFound = true;
				switch (m_ps[m_pos])
				{
				case '+':
				case'-':
					while (open)
					{
						dest += ')';
						open--;
					}
					dest += m_ps[m_pos];
					break;
				case '*':
				case '/':
					open++;
					dest += ',';
					break;
				default:
					dest += m_ps[m_pos];
					opFound = false;
				}
				m_pos++;
				if (opFound)
					break;
			}
		}
		while (open--)
			dest += ')';
		m_ps = std::move(dest);
	}

public:
	Mandelbrot_PSBuilder(const char* equation) : m_pos(0)
	{
		for (size_t i = 0; equation[i]; i++)
			if (equation[i] != ' ')
				m_ps += equation[i];
		0 ? BuildDefaultShader() :
			BuildShader();
	}
	const char* PSCode()const { return m_ps.c_str(); }
};

int main()
{
	std::cout << Mandelbrot_PSBuilder("z*z*(z+c)").PSCode() << std::endl;
	std::cin.get();
	return 0;
}