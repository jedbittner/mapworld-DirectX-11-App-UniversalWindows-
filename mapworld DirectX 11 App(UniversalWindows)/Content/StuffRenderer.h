#pragma once

#include "..\Common\DeviceResources.h"
#include <vector>
using namespace std;

namespace mapworld_DirectX_11_App_UniversalWindows_
{
	class StuffRenderer
	{
	public:
		StuffRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update();
		void Render();
		void ChangeState();
	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// state machine
		enum eState {getDominos, displayDominos, displayAnswer};
		eState m_state = getDominos;
		
		// Resources related to text rendering.
		std::wstring                                    m_text;
		DWRITE_TEXT_METRICS	                            m_textMetrics;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_whiteBrush;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		Microsoft::WRL::ComPtr<IDWriteTextLayout>       m_textLayout;
		Microsoft::WRL::ComPtr<IDWriteTextFormat>		m_textFormat;

		// domino stuff
		int m_maxNumberOfDominos = 8;
		int m_totalDominoValues = 0;
		TCHAR m_Answer[256];
		TCHAR m_DisplayAnswer[256];

		// <<<jedb>>> 09/09/2020 start
		struct domino {
			int lefthalf;
			int righthalf;
		};
		vector<domino> m_doms;
		vector<domino>::iterator m_iter;
		int m_numberOfDominos;
		int m_numDomsDisplayed;
		// <<<jedb>>> 09/09/2020 end

		float m_dominoLocationX = 150;
		float m_dominoLocationY = 120;
		float m_spaceBetweenDominos = 80;
		float m_spaceBetweenDominoHalves = 60;
		int m_dominoDotSize = 8;
		int m_distanceBetweenDots = 8;

		void StuffRenderer::ShuffleDominos();

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_solidColorBrush;

		void StuffRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, FLOAT r, FLOAT g, FLOAT b);
		void StuffRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, D2D1::ColorF color);
		void StuffRenderer::DrawDot(FLOAT x, FLOAT y, FLOAT r, FLOAT g, FLOAT b);
		void StuffRenderer::DrawDominoHalf(int number, FLOAT x, FLOAT y, FLOAT r, FLOAT g, FLOAT b);
		void StuffRenderer::DrawBackground(FLOAT x, FLOAT y);
	};
}

