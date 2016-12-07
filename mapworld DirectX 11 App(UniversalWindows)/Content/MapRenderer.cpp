#include "pch.h"
#include "MapRenderer.h"

#include "..\Common\DirectXHelper.h"

using namespace mapworld_DirectX_11_App_UniversalWindows_;

using namespace DirectX;
using namespace Windows::Foundation;

MapRenderer::MapRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
		);
	CreateDeviceDependentResources();
}

void MapRenderer::Update()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

	// reset iterator to beginning of color vector
	m_it = m_tileColors.begin();
}

void MapRenderer::Render()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	D2D1_SIZE_F rtSize = context->GetSize();
	FLOAT side = 32.0f;
	FLOAT m_numberOfColumns = rtSize.width / side;
	FLOAT m_numberOfRows = rtSize.height / side;
	for (FLOAT y = 0; y <= m_numberOfRows * side; y += side)
	{
		for (FLOAT x = 0; x <= m_numberOfColumns * side; x += side)
		{
			DrawTile(x, y, side, GetNextColor());
		}
	}

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void MapRenderer::CreateDeviceDependentResources()
{
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_solidColorBrush)
		);

	m_tileColors.push_back(D2D1::ColorF(220.0f / 255.0f, 62.0f / 255.0f, 35.0f / 255.0f, 1.0f));
	m_tileColors.push_back(D2D1::ColorF(215.0f / 255.0f, 81.0f / 255.0f, 116.0f / 255.0f, 1.0f));
	m_tileColors.push_back(D2D1::ColorF(235.0f / 255.0f, 91.0f / 255.0f, 57.0f / 124.0f, 1.0f));
	m_tileColors.push_back(D2D1::ColorF(252.0f / 255.0f, 90.0f / 255.0f, 85.0f / 255.0f, 1.0f));
	m_tileColors.push_back(D2D1::ColorF(215.0f / 255.0f, 81.0f / 255.0f, 116.0f / 255.0f, 1.0f));
	m_tileColors.push_back(D2D1::ColorF(248.0f / 255.0f, 142.0f / 255.0f, 206.0f / 255.0f, 1.0f));
	m_tileColors.push_back(D2D1::ColorF(119.0f / 255.0f, 160.0f / 255.0f, 81.0f / 255.0f, 1.0f));
	m_tileColors.push_back(D2D1::ColorF(84.0f / 255.0f, 122.0f / 255.0f, 61.0f / 255.0f, 1.0f));
	m_tileColors.push_back(D2D1::ColorF(80.0f / 255.0f, 115.0f / 255.0f, 61.0f / 255.0f, 1.0f));
//	m_tileColors.push_back(D2D1::ColorF(1.0f, 0.0f, 0.0f, 1.0f));
}
void MapRenderer::ReleaseDeviceDependentResources()
{
	m_solidColorBrush.Reset();
}

void MapRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, FLOAT r, FLOAT g, FLOAT b)
{
	D2D1::ColorF color = D2D1::ColorF(r, g, b, 1.0f);
	m_solidColorBrush->SetColor(color);
	m_deviceResources->GetD2DDeviceContext()->FillRectangle(D2D1::RectF(x, y, x + side, y + side), m_solidColorBrush.Get());
}

void MapRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, D2D1::ColorF color)
{
	m_solidColorBrush->SetColor(color);
	m_deviceResources->GetD2DDeviceContext()->FillRectangle(D2D1::RectF(x, y, x + side, y + side), m_solidColorBrush.Get());
}

D2D1::ColorF MapRenderer::GetNextColor()
{
	D2D1::ColorF retval = D2D1::ColorF::White;
	if (m_it != m_tileColors.end())
	{
		retval = *m_it;
		++m_it;
	}
	else
	{
		m_it = m_tileColors.begin();
		retval = GetNextColor();
	}
	return retval;
}
