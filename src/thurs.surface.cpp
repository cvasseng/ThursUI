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

  Surface::Surface(Renderer *renderer, Input *input) : m_skin(renderer) {
    m_renderer = renderer;
    m_input = input;
    m_canvasSize.x = 1024;
    m_canvasSize.y = 1024;
    m_tooltipActive = false;
    m_focused = 0;
    m_focusedChild = 0;
  } 

  void Surface::resize(const Vector2f& vec) {
    m_canvasSize = vec;
  }

  void Surface::resize(float w, float h) {
    m_canvasSize.x = w;
    m_canvasSize.y = h;
  }

  void Surface::_onUpdate() {
    //We need to check if the mouse is over the child
    if (m_focusedChild && m_focusedChild->mouseOver()) {
      m_input->disable();
    } else {
      m_input->enable();
    }

    //Update the children
    for (uint32 i = 0; i < m_controls.size(); i++) {
      if (!m_focused || m_focused != m_controls[i]) {
        m_controls[i]->update();
      }
    }

    if (m_focused) {
      m_focused->update();
    }
  }

  void Surface::doTooltip(const std::string& text) {
    m_tooltipActive = true;
    m_tooltipClass.setState(S_ACTIVE);
    m_tooltipPos = m_input->mouseCoords();
  } 

  void Surface::cancelTooltip() {
    m_tooltipClass.setState(S_NORMAL);
    m_tooltipActive = false;
    m_tooltipClass.reset();
  }

  const Vector2f& Surface::getSize() {
    return m_canvasSize;
  }

  void Surface::updateAndRender(float pixelFormat) {

   // printf("W %f H %f\n", m_canvasSize.x, m_canvasSize.y);
    m_renderer->begin(m_canvasSize.x, m_canvasSize.y, pixelFormat);

    _onUpdate();

    //May need to rearrange things
    //This is a really shitty solution and needs to be patched.
    if (m_focused && m_controls.size() > 0 && m_controls[m_controls.size() - 1] != m_focused) {
      Control* ph = m_controls[m_controls.size() - 1];
      for (uint32 i = 0; i < m_controls.size(); i++) {
        if (m_controls[i] == m_focused) {
          m_controls[m_controls.size() - 1] = m_focused;
          m_controls[i] = ph;
          break;
        }
      }
    }

    if (m_tooltipActive) {
      m_renderer->renderRect(m_tooltipClass.Attr, Vector2f(m_tooltipPos.x, m_tooltipPos.y), Vector2f(100.f, 100.f));
      m_tooltipClass.update();
    }

    m_renderer->end();

    int32 focusedIndex = -1;

    m_moveFocusedChildToTop = false;

    for (uint32 i = 0; i < m_children.size(); i++) {
      if (!m_focusedChild || m_focusedChild == m_children[i]) {
        m_input->enable();
        focusedIndex = i;
      } else {
        m_input->disable();
      }

      m_children[i]->m_canvasSize = m_canvasSize;
      m_children[i]->updateAndRender(pixelFormat);
    }

    //If the focused child is not on the top of the stack, move it there.
    if ( m_children.size() > 1 && m_children.size() - 1 != focusedIndex) {
      Surface* p = m_children[m_children.size() - 1];
      m_children[m_children.size() - 1] = m_focusedChild;
      m_children[focusedIndex] = p;
    }

  }

  Skin* const Surface::skin() {
    return &m_skin;
  }

  //Returns the attached input handler
  Input* const Surface::input() {
    return m_input;
  }

  Renderer* const Surface::renderer() {
    return m_renderer;
  }

  bool Surface::reloadSkin() {
    if (m_skin.reload()) {

      for (uint32 i = 0; i < m_controls.size(); i++) {
        m_controls[i]->reloadSkinClass();
      }

      for (uint32 i = 0; i < m_children.size(); i++) {
        m_children[i]->reloadSkin();
      }

      return true;
    }
    return false;
  }

  bool Surface::loadSkin(const std::string& filename) {
    if (m_skin.load(filename)) {
      m_tooltipClass = m_skin.getClass("Tooltip");
      return true;
    }
    return false;
  }

}
