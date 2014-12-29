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

namespace thurs {

  class Control;

	//A UI Surface
	/*
		All controls live inside a surface.
	*/
	class Surface {
    friend class Control;
	public:
    typedef std::map<uint16, Control*> ControlMap;
    typedef ControlMap::iterator ControlMapIt;
    #define ControlMapPair(x, y) std::pair<uint16, Control*>(x, y)

    ///////////////////////////////////////////////////////////////////////////

		Surface(Renderer *renderer, Input *input);

    //Update the surface
    void updateAndRender();

		//Returns the attached input handler
		Input* const input();
    //Returns the attached renderer
    Renderer* const renderer();

	protected:
		//Our renderer 
		Renderer *m_renderer;
		//Input injector
		Input *m_input;
    //Controls in the surface
    ControlMap m_controls;
    //The size of the surface
    Vector2s m_size;
	private:
	};

}

#endif
