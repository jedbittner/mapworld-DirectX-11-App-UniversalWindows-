#include "pch.h"
#include "StuffRenderer.h"

#include "..\Common\DirectXHelper.h"
#include "..\Common\utility.h"

using namespace mapworld_DirectX_11_App_UniversalWindows_;

using namespace DirectX;
using namespace Windows::Foundation;

StuffRenderer::StuffRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
		);
	CreateDeviceDependentResources();
	// seed random number generator
	time_t Time;
	time(&Time);
	srand(Time);
	// initialize variables
	m_dominoHalves.clear();
}

void StuffRenderer::Update()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

	// domino stuff
	switch (m_state)
	{
	case getDominos:
	{
		m_totalDominoValues = 0;
		m_dominoHalves.clear();					// empty all contents
		int numberOfDominos = (rand() % m_maxNumberOfDominos) + 1;
		int numberOfDominoHalves = numberOfDominos * 2;
		for (int i = 0; i < (numberOfDominoHalves); i++)
		{
			int value = rand() % 7;
			m_dominoHalves.push_back(value);
			m_totalDominoValues += value;
		}
		m_it = m_dominoHalves.begin();			// reset iterator
		// temp until displayed on screen
		//////TCHAR buff[256];
		//////swprintf(buff, 256, L"%d for %d\n", m_totalDominoValues, (int)m_totalDominoValues / 5);
		//////OutputDebugString(buff);
		Trace(L"%d for %d\n", m_totalDominoValues, (int)round((float)(m_totalDominoValues) / 5));
		// end temp
		m_state = displayDominos;
	}
		break;
	case displayDominos:
		break;
	}
}

void StuffRenderer::ChangeState()
{
	m_state = getDominos;
}

void StuffRenderer::Render()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	D2D1_SIZE_F rtSize = context->GetSize();

	if (false)  // temporarily turn off
	{
		// draw bouncing box
		float x = rtSize.width / 20.0f;
		float y = rtSize.height / 20.0f;
		float xOffset = rand() % 32;
		float yOffset = rand() % 32;
		DrawTile(x + xOffset, y + yOffset, rtSize.height / 2, 128, 128, 0);

		// draw random dot
		x = rtSize.width - rtSize.width / 20.0f;
		y = rtSize.height / 20.0f;
		xOffset = rand() % 10;
		yOffset = rand() % 10;
		DrawDot(x + xOffset, y + yOffset, 128, 128, 0);

		// draw twice for different effect:
		for (int i = 0; i < 2; i++)
		{
			// draw random dot, larger
			x = rtSize.width - rtSize.width / 20.0f;
			y = rtSize.height / 20.0f * 2;
			xOffset = rand() % 10;
			yOffset = rand() % 10;
			DrawTile(x + xOffset, y + yOffset, 10, 128, 128, 0);
		}
	}

	// domino code...
	int y = 0;
	while (m_it != m_dominoHalves.end())
	{
		int leftSide = *m_it;
		++m_it;
		int rightSide = *m_it;
		++m_it;
		for (int x = 0; x < 2; x++)
		{
			DrawDominoHalf(leftSide, m_dominoLocationX, m_dominoLocationY + (y * m_spaceBetweenDominos), 0, 0, 0);
			DrawDominoHalf(rightSide, m_dominoLocationX + m_spaceBetweenDominoHalves, m_dominoLocationY + (y * m_spaceBetweenDominos), 0, 0, 0);
		}
		y++;
	}
	m_it = m_dominoHalves.begin();	// reset iterator to beginning for next time through

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void StuffRenderer::CreateDeviceDependentResources()
{
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_solidColorBrush)
		);
}
void StuffRenderer::ReleaseDeviceDependentResources()
{
	m_solidColorBrush.Reset();
}

void StuffRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, FLOAT r, FLOAT g, FLOAT b)
{
	D2D1::ColorF color = D2D1::ColorF(r, g, b, 1.0f);
	m_solidColorBrush->SetColor(color);
	m_deviceResources->GetD2DDeviceContext()->FillRectangle(D2D1::RectF(x, y, x + side, y + side), m_solidColorBrush.Get());
}

void StuffRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, D2D1::ColorF color)
{
	m_solidColorBrush->SetColor(color);
	m_deviceResources->GetD2DDeviceContext()->FillRectangle(D2D1::RectF(x, y, x + side, y + side), m_solidColorBrush.Get());
}

void StuffRenderer::DrawDot(FLOAT x, FLOAT y, FLOAT r, FLOAT g, FLOAT b)
{
	D2D1::ColorF color = D2D1::ColorF(r, g, b, 1.0f);
	m_solidColorBrush->SetColor(color);
	// currently set to 3 pixel dot:
	m_deviceResources->GetD2DDeviceContext()->FillRectangle(D2D1::RectF(x, y, x + 3, y + 3), m_solidColorBrush.Get());
}

void StuffRenderer::DrawBackground(FLOAT x, FLOAT y)
{
	DrawTile(x - m_distanceBetweenDots, y - m_distanceBetweenDots, m_distanceBetweenDots * 7, 200, 200, 200);
}

void StuffRenderer::DrawDominoHalf(int number, FLOAT x, FLOAT y, FLOAT r, FLOAT g, FLOAT b)
{
	
	DrawBackground(x, y);
	switch (number)
	{
	case 0:
		break;
	case 1:
		DrawTile(x + (m_dominoDotSize +  m_distanceBetweenDots), y + (m_dominoDotSize + m_distanceBetweenDots), m_dominoDotSize, r, g, b);
		break;
	case 2:
		DrawTile(x, y, m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		break;
	case 3:
		DrawTile(x, y, m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize + m_distanceBetweenDots), y + (m_dominoDotSize + m_distanceBetweenDots), m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		break;
	case 4:
		DrawTile(x, y, m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), y, m_dominoDotSize, r, g, b);
		DrawTile(x, y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		break;
	case 5:
		DrawTile(x, y, m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), y, m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize + m_distanceBetweenDots), y + (m_dominoDotSize + m_distanceBetweenDots), m_dominoDotSize, r, g, b);
		DrawTile(x, y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		break;
	case 6:
		DrawTile(x, y, m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize + m_distanceBetweenDots), y, m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), y, m_dominoDotSize, r, g, b);
		DrawTile(x, y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize + m_distanceBetweenDots), y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		DrawTile(x + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), y + (m_dominoDotSize * 2 + m_distanceBetweenDots * 2), m_dominoDotSize, r, g, b);
		break;

	}
}
