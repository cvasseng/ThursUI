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

  Surface::Surface(Renderer *renderer, Input *input) {
    m_renderer = renderer;
    m_input = input;
    m_size.x = 1024;
    m_size.y = 1024;
  } 

  void Surface::_onUpdate() {
    //Update the children
    for (ControlMapIt it = m_controls.begin(); it != m_controls.end(); it++) {
      it->second->update();
    }
  }

  void Surface::updateAndRender() {
    m_renderer->begin(m_size.x, m_size.y, 1.f);

    _onUpdate();

    for (SurfaceMapIt it = m_children.begin(); it != m_children.end(); it++) {
      it->second->updateAndRender();
    }

    m_renderer->end();
  }

  //Returns the attached input handler
  Input* const Surface::input() {
    return m_input;
  }

  Renderer* const Surface::renderer() {
    return m_renderer;
  }

  bool Surface::loadSkin(const std::string& filename) {
    return m_skin.load(filename);
  }

}
