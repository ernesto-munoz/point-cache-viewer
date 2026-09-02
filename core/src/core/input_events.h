#pragma once
#pragma once

#include "event.h"

#include <format>

namespace core {

	class KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyEvent(int keycode)
			: m_KeyCode(keycode) {
		}

		int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, bool isRepeat)
			: KeyEvent(keycode), m_IsRepeat(isRepeat) {
		}

		inline bool IsRepeat() const { return m_IsRepeat; }

		std::string ToString() const override
		{
			return std::format("KeyPressedEvent: {} (repeat={})", m_KeyCode, m_IsRepeat);
		}

		EVENT_CLASS_TYPE(kKeyPressed)
	private:
		bool m_IsRepeat;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {
		}

		std::string ToString() const override
		{
			return std::format("KeyReleasedEvent: {}", m_KeyCode);
		}

		EVENT_CLASS_TYPE(kKeyReleased)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(double x, double y)
			: m_MouseX(x), m_MouseY(y) {
		}

		inline double GetX() const { return m_MouseX; }
		inline double GetY() const { return m_MouseY; }

		std::string ToString() const override
		{
			return std::format("MouseMovedEvent: {}, {}", m_MouseX, m_MouseY);
		}

		EVENT_CLASS_TYPE(kMouseMoved)
	private:
		double m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(double xOffset, double yOffset)
			: m_XOffset(xOffset), m_YOffset(yOffset) {
		}

		inline double GetXOffset() const { return m_XOffset; }
		inline double GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			return std::format("MouseScrolledEvent: {}, {}", m_XOffset, m_YOffset);
		}

		EVENT_CLASS_TYPE(kMouseScrolled)
	private:
		double m_XOffset, m_YOffset;
	};

	class MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }
		inline double GetX() const { return m_MouseX; }
		inline double GetY() const { return m_MouseY; }
	protected:
		MouseButtonEvent(int button, double x, double y)
			: m_Button(button), m_MouseX(x), m_MouseY(y) {
		}

		int m_Button;
		double m_MouseX, m_MouseY;
	};

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button, double x, double y)
			: MouseButtonEvent(button, x, y) {
		}

		std::string ToString() const override
		{
			return std::format("MouseButtonPressedEvent: {} ({},{})", m_Button, m_MouseX, m_MouseY);
		}

		EVENT_CLASS_TYPE(kMouseButtonPressed)
	};

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button, double x, double y)
			: MouseButtonEvent(button, x, y) {
		}

		std::string ToString() const override
		{
			return std::format("MouseButtonReleasedEvent: {} ({},{})", m_Button, m_MouseX, m_MouseY);
		}

		EVENT_CLASS_TYPE(kMouseButtonReleased)
	};
}
