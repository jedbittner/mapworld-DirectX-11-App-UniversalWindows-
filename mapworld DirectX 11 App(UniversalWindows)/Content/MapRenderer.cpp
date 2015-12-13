#include "pch.h"
#include "MapRenderer.h"

#include "..\Common\DirectXHelper.h"

using namespace mapworld_DirectX_11_App_UniversalWindows_;

using namespace DirectX;
using namespace Windows::Foundation;

MapRenderer::MapRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
}

void MapRenderer::Update()
{
}

void MapRenderer::Render()
{
}
