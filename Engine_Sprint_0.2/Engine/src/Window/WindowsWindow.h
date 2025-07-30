#ifndef WINDOWS_WINDOW_H
#define WINDOWS_WINDOW_H

#include "Window.h"
#include <Windows.h>

namespace Azul
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow() = delete;
		WindowsWindow(HINSTANCE hInstance, const WindowProps& props);
		virtual ~WindowsWindow() override;

		virtual bool Create() override;
		virtual void Destroy() override;

		void Show() override;
		void Hide() override;

		void SetVsync(bool b);
		void Present() override;

		virtual void SetTitle(const char* title) override;
		virtual bool GetVsync() override { return m_Data.VSync; }

		void OnUpdate(bool &quit) override;
		void OnEvent(Event& e) override;
		void OnWindowClose(Event& e);
		
		void QuitCallBack();
		void DrawUI();

		inline void* GetNativeHandle() const override { return m_hwnd; }
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline float GetAspectRatio() const override { return (float)m_Data.Width / (float)m_Data.Height; }
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback;  UpdateWindow(m_hwnd);}

		bool IsOpen() const override { return m_hwnd != nullptr; }

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		
		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		Vec4 GetWindowColor() override;
		inline void SetWindowColor(Vec4 color) { this->WindowColor = color; }

	private:
		Vec4 WindowColor;

		bool CreateWindowClass();
		bool CreateWindowInstance();

		virtual void Init(const WindowProps& props);

		struct WindowData
		{
			char* Title;
			bool VSync = true;

			unsigned int Width;
			unsigned int Height;
			EventCallbackFn EventCallback;
		};

		HWND m_hwnd = nullptr;
		HINSTANCE instance = nullptr;
		WindowData m_Data;

	};
}

#endif