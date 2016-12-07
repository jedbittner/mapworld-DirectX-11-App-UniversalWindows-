#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"
#include "Content\MapRenderer.h"
#include "Content\StuffRenderer.h"
#include "Content\MoveLookController.h"

// Renders Direct2D and 3D content on the screen.
namespace mapworld_DirectX_11_App_UniversalWindows_
{
	class mapworld_DirectX_11_App_UniversalWindows_Main : public DX::IDeviceNotify
	{
	public:
		mapworld_DirectX_11_App_UniversalWindows_Main(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~mapworld_DirectX_11_App_UniversalWindows_Main();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;
		std::unique_ptr<MapRenderer> m_mapRenderer;
		std::unique_ptr<StuffRenderer> m_stuffRenderer;
		MoveLookController^ m_moveLookController;

		// Rendering loop timer.
		DX::StepTimer m_timer;
 	};
}