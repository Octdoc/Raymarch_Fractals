#pragma once

#include "camera.h"
#include "fractal.h"

namespace fracapp
{
	struct FractalParams3D
	{
		mth::float3 position;
		mth::float3 rotation;

		float epsilon;
		float minDistance;
		float maxDistance;
		unsigned maxMarch;
		float ambientLight;
		float occlusionFactor;

		float angle1;
		float angle2;
		float angle3;
		float scale;
		mth::float3 offset;
		unsigned iterationCount;
	};

	class Fractal3D : public Fractal
	{
		FractalParams3D m_fractalParams;
		std::function<void(FractalParams3D&)> m_paramReader;
		Camera m_camera;

	private:
		void Render();

	public:
		Fractal3D(Graphics& graphics, std::function<void(FractalParams3D&)> paramReader);

		virtual void OnWindowSizeChanged(float newWidth, float newHeight) override;
		virtual void OnFractalParamChanged() override;
		virtual void OnPointerPressed(mth::float2 point, bool lmbDown, bool rmbDown) override;
		virtual void OnPointerMoved(mth::float2 point, bool lmbDown, bool rmbDown) override;
		virtual void OnPointerWheelChanged(mth::float2 point, int delta) override;
	};
}