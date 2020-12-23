#include "pch.h"
#include "mandelbrotfractal.h"

namespace fracapp
{
	const std::string g_MandelbrotFractalCode1 = R"(
cbuffer Data
{	
	float2 resolution;
	float2 offset;
	float2 center;
	float zoom;
	float iterationCount;
};
struct PixelInputType
{
	float4 windowPosition : SV_POSITION;
};

inline float angle(float2 z)
{
	if (z.x > 0.0f)
		return atan(z.y / z.x);
	float pi = 3.141592654f;
	if (z.y > 0.0f)
		return pi * 0.5f - atan(z.x / z.y);
	if (z.y < 0.0f)
		return -pi * 0.5f - atan(z.x / z.y);
	if (z.x < 0.0f)
		return atan(z.y / z.x) + pi;
	return 0.0f;
}

inline float2 ComplexMul(float2 z1, float2 z2)
{
	return float2(z1.x * z2.x - z1.y * z2.y, z1.x * z2.y + z1.y * z2.x);
}
inline float2 ComplexDiv(float2 z1, float2 z2)
{
	return float2(z1.x * z2.x + z1.y * z2.y, z1.y * z2.x - z1.x * z2.y) / (z2.x * z2.x + z2.y * z2.y);
}
inline float2 ComplexExp(float2 z)
{
	return float2(cos(z.y), sin(z.y)) * exp(z.x);
}
inline float2 ComplexLog(float2 z)
{
	return float2(log(length(z)), angle(z));
}
inline float2 ComplexPow(float2 z1, float2 z2)
{
	return ComplexExp(ComplexMul(z2, ComplexLog(z1)));
}
inline float2 Function(float2 z, float2 c)
{
	return )";

	const std::string g_MandelbrotFractalCode2 = R"(;
}

float MandelbrotIterations(float2 coord)
{
	float2 z = float2(0.0f, 0.0f) + offset;
	float2 c = coord;
	for (float i = 0.0f; i < iterationCount; i += 1.0f)
	{
		z = Function(z, c);
		if (length(z) > 4.0f)
			return i / 256.0f;
	}
	return 0.0f;
}
float4 ToColor(in float ratio)
{
    float r = fmod(ratio, 1.0f);
	float R = abs(r * 6.0f - 3.0f) - 1.0f;
	float G = 2.0f - abs(r * 6.0f - 2.0f);
	float B = 2.0f - abs(r * 6.0f - 4.0f);
    r = 1.0f - R * 0.49f;
	return saturate(float4(R * r, G * r, B * r, 1.0f));
}

float2 Transform(float2 screen)
{
    return (screen * 2.0 - resolution.xy) / (resolution.y * zoom) + center;
}

float4 main(PixelInputType input) : SV_TARGET
{
	float2 screen = input.windowPosition.xy;
	return ToColor(MandelbrotIterations(Transform(screen)));
}
)";

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
			m_ps = g_MandelbrotFractalCode1 + dest + g_MandelbrotFractalCode2;
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

	mth::float2 Mandelbrot::ScreenToCoord(mth::float2 screen)
	{
		return (screen - m_fractalParams.resolution * 0.5f) / (m_fractalParams.resolution.y * m_fractalParams.zoom) * 2.0f + m_fractalParams.center;
	}

	void Mandelbrot::Render()
	{
		m_paramWriter(m_fractalParams);
		m_graphics.Render2D(&m_fractalParams);
	}
	Mandelbrot::Mandelbrot(Graphics& graphics,
		std::function<void(FractalParamsMandelbrot&)> paramReader,
		std::function<void(FractalParamsMandelbrot&)> paramWriter) :
		Fractal(graphics, FractalType::Mandelbrot),
		m_paramReader(paramReader),
		m_paramWriter(paramWriter)
	{
		m_fractalParams.zoom = 0.75f;
		m_paramReader(m_fractalParams);
		m_fractalParams.resolution = m_graphics.ScreenSize();
		ChangeFunction("z*z+c");
		//m_graphics.SetFractalRenderer(g_MandelbrotFractalCode, sizeof(m_fractalParams));
		//Render();
	}
	void Mandelbrot::ChangeFunction(const char* function)
	{
		try
		{
			m_graphics.SetFractalRenderer(Mandelbrot_PSBuilder(function).PSCode(), sizeof(m_fractalParams));
			Render();
		}
		catch (...) {}
	}
	void Mandelbrot::OnWindowSizeChanged(float newWidth, float newHeight)
	{
		m_fractalParams.resolution = mth::float2(newWidth, newHeight);
		Render();
	}
	void Mandelbrot::OnFractalParamChanged()
	{
		m_paramReader(m_fractalParams);
		Render();
	}
	void Mandelbrot::OnPointerPressed(mth::float2 point, bool lmbDown, bool rmbDown)
	{
		if (rmbDown)
			m_fractalParams.offset = ScreenToCoord(point);
		m_prevCursor = point;
		Render();
	}
	void Mandelbrot::OnPointerMoved(mth::float2 point, bool lmbDown, bool rmbDown)
	{
		if (lmbDown)
			m_fractalParams.center -= 2.0f * (point - m_prevCursor) / (m_fractalParams.resolution.y * m_fractalParams.zoom);
		if (rmbDown)
			m_fractalParams.offset = ScreenToCoord(point);
		m_prevCursor = point;
		Render();
	}
	void Mandelbrot::OnPointerWheelChanged(mth::float2 point, int delta)
	{
		float zoomChange = delta > 0 ? 1.1f : 1.0f / 1.1f;
		m_fractalParams.zoom *= zoomChange;
		m_fractalParams.center += (point - m_fractalParams.resolution * 0.5f) / (m_fractalParams.resolution.y * m_fractalParams.zoom) * (2.0f * zoomChange - 2.0f);

		Render();
	}
}