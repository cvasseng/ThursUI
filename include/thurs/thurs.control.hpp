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

#ifndef h__thurs__control__
#define h__thurs__control__

#include "sigslot.h"
#include "thurs.types.hpp"
#include "thurs.surface.hpp"

namespace thurs {

  /*
    Skinning stuff:
      
    states:
      * normal
      * hover
      * active
      * focus
  
    {
      "normal": {
        "fill":"color",
        "stroke":"color",
        "fontSize": number,
        "font": "blah"
      },
      "hover": {
        "fill":"other color",
      }

    }

    Then tween everything based on that when states change.
    So have one master struct or whatever that contains the current state.

  */

  enum SkinState {
    S_NORMAL,  
    S_HOVER,   
    S_ACTIVE, 
    S_FOCUS,
    S_COUNT, 
  };


  class SkinPart {
  public:
    
    struct Atom {
      bool active;
      Color fill;
      Color stroke;
      Color text;
    };

    //Constructor
    SkinPart();
    //Set the current state
    void setState(SkinState s);
    //Update the skin
    void update();
    //Add a state
    void add(SkinState s, Atom props);

    //The active settings
    Atom Active;
  protected:
    //Current state
    SkinState m_state;
    //State atoms
    Atom m_states[S_COUNT];
  private:
  };

  class Skin {
  public:
    //Update
    void update();
    //Set state
    void setState(SkinState s);
  protected:
  private:
  };


	class Control {
	public:
    ///////////////////////////////////////////////////////////////////////////

    Control(uint32 id, Surface *surface);
    virtual ~Control();

    ///////////////////////////////////////////////////////////////////////////
		//Signals

    sigslot::signal0<> Clicked;
    sigslot::signal0<> MouseDown;
    sigslot::signal0<> MouseUp;
    sigslot::signal0<> MouseIn;
    sigslot::signal0<> MouseOut;

    ///////////////////////////////////////////////////////////////////////////

    virtual void update();

    ///////////////////////////////////////////////////////////////////////////
		
		//Return true or false based on the current focus state
		bool focused();
    //Return the id
    uint32 id();

		//If set to true, the user can move the control by dragging
		void canMove(bool flag);
		//If set to true, the user can resize the control by dragging
		void canResize(bool flag);

		//Set the position of the control
		void setPosition(const Vector2s& pos);
		void setPosition(Vector2s pos);
		void setPosition(uint16 x, uint16 y);

		//Set the size of the control
		void setSize(const Vector2s& size);
		void setSize(Vector2s size);
		void setSize(uint16 w, uint16 h);
	protected:
		//Surface
		Surface *m_surface;
    //Input
    Input *m_input;
    //Renderer
    Renderer *m_renderer;

		//The size of the control
		Vector2s m_size;
		//The position of the control
		Vector2s m_position;
    //The world position of the control
    Vector2s m_wposition;

		//Return true/false if the mouse is inside the control
		bool mouseInside();

    //Background color - temporary
    Color m_background;
    Color m_foreground;

	private:
    //ID
    uint32 m_id;
		//Can move?
		bool m_canMove;
		//Can resize?
		bool m_canResize;
    //Are we moving?
    bool m_isMoving;
    //Are we resizing?
    bool m_isResizing;
    //Initial position
    Vector2s m_initalPos;
    //Start move position
    Vector2s m_startMovePos;
    //Stop move position
    Vector2s m_stopMovePos;
    //Mouse was inside last frame?
    bool m_mouseWasInside;


	};

}

#endif
