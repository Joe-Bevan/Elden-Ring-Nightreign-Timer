#include "D3D11App.h"

#include <cstdint>
#include <cstdio>

#include "Constants.h"
#include "Win32App.h"

#include "../ImGui/backends/imgui_impl_dx11.h"

// Really simple error checking macro
#if _DEBUG
#define D3D_VERIFY(x) { HRESULT __hr = x; if (FAILED(__hr)) { printf("HRESULT: (0x%x)", __hr); __debugbreak(); } }
#else
#define D3D_VERIFY(x) x
#endif

D3D11Application::D3D11Application(const Win32Application& overlay)
	: m_device(nullptr)
	, m_deviceContext(nullptr)
	, m_swapChain(nullptr)
	, m_backBuffer(nullptr)
	, m_backBufferRTV(nullptr)
{
	DXGI_SWAP_CHAIN_DESC swapchainDesc{};
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.SampleDesc.Quality = 0u;
	swapchainDesc.SampleDesc.Count = 1u;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.BufferCount = core::NumBackbuffers;
	swapchainDesc.OutputWindow = overlay.GetHWND();
	swapchainDesc.Windowed = true;
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // DIFFRENT FROM TUT
	swapchainDesc.Flags = 0u;

	constexpr D3D_FEATURE_LEVEL supportedFLs[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	// We only call D3D funcs from main thread.
	// Inform D3D to not worry about thread safety (can increase perf slightly)
	const uint32_t deviceFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;

	D3D_VERIFY(D3D11CreateDeviceAndSwapChain(
		nullptr,					/* Adapter */
		D3D_DRIVER_TYPE_HARDWARE,	/* Driver type */
		nullptr,					/* Software */
		deviceFlags,				/* Flags */
		supportedFLs,				/* Feature Levels */
		ARRAYSIZE(supportedFLs),	/* Num Feature Levels*/
		D3D11_SDK_VERSION,			/* SDK Version */
		&swapchainDesc,				/* Swapchain Desc */
		&m_swapChain,				/* pSwapchain */
		&m_device,					/* pDevice */
		nullptr,					/* Feature Level used */
		&m_deviceContext			/* pDeviceContext */
	));

	D3D_VERIFY(m_swapChain->GetBuffer(0u, IID_PPV_ARGS(&m_backBuffer)));
	D3D_VERIFY(m_device->CreateRenderTargetView(m_backBuffer, nullptr, &m_backBufferRTV));

	ImGui_ImplDX11_Init(m_device, m_deviceContext);
}

D3D11Application::~D3D11Application()
{
	ImGui_ImplDX11_Shutdown();

	m_deviceContext->Flush();

	m_backBufferRTV->Release();
	m_backBuffer->Release();

	m_swapChain->Release();
	m_deviceContext->Release();
	m_device->Release();
}

void D3D11Application::ClearBackbuffer()
{
	constexpr float clearColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_deviceContext->OMSetRenderTargets(1u, &m_backBufferRTV, nullptr);
	m_deviceContext->ClearRenderTargetView(m_backBufferRTV, clearColour);
}

void D3D11Application::Present()
{
	// No VSync because we manually FPS limit 
	D3D_VERIFY(m_swapChain->Present(1u, 0u));
}
