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

#ifndef h__thurs__renderer__
#define h__thurs__renderer__

#include <vector>
#include <string>

#include "thurs.types.hpp"
#include "thurs.color.hpp"
#include "thurs.skin.hpp"

namespace thurs {

	class Surface;

	class Renderer {
		friend class Surface;
	public:

    virtual ~Renderer() {}

		///////////////////////////////////////////////////////////////////////////

    //Begin rendering
    virtual bool begin(uint16 width, uint16 height, float aspect) = 0;
    //End rendering
    virtual bool end() = 0;

    //Initialize the renderer
    virtual bool init() = 0;
		//Render text - the implementation should keep track of loaded fonts etc.
		virtual bool renderText(Skin::SkinClass::Attributes &skinClass, const std::string& text, const Vector2f& pos) = 0;
    //Render a rectangle
    virtual bool renderRect(Skin::SkinClass::Attributes &skinClass, const Vector2f& pos, const Vector2f& size) = 0;
    //Set the scissor rect
    virtual bool setScissor(const Vector2f& pos, const Vector2f& size) = 0;
    //Clear the scissor rect
    virtual bool clearScissor() = 0;

    //Get the length of a string in pixels
   // virtual float getTextWidth(uint32 font, const std::string& text);


		///////////////////////////////////////////////////////////////////////////

	protected:
		//Surfaces registered to the renderer - we keep track of them here
		//so that we can render all surfaces in a single call if needed.
		std::vector<Surface*> m_surfaces;
	private:
	};

}

#endif