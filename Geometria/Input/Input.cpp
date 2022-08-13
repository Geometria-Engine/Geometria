#include "Input.h"
#include "../Graphics/Cores/MainAPI/Graphics.h"

std::vector<int> Input::checkKeys;
std::vector<int> Input::checkMouseButtons;

bool Input::GetKey(unsigned int keycode)
{
    if (keycode >= 1024)
    {
        return false;
    }

    return Graphics::GetMainWindow().keys[keycode];
}

bool Input::GetKeyDown(unsigned int keycode)
{
    if (keycode >= 1024)
    {
        return false;
    }

    if (Graphics::GetMainWindow().keys[keycode] && Graphics::GetMainWindow().keysState[keycode] == 0)
    {
        Graphics::GetMainWindow().keysState[keycode]++;
        Input::checkKeys.push_back(keycode);
    }
    else
    {
        return false;
    }

    return Graphics::GetMainWindow().keys[keycode];
}

void Input::UpdateKeyState()
{
    for (int i = 0; i < Input::checkKeys.size(); i++)
    {
        if (!Graphics::GetMainWindow().keys[Input::checkKeys[i]])
        {
            Graphics::GetMainWindow().keysState[Input::checkKeys[i]] = 0;
            Input::checkKeys.erase(Input::checkKeys.begin() + i);
        }
    }

    for (int i = 0; i < Input::checkMouseButtons.size(); i++)
    {
        if (!Graphics::GetMainWindow().mouse[Input::checkMouseButtons[i]])
        {
            Graphics::GetMainWindow().mouseState[Input::checkMouseButtons[i]] = 0;
            Input::checkMouseButtons.erase(Input::checkMouseButtons.begin() + i);
        }
    }
}

bool Input::GetMouseButton(unsigned int button)
{
    if (button >= 32)
    {
        return false;
    }
    return Graphics::GetMainWindow().mouse[button];
}

bool Input::GetMouseButtonDown(unsigned int button)
{
    if (button >= 32)
    {
        return false;
    }

    if (Graphics::GetMainWindow().mouse[button] && Graphics::GetMainWindow().mouseState[button] == 0)
    {
        Graphics::GetMainWindow().mouseState[button] = true;
        Input::checkMouseButtons.push_back(button);
    }
    else
    {
        return false;
    }

    return Graphics::GetMainWindow().mouse[button];
}
