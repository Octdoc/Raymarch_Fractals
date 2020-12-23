#include "pch.h"
#include "fractal.h"

namespace fracapp
{
	Fractal::Fractal(Graphics& graphics, FractalType type) :
		m_graphics(graphics),
		m_type(type) {}
}