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

#ifndef h__thurs__color__
#define h__thurs__color__

#include <string>
#include "thurs.types.hpp"

namespace thurs {

  class Color {
  public:
    uint16 r;
    uint16 g;
    uint16 b;
    uint16 a;

    ///////////////////////////////////////////////////////////////////////////

    Color();
    Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a);
    Color(uint8 _r, uint8 _g, uint8 _b);
    Color(uint8 rgb, uint8 _a);
    Color(uint8 rgb);
    Color(const std::string& rgba);

    void operator=(Color other) {
      r = other.r;
      g = other.g;
      b = other.b;
      a = other.a;
    }

    ///////////////////////////////////////////////////////////////////////////

    //Tween to another color
    void tween(Color to, uint16 durationMS = 150);
    //Update tween
    void update();
  protected:
    bool m_tweaining;
    uint32 m_tweenStart;
    uint32 m_duration;

    uint8 m_sr, m_sg, m_sb, m_sa;
    uint8 m_tr, m_tg, m_tb, m_ta;
  private:
  };

}

#endif
