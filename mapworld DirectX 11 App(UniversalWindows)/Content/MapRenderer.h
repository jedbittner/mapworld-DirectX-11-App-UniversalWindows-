#pragma once

#include "..\Common\DeviceResources.h"

namespace mapworld_DirectX_11_App_UniversalWindows_
{
	class MapRenderer
	{
	public:
		MapRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void Update();
		void Render();
	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
	};
}

