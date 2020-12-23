#include "pch.h"
#include "frac3d.h"

namespace fracapp
{
	static const char* g_3DFractalCode = R"(
cbuffer Data
{
	float3 cb_position;
	float cb_pitch;
	float cb_yaw;
	float cb_roll;

	float cb_epsilon;
	float cb_minDistance;
	float cb_maxDistance;
	uint cb_maxMarch;
	float cb_ambientLight;
	float cb_occlusionFactor;

	float cb_angle1;
	float cb_angle2;
	float cb_angle3;
	float cb_scale;
	float3 cb_offset;
	uint cb_iterationCount;
};
struct PixelInputType
{
	float4 windowPosition : SV_POSITION;
	float3 position : POSITION;
};

struct RayMarchData
{
	float3 position;
	float distance;
	float3 color;
	uint marchCount;
};

void mengerFold(inout float4 z)
{
	float a = min(z.x - z.y, 0.0);
	z.x -= a;
	z.y += a;
	a = min(z.x - z.z, 0.0);
	z.x -= a;
	z.z += a;
	a = min(z.y - z.z, 0.0);
	z.y -= a;
	z.z += a;
}
void rotX(inout float4 z, float s, float c)
{
	z.yz = float2(c * z.y + s * z.z, c * z.z - s * z.y);
}
void rotY(inout float4 z, float s, float c)
{
	z.xz = float2(c * z.x - s * z.z, c * z.z + s * z.x);
}
void rotZ(inout float4 z, float s, float c)
{
	z.xy = float2(c * z.x + s * z.y, c * z.y - s * z.x);
}
void rotX(inout float4 z, float a)
{
	rotX(z, sin(a), cos(a));
}
void rotY(inout float4 z, float a)
{
	rotY(z, sin(a), cos(a));
}
void rotZ(inout float4 z, float a)
{
	rotZ(z, sin(a), cos(a));
}
float de_box(float4 p, float3 s)
{
	float3 a = abs(p.xyz) - s;
	return (min(max(max(a.x, a.y), a.z), 0.0) + length(max(a, 0.0))) / p.w;
}
float de_sphere(float4 p, float r)
{
	return length(p.xyz) - r / p.w;
}
float DistanceEstimator(float3 q)
{
	float4 p = float4(q, 1.0f);
	//return de_sphere(p, 1.0f);
	for (int i = 0; i < cb_iterationCount; ++i) {
		p.xyz = abs(p.xyz);
		rotZ(p, cb_angle1);
		mengerFold(p);
		rotX(p, cb_angle2);
		mengerFold(p);
		rotY(p, cb_angle3);
		p *= cb_scale;
		p.xyz += cb_offset;
	}
	return de_box(p, float3(6.0f, 6.0f, 6.0f));
}

RayMarchData RayMarch(float3 raysrc, float3 raydir)
{
	RayMarchData rmData;
	rmData.color = float3(0.0f, 0.0f, 0.0f);
	rmData.position = raysrc;
	rmData.distance = 0.0f;
	for (rmData.marchCount = 0; rmData.marchCount < cb_maxMarch; rmData.marchCount++)
	{
		float d = DistanceEstimator(rmData.position);
		rmData.distance += d;
		float3 newPosition = raysrc + raydir * rmData.distance;
		rmData.color += abs(rmData.position - newPosition) * 0.1f;
		rmData.position = newPosition;
		if (d < cb_minDistance)
			return rmData;
		else if (rmData.distance > cb_maxDistance)
			return rmData;
	}
	return rmData;
}
float4 main(PixelInputType input) : SV_TARGET
{
//return float4(cb_angle1, cb_angle2, cb_angle3, 1.0f);
	float3 raydir = normalize(input.position);
	RayMarchData rmData = RayMarch(cb_position, raydir);
	float2 epsilon = float2(cb_epsilon, 0.0f);
	float3 normal = normalize(float3(
		DistanceEstimator(rmData.position - epsilon.xyy) - DistanceEstimator(rmData.position + epsilon.xyy),
		DistanceEstimator(rmData.position - epsilon.yxy) - DistanceEstimator(rmData.position + epsilon.yxy),
		DistanceEstimator(rmData.position - epsilon.yyx) - DistanceEstimator(rmData.position + epsilon.yyx)));
	float color = (rmData.distance < cb_maxDistance) ?
		dot(raydir, normal) * (1.0f - cb_ambientLight) + cb_ambientLight : 0.0f;
	color *= (1.0f - float(rmData.marchCount) / float(cb_maxMarch)) * cb_occlusionFactor + (1.0f - cb_occlusionFactor);
	return float4(color.xxx, 1.0f);	//color with lighting
}
)";

	void Fractal3D::Render()
	{
		m_fractalParams.position = m_camera.position;
		m_fractalParams.rotation = m_camera.rotation;
		m_graphics.Render3D(m_camera, &m_fractalParams);
	}

	Fractal3D::Fractal3D(Graphics& graphics, std::function<void(FractalParams3D&)> paramReader) :
		Fractal(graphics, FractalType::Fractal3D),
		m_paramReader(paramReader)
	{
		m_graphics.SetFractalRenderer(g_3DFractalCode, sizeof(m_fractalParams));
		m_paramReader(m_fractalParams);
		mth::float2 screenSize = m_graphics.ScreenSize();
		m_camera.ScreenAspectRatio(screenSize.x / screenSize.y);
		Render();
	}
	void Fractal3D::OnWindowSizeChanged(float newWidth, float newHeight)
	{
		m_camera.ScreenAspectRatio(newWidth / newHeight);
		Render();
	}
	void Fractal3D::OnFractalParamChanged()
	{
		m_paramReader(m_fractalParams);
		Render();
	}
	void Fractal3D::OnPointerPressed(mth::float2 point, bool lmbDown, bool rmbDown)
	{
		m_camera.StartTracking(point);
	}
	void Fractal3D::OnPointerMoved(mth::float2 point, bool lmbDown, bool rmbDown)
	{
		if (m_camera.Track(point, lmbDown, rmbDown))
			Render();
	}
	void Fractal3D::OnPointerWheelChanged(mth::float2 point, int delta)
	{
		m_camera.Zoom(delta > 0 ? 1.1f : 1.0f / 1.1f);
		Render();
	}
}