#pragma once
#include <string>

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::##type; }\
								EventType GetEventType() const override { return GetStaticType(); }\
								const char* GetName() const override { return #type; }

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, 
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

class Event
{
public:
	bool Handled = false;

	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); }
};

class EventDispatcher
{
public:
	EventDispatcher(Event& event)
		: m_Event(event) {}

	template<typename T, typename F>
	bool Dispatch(const F& func) // func signature: bool func(Event& e)
	{
		if (m_Event.GetEventType() == T::GetStaticType())
		{
			m_Event.Handled = func(static_cast<T&>(m_Event));
			return true;
		}
		return false;
	}
private:
	Event& m_Event;
};
