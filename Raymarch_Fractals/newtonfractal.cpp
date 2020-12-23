#include "pch.h"
#include "newtonfractal.h"

namespace fracapp
{
	const char* g_NewtonFractalCode = R"(
cbuffer Data
{
	float2 resolution;
	float2 step;
	float2 center;
	float zoom;
	float iterationCount;
};
struct PixelInputType
{
	float4 windowPosition : SV_POSITION;
};

inline float2 ComplexMul(float2 z1, float2 z2)
{
	return float2(z1.x * z2.x - z1.y * z2.y, z1.x * z2.y + z1.y * z2.x);
}
inline float2 ComplexDiv(float2 z1, float2 z2)
{
	return float2(z1.x * z2.x + z1.y * z2.y, z1.y * z2.x - z1.x * z2.y) / (z2.x * z2.x + z2.y * z2.y);
}

float2 Func(float2 z)
{
	return ComplexMul(z, ComplexMul(z, z)) - float2(1.0f, 0.0f);
}
float2 Deriv(float2 z)
{
	return 3.0f * ComplexMul(z, z);
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

bool IsNear(float2 z)
{
	float tolerance = 1e-5f;
	if (length(z - float2(1.0f, 0.0f)) < tolerance)
		return true;
	if (length(z - float2(-0.5f, sqrt(3.0f) * 0.5f)) < tolerance)
		return true;
	if (length(z - float2(-0.5f, sqrt(3.0f) * -0.5f)) < tolerance)
		return true;
	return false;
}

float NewtonIterations(float2 coord)
{
	float2 z = coord;
	for (float i = 0.0f; i < iterationCount; i += 1.0f)
	{
		z -= ComplexMul(step, ComplexDiv(Func(z), Deriv(z)));
		if (IsNear(z))
			return i / 256.0f;
	}
return 0.0f;
}

float2 Transform(float2 screen)
{
    return (screen * 2.0 - resolution.xy) / (resolution.y * zoom) + center;
}

float4 main(PixelInputType input) : SV_TARGET
{
	return ToColor(NewtonIterations(Transform(input.windowPosition.xy)));
}
)";
	mth::float2 NewtonFractal::ScreenToCoord(mth::float2 screen)
	{
		return (screen - m_fractalParams.resolution * 0.5f) / (m_fractalParams.resolution.y * m_fractalParams.zoom) * 2.0f + m_fractalParams.center;
	}
	void NewtonFractal::Render()
	{
		m_paramWriter(m_fractalParams);
		m_graphics.Render2D(&m_fractalParams);
	}
	NewtonFractal::NewtonFractal(Graphics& graphics,
		std::function<void(FractalParamsNewton&)> paramReader,
		std::function<void(FractalParamsNewton&)> paramWriter) :
		Fractal(graphics, FractalType::Newton),
		m_paramReader(paramReader),
		m_paramWriter(paramWriter)
	{
		m_fractalParams.zoom = 0.75f;
		m_fractalParams.step.x = 1.0f;
		m_graphics.SetFractalRenderer(g_NewtonFractalCode, sizeof(m_fractalParams));
		m_paramReader(m_fractalParams);
		m_fractalParams.resolution = m_graphics.ScreenSize();
		Render();
	}

	void NewtonFractal::OnWindowSizeChanged(float newWidth, float newHeight)
	{
		m_fractalParams.resolution = mth::float2(newWidth, newHeight);
		Render();
	}
	void NewtonFractal::OnFractalParamChanged()
	{
		m_paramReader(m_fractalParams);
		Render();
	}
	void NewtonFractal::OnPointerPressed(mth::float2 point, bool lmbDown, bool rmbDown)
	{
		if (rmbDown)
			m_fractalParams.step = ScreenToCoord(point);
		m_prevCursor = point;
		Render();
	}
	void NewtonFractal::OnPointerMoved(mth::float2 point, bool lmbDown, bool rmbDown)
	{
		if (lmbDown)
			m_fractalParams.center -= 2.0f * (point - m_prevCursor) / (m_fractalParams.resolution.y * m_fractalParams.zoom);
		if (rmbDown)
			m_fractalParams.step = ScreenToCoord(point);
		m_prevCursor = point;
		Render();
	}
	void NewtonFractal::OnPointerWheelChanged(mth::float2 point, int delta)
	{
		float zoomChange = delta > 0 ? 1.1f : 1.0f / 1.1f;
		m_fractalParams.zoom *= zoomChange;
		m_fractalParams.center += (point - m_fractalParams.resolution * 0.5f) / (m_fractalParams.resolution.y * m_fractalParams.zoom) * (2.0f * zoomChange - 2.0f);

		Render();
	}
}