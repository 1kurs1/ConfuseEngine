#pragma once

#include "Event.h"

namespace Confuse{
    class KeyEvent : public Event{
    public:
        inline int getKeyCode() const {return m_keyCode;}
        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(int keyCode) : m_keyCode(keyCode){}
        int m_keyCode;
    };

    class KeyPressedEvent : public KeyEvent{
    public:
        KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode), m_repeatCount(repeatCount){}

        inline int getRepeatCount() const {return m_repeatCount;}

        std::string toString() const override{
            std::stringstream ss;
            ss << "key pressed event: " << m_keyCode << " (" << m_repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int m_repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent{
    public:
        KeyReleasedEvent(int keyCode) : KeyEvent(keyCode){}

        std::string toString() const override{
            std::stringstream ss;
            ss << "key released event: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent{
    public:
        KeyTypedEvent(int keyCode) : KeyEvent(keyCode){}

        std::string toString() const override{
            std::stringstream ss;
            ss << "key typed event: " << m_keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}