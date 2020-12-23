//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "mandelbrotfractal.h"
#include "newtonfractal.h"
#include "frac3D.h"

namespace Raymarch_Fractals
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
		std::unique_ptr<fracapp::Graphics> m_graphics;
		std::unique_ptr<fracapp::Fractal> m_fractal;

	private:
		void SubscribeToUI();

		void ReadMandelbrotFractalParams(fracapp::FractalParamsMandelbrot& params);
		void WriteMandelbrotFractalParams(fracapp::FractalParamsMandelbrot& params);
		void ReadNewtonFractalParams(fracapp::FractalParamsNewton& params);
		void WriteNewtonFractalParams(fracapp::FractalParamsNewton& params);
		void Read3DFractalParams(fracapp::FractalParams3D& params);

		void SetFractalToMandelbrot();
		void SetFractalToNewton();
		void SetFractalTo3D();

		void OnMandelbrotSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void OnMandelbrotFunctionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e);
		void OnNewtonSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void On3DFractalSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void OnSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void OnPointerWheelChanged(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void OnFractalSelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);

	public:
		MainPage();
	};
}
