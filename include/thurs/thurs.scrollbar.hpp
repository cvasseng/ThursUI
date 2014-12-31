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

#ifndef h__thurs_scrollbar__
#define h__thurs_scrollbar__

#include "thurs.types.hpp"
#include "thurs.renderer.hpp"
#include "thurs.input.hpp"
#include "thurs.skin.hpp"

namespace thurs {

  class Scrollbar {
  public:
    Scrollbar(Surface* surface);
    //Update the current value - handles mouse interactions
    const float& update(Vector2f size, Vector2f pos);
    //Return the current scroll position
    const float& val();
    //Set the scrollbar class
    void setClass(const std::string& name);
  protected:
    Renderer *m_renderer;
    Input *m_input;
    Skin *m_skin;

    Vector2f m_size;
    Vector2f m_pos;

    //The current scroll factor in pixels
    float m_scrollVal;
    //Is scrolling?
    bool m_scrolling;
    //Mouse delta
    Vector2s m_mdelta;
    //Scroll value prior to scrolling
    float m_initialVal;

    //kin class
    Skin::SkinClass m_skinClass;
    //The body
    Skin::SkinClass* m_bodyClass;
    //The bar
    Skin::SkinClass* m_barClass;
  private:
  };

}

#endif
