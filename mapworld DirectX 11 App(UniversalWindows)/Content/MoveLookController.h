#pragma once
#include "..\Common\DeviceResources.h"

using namespace Windows::UI::Core;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Devices::Input;

namespace mapworld_DirectX_11_App_UniversalWindows_
{
	ref class MoveLookController sealed
	{
	private:
		bool m_forward, m_back;         // States for movement
		bool m_left, m_right;
		bool m_up, m_down;
		bool m_anyKey;

	public:
		MoveLookController();

		void OnKeyDown(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::KeyEventArgs^ args
			);

		void OnKeyUp(
			_In_ Windows::UI::Core::CoreWindow^ sender,
			_In_ Windows::UI::Core::KeyEventArgs^ args
			);

		// Set up the Controls that this controller supports
		void Initialize(_In_ Windows::UI::Core::CoreWindow^ window);

		bool Update(Windows::UI::Core::CoreWindow ^window);


	};
}

