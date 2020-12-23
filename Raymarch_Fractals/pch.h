//
// pch.h
//

#pragma once

#include <memory>
#include <vector>
#include <string>
#include <collection.h>
#include <ppltasks.h>
#include <d3d11_3.h>
#include <dxgi1_4.h>
#include "math/position.hpp"

#include "App.xaml.h"

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
		throw std::exception(std::to_string(hr).c_str());
}