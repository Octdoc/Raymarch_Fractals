#pragma once

#include "pch.h"
#include "graphics.h"

namespace fracapp
{
	enum class FractalType
	{
		Mandelbrot,
		Newton,
		Fractal3D
	};

	class Fractal
	{
	protected:
		Graphics& m_graphics;
		const FractalType m_type;

	public:
		Fractal(Graphics& graphics, FractalType type);

		virtual void OnWindowSizeChanged(float newWidth, float newHeight) = 0;
		virtual void OnFractalParamChanged() = 0;
		virtual void OnPointerPressed(mth::float2 point, bool lmbDown, bool rmbDown) = 0;
		virtual void OnPointerMoved(mth::float2 point, bool lmbDown, bool rmbDown) = 0;
		virtual void OnPointerWheelChanged(mth::float2 point, int delta) = 0;

		inline FractalType Type() const { return m_type; }
	};
}