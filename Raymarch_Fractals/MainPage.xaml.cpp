//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"


using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;


namespace Raymarch_Fractals
{
	void MainPage::SubscribeToUI()
	{
		fv_paramSetter->SelectionChanged += ref new Windows::UI::Xaml::Controls::SelectionChangedEventHandler(this, &Raymarch_Fractals::MainPage::OnFractalSelectionChanged);

		sl_MBiter->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::OnMandelbrotSliderValueChanged);
		tb_MBfunction->TextChanged += ref new Windows::UI::Xaml::Controls::TextChangedEventHandler(this, &Raymarch_Fractals::MainPage::OnMandelbrotFunctionChanged);

		sl_NTiter->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::OnNewtonSliderValueChanged);

		sl_3Depsilon->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3DminDist->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3DmaxDist->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3DmaxMarch->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3Dambient->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3Docclusion->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3Dangle1->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3Dangle2->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3Dangle3->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3Dscale->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3DoffsetX->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3DoffsetY->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3DoffsetZ->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);
		sl_3Diters->ValueChanged += ref new Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, &Raymarch_Fractals::MainPage::On3DFractalSliderValueChanged);

		sc_fractalCanvas->SizeChanged += ref new Windows::UI::Xaml::SizeChangedEventHandler(this, &Raymarch_Fractals::MainPage::OnSizeChanged);
		sc_fractalCanvas->PointerPressed += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &Raymarch_Fractals::MainPage::OnPointerPressed);
		sc_fractalCanvas->PointerMoved += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &Raymarch_Fractals::MainPage::OnPointerMoved);
		sc_fractalCanvas->PointerWheelChanged += ref new Windows::UI::Xaml::Input::PointerEventHandler(this, &Raymarch_Fractals::MainPage::OnPointerWheelChanged);
	}
	void MainPage::ReadMandelbrotFractalParams(fracapp::FractalParamsMandelbrot& params)
	{
		params.iterationCount = static_cast<float>(sl_MBiter->Value);
	}
	void MainPage::WriteMandelbrotFractalParams(fracapp::FractalParamsMandelbrot& params)
	{
		tb_MBcenterX->Text = ref new Platform::String(std::to_wstring(params.center.x).c_str());
		tb_MBcenterY->Text = ref new Platform::String(std::to_wstring(params.center.y).c_str());
		tb_MBoffsetX->Text = ref new Platform::String(std::to_wstring(params.offset.x).c_str());
		tb_MBoffsetY->Text = ref new Platform::String(std::to_wstring(params.offset.y).c_str());
		tb_MBzoom->Text = ref new Platform::String(std::to_wstring(params.zoom).c_str());
	}
	void MainPage::ReadNewtonFractalParams(fracapp::FractalParamsNewton& params)
	{
		params.iterationCount = static_cast<float>(sl_NTiter->Value);
	}
	void MainPage::WriteNewtonFractalParams(fracapp::FractalParamsNewton& params)
	{
		tb_NTcenterX->Text = ref new Platform::String(std::to_wstring(params.center.x).c_str());
		tb_NTcenterY->Text = ref new Platform::String(std::to_wstring(params.center.y).c_str());
		tb_NTstepRe->Text = ref new Platform::String(std::to_wstring(params.step.x).c_str());
		tb_NTstepIm->Text = ref new Platform::String(std::to_wstring(params.step.y).c_str());
		tb_NTzoom->Text = ref new Platform::String(std::to_wstring(params.zoom).c_str());
	}
	void MainPage::Read3DFractalParams(fracapp::FractalParams3D& params)
	{
		params.epsilon = static_cast<float>(sl_3Depsilon->Value);
		params.minDistance = static_cast<float>(sl_3DminDist->Value);
		params.maxDistance = static_cast<float>(sl_3DmaxDist->Value);
		params.maxMarch = static_cast<unsigned>(sl_3DmaxMarch->Value);
		params.ambientLight = static_cast<float>(sl_3Dambient->Value);
		params.occlusionFactor = static_cast<float>(sl_3Docclusion->Value);
		params.angle1 = static_cast<float>(sl_3Dangle1->Value);
		params.angle2 = static_cast<float>(sl_3Dangle2->Value);
		params.angle3 = static_cast<float>(sl_3Dangle3->Value);
		params.scale = static_cast<float>(sl_3Dscale->Value);
		params.offset.x = static_cast<float>(sl_3DoffsetX->Value);
		params.offset.y = static_cast<float>(sl_3DoffsetY->Value);
		params.offset.z = static_cast<float>(sl_3DoffsetZ->Value);
		params.iterationCount = static_cast<unsigned>(sl_3Diters->Value);
	}
	void MainPage::SetFractalToMandelbrot()
	{
		m_fractal = std::make_unique<fracapp::Mandelbrot>(*m_graphics,
			[this](fracapp::FractalParamsMandelbrot& params) {
				ReadMandelbrotFractalParams(params); },
			[this](fracapp::FractalParamsMandelbrot& params) {
				WriteMandelbrotFractalParams(params); }
			);
		std::wstring wfunc(tb_MBfunction->Text->Data());
		static_cast<fracapp::Mandelbrot*>(m_fractal.get())->ChangeFunction(std::string(wfunc.begin(), wfunc.end()).c_str());
	}
	void MainPage::SetFractalToNewton()
	{
		m_fractal = std::make_unique<fracapp::NewtonFractal>(*m_graphics,
			[this](fracapp::FractalParamsNewton& params) {
				ReadNewtonFractalParams(params); },
			[this](fracapp::FractalParamsNewton& params) {
				WriteNewtonFractalParams(params); }
			);
	}
	void MainPage::SetFractalTo3D()
	{
		m_fractal = std::make_unique<fracapp::Fractal3D>(*m_graphics,
			[this](fracapp::FractalParams3D& params) {
				Read3DFractalParams(params); });
	}
	void MainPage::OnMandelbrotSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
	{
		if (m_fractal && m_fractal->Type() == fracapp::FractalType::Mandelbrot)
			m_fractal->OnFractalParamChanged();
	}
	void MainPage::OnMandelbrotFunctionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::TextChangedEventArgs^ e)
	{
		if (m_fractal && m_fractal->Type() == fracapp::FractalType::Mandelbrot)
		{
			std::wstring wfunc(tb_MBfunction->Text->Data());
			static_cast<fracapp::Mandelbrot*>(m_fractal.get())->ChangeFunction(std::string(wfunc.begin(), wfunc.end()).c_str());
		}
	}
	void MainPage::OnNewtonSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
	{
		if (m_fractal && m_fractal->Type() == fracapp::FractalType::Newton)
			m_fractal->OnFractalParamChanged();
	}
	void MainPage::On3DFractalSliderValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
	{
		if (m_fractal && m_fractal->Type() == fracapp::FractalType::Fractal3D)
			m_fractal->OnFractalParamChanged();
	}
	void MainPage::OnSizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
	{
		if (m_graphics)
			m_graphics->Resize(static_cast<unsigned>(e->NewSize.Width), static_cast<unsigned>(e->NewSize.Height));
		if (m_fractal)
			m_fractal->OnWindowSizeChanged(e->NewSize.Width, e->NewSize.Height);
	}
	void MainPage::OnPointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
	{
		if (m_fractal)
		{
			auto p = e->GetCurrentPoint(nullptr);
			m_fractal->OnPointerPressed(mth::float2(p->Position.X, p->Position.Y), p->Properties->IsLeftButtonPressed, p->Properties->IsRightButtonPressed);
		}
	}
	void MainPage::OnPointerMoved(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
	{
		if (m_fractal)
		{
			auto p = e->GetCurrentPoint(nullptr);
			m_fractal->OnPointerMoved(mth::float2(p->Position.X, p->Position.Y), p->Properties->IsLeftButtonPressed, p->Properties->IsRightButtonPressed);
		}
	}
	void MainPage::OnPointerWheelChanged(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
	{
		if (m_fractal)
		{
			auto p = e->GetCurrentPoint(nullptr);
			m_fractal->OnPointerWheelChanged(mth::float2(p->Position.X, p->Position.Y), p->Properties->MouseWheelDelta);
		}
	}
	void MainPage::OnFractalSelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
	{
		switch (static_cast<fracapp::FractalType>(fv_paramSetter->SelectedIndex))
		{
		case fracapp::FractalType::Mandelbrot:
			SetFractalToMandelbrot();
			break;
		case fracapp::FractalType::Newton:
			SetFractalToNewton();
			break;
		case fracapp::FractalType::Fractal3D:
			SetFractalTo3D();
			break;
		}
	}

	MainPage::MainPage()
	{
		InitializeComponent();

		m_graphics = std::make_unique<fracapp::Graphics>();
		m_graphics->SetSwapChainPanel(sc_fractalCanvas);
		SubscribeToUI();
		SetFractalToMandelbrot();
	}
}