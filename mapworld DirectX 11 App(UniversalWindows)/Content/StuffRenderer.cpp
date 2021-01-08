#include "pch.h"
#include "StuffRenderer.h"

#include "..\Common\DirectXHelper.h"
#include "..\Common\utility.h"

using namespace mapworld_DirectX_11_App_UniversalWindows_;

using namespace DirectX;
using namespace Windows::Foundation;

StuffRenderer::StuffRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_text(L""),
	m_deviceResources(deviceResources)
{
	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

	// Create device independent resources
	DX::ThrowIfFailed(
		m_deviceResources->GetDWriteFactory()->CreateTextFormat(
			L"Segoe UI",
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			32.0f,
			L"en-US",
			&m_textFormat
		)
	);

	DX::ThrowIfFailed(
		m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
	);

	DX::ThrowIfFailed(
		m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
		);
	CreateDeviceDependentResources();

	// seed random number generator
	time_t Time;
	time(&Time);
	srand((unsigned int)Time);

	// initialize variables
	
	// <<<jedb>>> 09/09/2020 start
	int dominoOffset = 0;
	for (int i = 0; i < 7; i++)	{		// right
		for (int j = i; j < 7; j++) {	// left
			m_doms.push_back(domino());
			m_doms[dominoOffset].lefthalf = j;
			m_doms[dominoOffset].righthalf = i;
			dominoOffset++;
		}
	}
	m_numberOfDominos = m_doms.size();
	// <<<jedb>>> 09/09/2020 end

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
		m_numDomsDisplayed = (rand() % m_maxNumberOfDominos) + 1;
		ShuffleDominos();
		for (int i = 0; i < m_numDomsDisplayed; i++)
		{
			m_totalDominoValues += m_doms[i].lefthalf + m_doms[i].righthalf;
		}
		swprintf(m_Answer, 256, L"%d for %d\n", m_totalDominoValues, (int)round((float)(m_totalDominoValues) / 5));
		m_text = L"";
		m_state = displayDominos;
	}
		break;
	case displayAnswer:
		// Update display text.
		m_text = m_Answer;
		DX::ThrowIfFailed(
			m_deviceResources->GetDWriteFactory()->CreateTextLayout(
				m_text.c_str(),
				(uint32)m_text.length(),
				m_textFormat.Get(),
				240.0f, // Max width of the input text.
				50.0f, // Max height of the input text.
				&m_textLayout
			)
		);

		DX::ThrowIfFailed(
			m_textLayout->GetMetrics(&m_textMetrics)
		);
		break;
	case displayDominos:
		break;
	}
}

void StuffRenderer::ChangeState()
{
	if (m_state == displayDominos)
		m_state = displayAnswer;
	else
		m_state = getDominos;
}

void StuffRenderer::Render()
{
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	D2D1_SIZE_F rtSize = context->GetSize();
	
	// domino code...
	m_dominoLocationX = (rtSize.width / 2) - (m_spaceBetweenDominoHalves / 2) - (m_dominoDotSize * 7 / 2);
	m_dominoLocationY = m_spaceBetweenDominos;

	int y = 0;
	for (int i = 0; i < m_numDomsDisplayed; i++)
	{
		DrawDominoHalf(m_doms[i].lefthalf, m_dominoLocationX, m_dominoLocationY + (y * m_spaceBetweenDominos), 0, 0, 0);
		DrawDominoHalf(m_doms[i].righthalf, m_dominoLocationX + m_spaceBetweenDominoHalves, m_dominoLocationY + (y * m_spaceBetweenDominos), 0, 0, 0);
		y++;
	}
									
	// Position answer on the bottom
	if (m_text.compare(L"") != 0)	// skip if text is blank
	{
		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
			logicalSize.Width - m_textMetrics.layoutWidth,
			logicalSize.Height - m_textMetrics.height
		);

		context->SetTransform(screenTranslation * m_deviceResources->GetOrientationTransform2D());

		DX::ThrowIfFailed(
			m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING)
		);

		context->DrawTextLayout(
			D2D1::Point2F(0.f, 0.f),
			m_textLayout.Get(),
			m_whiteBrush.Get()
		);
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

void StuffRenderer::CreateDeviceDependentResources()
{
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_solidColorBrush)
		);
	DX::ThrowIfFailed(
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_whiteBrush)
	);
}
void StuffRenderer::ReleaseDeviceDependentResources()
{
	m_solidColorBrush.Reset();
	m_whiteBrush.Reset();
}

void StuffRenderer::ShuffleDominos()
{
	int n = m_doms.size();
	int index_arr[256];	// max 28 dominos
	vector<domino> shuffled = m_doms;
	int index;
	for (int i = 0; i < n; i++)
		index_arr[i] = 0;
	for (int i = 0; i < n; i++) {
		do {
			index = rand() % n;
		} while (index_arr[index] != 0);
		index_arr[index] = 1;
		shuffled[i] = m_doms[index];
		// psuedo-random swap left and right domino sides (for display, not part of Fisher-Yates shuffle)
		if (index % 3) {
			int temp = shuffled[i].lefthalf;
			shuffled[i].lefthalf = shuffled[i].righthalf;
			shuffled[i].righthalf = temp;
		}
	}
	m_doms = shuffled;
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
