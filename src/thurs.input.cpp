/******************************************************************************


Copyright (c) 2014, Chris Vasseng - github.com/cvasseng - cvasseng@gmail.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

#include "../include/thurs/thurs.hpp"

namespace thurs {

  Input::Input() {
    m_mbutton = MB_NONE;
    m_mbuttonl = MB_NONE;
    m_handled = false;
  }

  //Inject mouse coordinates
  void Input::injectMouseCoords(uint16 x, uint16 y) {
    m_coords.x = x;
    m_coords.y = y;
    m_handled = false;
  }

  //Inject mouse button state
  void Input::injectMouseButton(MouseButton b) {
    m_mbuttonl = m_mbutton;
    m_mbutton = b;
    m_handled = false;
  }

  ///////////////////////////////////////////////////////////////////////////

  //Get the current mouse position
  const Vector2s& Input::mouseCoords() {
    return m_coords;
  }

  //Get the current mouse button
  const Input::MouseButton& Input::mouseButton() {
    return m_mbutton;
  }

  //Returns true if there was a mouse click
  bool Input::mouseClick(MouseButton btn) {
    return m_mbutton != btn && m_mbuttonl == btn;
  }

  //Returns true if the given mouse button was just pressed
  bool Input::mouseDown(MouseButton btn) {
    return m_mbutton == btn && m_mbuttonl != btn;
  }

  //Returns true if the given mouse button was just released
  bool Input::mouseUp(MouseButton btn) {
    return m_mbutton != btn && m_mbuttonl == btn;
  }

  //Mark input as handled 
  void Input::markHandled() {
    m_handled = true;
  }

  //Is the input handled? - can be used by the game engine to cancel
  //e.g. character movement etc.
  bool Input::handled() {
    return m_handled;
  }

}
