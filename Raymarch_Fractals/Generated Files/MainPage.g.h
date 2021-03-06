﻿#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------


namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class SwapChainPanel;
                ref class FlipView;
                ref class Slider;
                ref class TextBox;
            }
        }
    }
}

namespace Raymarch_Fractals
{
    [::Windows::Foundation::Metadata::WebHostHidden]
    partial ref class MainPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector,
        public ::Windows::UI::Xaml::Markup::IComponentConnector2
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
        virtual ::Windows::UI::Xaml::Markup::IComponentConnector^ GetBindingConnector(int connectionId, ::Platform::Object^ target);
    
    private:
        void UnloadObject(::Windows::UI::Xaml::DependencyObject^ dependencyObject);
        void DisconnectUnloadedObject(int connectionId);
    
    private:
        bool _contentLoaded;
    
        class MainPage_obj1_Bindings;
    
        private: ::Windows::UI::Xaml::Controls::SwapChainPanel^ sc_fractalCanvas;
        private: ::Windows::UI::Xaml::Controls::FlipView^ fv_paramSetter;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3Dangle1;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3Dangle2;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3Dangle3;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3Dscale;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3DoffsetX;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3DoffsetY;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3DoffsetZ;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3Diters;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3Dambient;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3Docclusion;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3Depsilon;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3DminDist;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3DmaxDist;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_3DmaxMarch;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_NTiter;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_NTzoom;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_NTcenterY;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_NTcenterX;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_NTstepIm;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_NTstepRe;
        private: ::Windows::UI::Xaml::Controls::Slider^ sl_MBiter;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_MBzoom;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_MBfunction;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_MBcenterY;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_MBcenterX;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_MBoffsetY;
        private: ::Windows::UI::Xaml::Controls::TextBox^ tb_MBoffsetX;
    };
}

