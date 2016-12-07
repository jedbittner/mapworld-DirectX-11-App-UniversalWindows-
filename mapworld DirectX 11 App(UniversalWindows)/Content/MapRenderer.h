#pragma once

#include "..\Common\DeviceResources.h"
#include <vector>

namespace mapworld_DirectX_11_App_UniversalWindows_
{
	class MapRenderer
	{
	public:
		MapRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update();
		void Render();
	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_solidColorBrush;
		std::vector<D2D1::ColorF>						m_tileColors;
		std::vector<D2D1::ColorF>::iterator				m_it;

		void MapRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, FLOAT r, FLOAT g, FLOAT b);
		void MapRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, D2D1::ColorF color);
		D2D1::ColorF MapRenderer::GetNextColor();
		void MapRenderer::DrawDot(FLOAT x, FLOAT y, FLOAT r, FLOAT g, FLOAT b);
	};
}

