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

#ifndef h__thurs__input__
#define h__thurs__input__

#include "thurs.types.hpp"

namespace thurs {

	class Input {
	public:
		///////////////////////////////////////////////////////////////////////////
		
		//Mouse button states
		enum MouseButton {
			MB_NONE,
			MB_LEFT,
			MB_RIGHT,
			MB_MIDDLE,
		};

		///////////////////////////////////////////////////////////////////////////

		Input();

		///////////////////////////////////////////////////////////////////////////
		//INJECTIONS

		//Inject mouse coordinates
		void injectMouseCoords(uint16 x, uint16 y);
		//Inject mouse button state
		void injectMouseButton(MouseButton b);

		///////////////////////////////////////////////////////////////////////////

		//Get the current mouse position
		const Vector2s& mouseCoords();
		//Get the current mouse button
		const MouseButton& mouseButton();
    //Returns true if there was a mouse click
    bool mouseClick(MouseButton btn = MB_LEFT);
    //Returns true if the given mouse button was just pressed
    bool mouseDown(MouseButton btn = MB_LEFT);
    //Returns true if the given mouse button was just released
    bool mouseUp(MouseButton btn = MB_LEFT);

    //Mark input as handled 
    void markHandled();
    //Is the input handled? - can be used by the game engine to cancel
    //e.g. character movement etc.
    bool handled();

	protected:
		//Current mouse coords
		Vector2s m_coords;
		//Current mouse button state
		MouseButton m_mbutton;
    //Last state of mouse button
    MouseButton m_mbuttonl;
    //Handled?
    bool m_handled;
	private:
	};

}

#endif
