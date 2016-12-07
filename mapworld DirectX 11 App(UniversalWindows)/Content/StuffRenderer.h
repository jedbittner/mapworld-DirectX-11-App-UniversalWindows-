#pragma once

#include "..\Common\DeviceResources.h"
#include <vector>

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

		enum eState {getDominos, displayDominos};
		eState m_state = getDominos;
		std::vector<int>			m_dominoHalves;  // halves are sequential
		std::vector<int>::iterator	m_it;
		int m_maxNumberOfDominos = 8;
		int m_maxNumberOfDominoHalves = m_maxNumberOfDominos * 2;
		int m_numDoms = 0;				// random number of dominos, set for each turn
		int m_totalDominoValues = 0;

		float m_dominoLocationX = 150;
		float m_dominoLocationY = 120;
		float m_spaceBetweenDominos = 80;
		float m_spaceBetweenDominoHalves = 60;
		int m_dominoDotSize = 8;
		int m_distanceBetweenDots = 8;

		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>    m_solidColorBrush;

		void StuffRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, FLOAT r, FLOAT g, FLOAT b);
		void StuffRenderer::DrawTile(FLOAT x, FLOAT y, FLOAT side, D2D1::ColorF color);
		void StuffRenderer::DrawDot(FLOAT x, FLOAT y, FLOAT r, FLOAT g, FLOAT b);
		void StuffRenderer::DrawDominoHalf(int number, FLOAT x, FLOAT y, FLOAT r, FLOAT g, FLOAT b);
		void StuffRenderer::DrawBackground(FLOAT x, FLOAT y);
	};
}

