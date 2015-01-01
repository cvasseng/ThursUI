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

#ifndef h__thurs__window__
#define h__thurs__window__

#include "sigslot.h"

#include "thurs.surface.hpp"
#include "thurs.skin.hpp"

namespace thurs {

  class Window : public Surface {
  public:

    ///////////////////////////////////////////////////////////////////////////

    Window(Surface* surface);
    virtual ~Window();

    ///////////////////////////////////////////////////////////////////////////

    //Hide window
    void hide();
    //Show window
    void show();

    ///////////////////////////////////////////////////////////////////////////
    // Signals

    sigslot::signal0<> OnHide;
    sigslot::signal0<> OnShow;

    ///////////////////////////////////////////////////////////////////////////

    virtual void setSkinClass(const std::string& name);
    bool reloadSkin();
    void setPos(float x, float y);
    void setPos(const Vector2f& vec);
    bool mouseOver();
  
    ///////////////////////////////////////////////////////////////////////////
  
    //The window title
    std::string Title;
    //Can close?
    bool CanClose;
    //Can move?
    bool CanMove;
    //Can collapse?
    bool CanCollapse;
  protected:
    //The window size
    Vector2f m_winSize;
    //The window position
    Vector2f m_winPos;

    //We override this to draw the window itself
    void _onUpdate();
  private:
    //The internal window id
    uint16 m_id;
    //The owner surface
    Surface* m_owner;
    //Mouse delta
    Vector2s m_mdelta;
    //Pre move pos
    Vector2f m_preMovePos;
    //Moving?
    bool m_moving;
    //Height of titlebar
    uint16 m_titlebarHeight;
    //Showing?
    bool m_visible;
    //Collapsed?
    bool m_collapsed;
    //The class name
    std::string m_skinClassName;

    //The window skin class
    Skin::SkinClass m_skinClass;
    //The title bar skin class
    Skin::SkinClass* m_titlebarClass;
    //The close icon class
    Skin::SkinClass* m_closeClass;
    //The collapse icon class
    Skin::SkinClass* m_collapseClass;
  };

}

#endif
