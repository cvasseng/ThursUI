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

#ifndef h__thurs_surface__
#define h__thurs_surface__

#include <map>

#include "thurs.types.hpp"
#include "thurs.renderer.hpp"
#include "thurs.input.hpp"
#include "thurs.skin.hpp"

namespace thurs {

  class Control;
  class Window;

	//A UI Surface
	/*
		All controls live inside a surface.
	*/
	class Surface {
    friend class Control;
    friend class Window;
	public:
    typedef std::map<uint16, Control*> ControlMap;
    typedef ControlMap::iterator ControlMapIt;
    #define ControlMapPair(x, y) std::pair<uint16, Control*>(x, y)

    typedef std::map<uint16, Surface*> SurfaceMap;
    typedef SurfaceMap::iterator SurfaceMapIt;
    #define SurfaceMapPair(x, y) std::pair<uint16, Surface*>(x, y)

    ///////////////////////////////////////////////////////////////////////////

		Surface(Renderer *renderer, Input *input);
    virtual ~Surface(){}

    //Update the surface
    void updateAndRender(float pielFormat = 1.f);

		//Returns the attached input handler
		Input* const input();
    //Returns the attached renderer
    Renderer* const renderer();
    //Return the attached skin
    Skin* const skin();

    //Resize the surface
    virtual void resize(const Vector2f& vec);
    virtual void resize(float w, float h);

    //Load skin - delegates to m_skin.load(..)
    /*
      Having this here rather than injecting a skin makes it a bit more
      manageable to have different skins for different surfaces, since we
      won't have to maintain a bunch of skin classes outside the library.
    */
    bool loadSkin(const std::string& filename);
    //Reload skin
    virtual bool reloadSkin();
    //Get size
    virtual const Vector2f& getSize();

    //Do a tooltip
    virtual void doTooltip(const std::string& text);
    virtual void cancelTooltip();
    virtual bool mouseOver() { return true;}
	protected:
    //Our skin
    Skin m_skin;
	//Our renderer 
	Renderer *m_renderer;
	//Input injector
	Input *m_input;
    //Controls in the surface
    std::vector<Control*> m_controls;
    //Children in the surface
    std::vector<Surface*> m_children;
    //The size of the surface
    Vector2f m_canvasSize;
    //Pixel aspect
    float m_pixelAspect;
    //Focused control
    Control *m_focused;
    //Focused child
    Surface* m_focusedChild;
    //Set to true to move the focused child to the top the next frame
    bool m_moveFocusedChildToTop;
    //Visible?
    bool m_visible;

    //The active tooltip
    Skin::SkinClass m_tooltipClass;
    //Is a tooltip active?
    bool m_tooltipActive;
    //Tooltip text
    std::string m_tooltip;
    //Tooltip position
    Vector2s m_tooltipPos;


    virtual void _onUpdate();
	private:
	};

}

#endif
