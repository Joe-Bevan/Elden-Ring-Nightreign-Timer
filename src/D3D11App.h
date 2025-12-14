#pragma once
#include <d3d11.h>

class Win32Application;

class D3D11Application
{
public:
	D3D11Application(const Win32Application& overlay);
	~D3D11Application();

	ID3D11Device* GetDevice() const { return m_device; }
	ID3D11DeviceContext* GetDeviceCtx() const { return m_deviceContext; }

	//! @brief Issue a clear on the backbuffer
	void ClearBackbuffer();

	//! @brief Present the backbuffer
	void Present();

private:
	//!{ 
	//! @brief D3D11 core
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	//!}

	//!{ 
	//! @brief Resources
	ID3D11Texture2D* m_backBuffer;
	ID3D11RenderTargetView* m_backBufferRTV;
	//!}
};