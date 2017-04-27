/*****************************************************************************************
 *                                                                                       *
 * OpenSpace                                                                             *
 *                                                                                       *
 * Copyright (c) 2014-2017                                                               *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#include <openspace/interaction/keyboardmousestate.h>


namespace openspace {
namespace interaction {

KeyboardMouseState::KeyboardMouseState() {

}

KeyboardMouseState::~KeyboardMouseState() {

}

bool KeyboardMouseState::handleKeyboard(Key key, KeyModifier modifier, KeyAction action) {
    if (action == KeyAction::Press) {
        _keysDown.push_back(std::pair<Key, KeyModifier>(key, modifier));
    }
    else if (action == KeyAction::Release) {
        // Remove all key pressings for 'key'
        _keysDown.remove_if([key](std::pair<Key, KeyModifier> keyModPair)
        { return keyModPair.first == key; });
    }
    return false;
}

bool KeyboardMouseState::handleMouseButton(MouseButton button, MouseAction action) {
    if (action == MouseAction::Press) {
        _mouseButtonsDown.push_back(button);
    }
    else if (action == MouseAction::Release) {
        // Remove all key pressings for 'button'
        _mouseButtonsDown.remove_if([button](MouseButton buttonInList)
        { return button == buttonInList; });
    }
    return false;
}

bool KeyboardMouseState::handleMousePosition(double mouseX, double mouseY) {
    _mousePosition = glm::dvec2(mouseX, mouseY);
    return false;
}

bool KeyboardMouseState::handleMouseScroll(double mouseScrollDelta) {
    _mouseScrollDelta = mouseScrollDelta;
    return false;
}

const std::list<std::pair<Key, KeyModifier> >& KeyboardMouseState::getPressedKeys() const {
    return _keysDown;
}

const std::list<MouseButton>& KeyboardMouseState::getPressedMouseButtons() const {
    return _mouseButtonsDown;
}

glm::dvec2 KeyboardMouseState::getMousePosition() const {
    return _mousePosition;
}

double KeyboardMouseState::getMouseScrollDelta() const {
    return _mouseScrollDelta;
}

bool KeyboardMouseState::isKeyPressed(std::pair<Key, KeyModifier> keyModPair) const {
    for (auto it = _keysDown.begin(); it != _keysDown.end(); it++) {
        if (*it == keyModPair) {
            return true;
        }
    }
    return false;
}

bool KeyboardMouseState::isKeyPressed(Key key) const {
    for (auto it = _keysDown.begin(); it != _keysDown.end(); it++) {
        if ((*it).first == key) {
            return true;
        }
    }
    return false;
}

bool KeyboardMouseState::isMouseButtonPressed(MouseButton mouseButton) const {
    for (auto it = _mouseButtonsDown.begin(); it != _mouseButtonsDown.end(); it++) {
        if (*it == mouseButton) {
            return true;
        }
    }
    return false;
}

} // namespace interaction
} // namespace openspace