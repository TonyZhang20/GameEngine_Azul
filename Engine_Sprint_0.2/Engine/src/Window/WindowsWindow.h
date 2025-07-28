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
		void GUIClearnUp();
		void SetupImGUI();

		virtual void SetTitle(const char* title) override;

		void OnUpdate(bool &quit) override;
		
		void DrawUI();
		void OnRenderUI(class StateRenderTargetView& renderTargetView);


		inline void* GetNativeHandle() const override { return hwnd; }
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		bool IsOpen() const override { return hwnd != nullptr; }

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }

		Vec4 GetWindowColor();

	private:
		Vec4 WindowColor;

		bool CreateWindowClass();
		bool CreateWindowInstance();

		virtual void Init(const WindowProps& props);

		Window* m_Window;

		struct WindowData
		{
			char* Title;
			bool VSync;

			unsigned int Width;
			unsigned int Height;

			EventCallbackFn EventCallback;
		};

		HWND hwnd = nullptr;
		HINSTANCE instance = nullptr;


		WindowData m_Data;
	};
}

#endif