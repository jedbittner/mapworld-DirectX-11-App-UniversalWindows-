#include "pch.h"
#include "MoveLookController.h"

using namespace mapworld_DirectX_11_App_UniversalWindows_;

MoveLookController::MoveLookController()
{
}

void MoveLookController::OnKeyDown(
	__in CoreWindow^ sender,
	__in KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

//	m_anyKey = true;	// trigger on any keypress
}

void MoveLookController::OnKeyUp(
	__in CoreWindow^ sender,
	__in KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	m_anyKey = true;
}

void MoveLookController::Initialize(_In_ CoreWindow^ window)
{

	// Opt in to recieve touch/mouse events.
	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MoveLookController::OnKeyDown);

	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MoveLookController::OnKeyUp);

}

bool MoveLookController::Update(CoreWindow ^window)
{
	// Poll our state bits that are set by the keyboard input events.
	if (m_anyKey)
	{
		m_anyKey = false;
		return true;
	}
	return false;
}
