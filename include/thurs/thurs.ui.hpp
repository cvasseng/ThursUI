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

#ifndef h__thursui__ui__
#define h__thursui__ui__

#include "thurs.types.hpp"
#include "thurs.surface.hpp"
#include "thurs.renderer.hpp"
#include "thurs.control.hpp"
#include "thurs.color.hpp"
#include "thurs.skin.hpp"
#include "thurs.window.hpp"

#include "controls/thurs.controls.hpp"

namespace thurs {
  
  class Heirarchy {
  public:
  protected:
  private:  
  };

  class UI {
  public:
    //Constructor
    UI(Renderer * const renderer);
    //Destructor
    virtual ~UI();
    
    //Load main skin 
    bool loadSkin(const std::string& filename);    
    //Include UI definition
    bool include(const std::string& filename);
    
    
    
    //Input handler 
    Input InputHandler;
  protected:
    //Renderer 
    Renderer* m_renderer;
    //Main surface 
    Surface* m_surface;
  private:  
    
  };

}

#endif
