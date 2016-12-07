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

	m_anyKey = true;	// will this trigger on any keypress?

	// Figure out the command from the keyboard.
	if (Key == VirtualKey::W)     // Forward
		m_forward = true;
	if (Key == VirtualKey::S)     // Back
		m_back = true;
	if (Key == VirtualKey::A)     // Left
		m_left = true;
	if (Key == VirtualKey::D)     // Right
		m_right = true;
}

void MoveLookController::OnKeyUp(
	__in CoreWindow^ sender,
	__in KeyEventArgs^ args)
{
	Windows::System::VirtualKey Key;
	Key = args->VirtualKey;

	m_anyKey = false;	// will this trigger on any keypress?

						// Figure out the command from the keyboard.
	if (Key == VirtualKey::W)     // forward
		m_forward = false;
	if (Key == VirtualKey::S)     // back
		m_back = false;
	if (Key == VirtualKey::A)     // left
		m_left = false;
	if (Key == VirtualKey::D)     // right
		m_right = false;
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
	if (m_forward)
	{
		return true;
	}

	if (m_anyKey)
	{
		return true;
	}

	//////if (m_back)
	//////	m_moveCommand.y -= 1.0f;

	//////if (m_left)
	//////	m_moveCommand.x -= 1.0f;
	//////if (m_right)
	//////	m_moveCommand.x += 1.0f;

	//////if (m_up)
	//////	m_moveCommand.z += 1.0f;
	//////if (m_down)
	//////	m_moveCommand.z -= 1.0f;
	
	return false;
}
