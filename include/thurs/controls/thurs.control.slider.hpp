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

#ifndef h__thurs_control_slider__
#define h__thurs_control_slider__

#include <string>

#include "../sigslot.h"
#include "../thurs.control.hpp"
#include "../thurs.color.hpp"

namespace thurs {

  class Slider : public Control {
  public:
    //Constructor. Duh.
    Slider(uint32 id, Surface *surface);
    //Update and draw
    void update();

    void setSkinClass(const std::string& name);
    //Type
    virtual WidgetType type() { return WT_SLIDER; }

    int32 Min;
    int32 Max;
    int32 Value;

    sigslot::signal1<int> OnChange;

    //Caption
    std::string Caption;
  protected:
    Skin::SkinClass* m_knob;
    Skin::SkinClass* m_bar;

    bool m_isDragging;
    float m_preDrag;
    Vector2s m_mdelta;
  private:
  };

}

#endif
