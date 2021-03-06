#pragma once

#include "math/position.hpp"

namespace fracapp
{
	class Camera :public mth::Positionf
	{
		mth::float4x4 m_viewMatrix;
		mth::float4x4 m_projectionMatrix;
		mth::float2 m_prevCursor;
		mth::float3 m_center;
		float m_screenAspectRatio;
		float m_fov;
		float m_screenNear;
		float m_screenDepth;
		float m_distance;
		float m_sensitivity;
		bool m_tracking;

	private:
		void SetPosition();
		void SetProjection();

	public:
		Camera();

		void StartTracking(mth::float2 cursor);
		bool Track(mth::float2 cursor, bool left, bool right);
		void Zoom(float zoom);

		void ScreenAspectRatio(float screenAspectRatio);

		mth::float4x4 CameraMatrix() const;
		inline mth::float4x4 ViewMatrix() const { return m_viewMatrix; }
		inline mth::float4x4 ProjectionMatrix() const { return m_projectionMatrix; }
	};
}