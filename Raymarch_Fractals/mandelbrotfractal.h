#pragma once

#include "fractal.h"

namespace fracapp
{
	struct FractalParamsMandelbrot
	{
		mth::float2 resolution;
		mth::float2 offset;
		mth::float2 center;
		float zoom;
		float iterationCount;
	};

	class Mandelbrot : public Fractal
	{
		FractalParamsMandelbrot m_fractalParams;
		std::function<void(FractalParamsMandelbrot&)> m_paramReader;
		std::function<void(FractalParamsMandelbrot&)> m_paramWriter;
		mth::float2 m_prevCursor;

	private:
		mth::float2 ScreenToCoord(mth::float2 screen);
		void Render();

	public:
		Mandelbrot(Graphics& graphics,
			std::function<void(FractalParamsMandelbrot&)> paramReader,
			std::function<void(FractalParamsMandelbrot&)> paramWriter);

		void ChangeFunction(const char* function);

		virtual void OnWindowSizeChanged(float newWidth, float newHeight) override;
		virtual void OnFractalParamChanged() override;
		virtual void OnPointerPressed(mth::float2 point, bool lmbDown, bool rmbDown) override;
		virtual void OnPointerMoved(mth::float2 point, bool lmbDown, bool rmbDown) override;
		virtual void OnPointerWheelChanged(mth::float2 point, int delta) override;
	};
}