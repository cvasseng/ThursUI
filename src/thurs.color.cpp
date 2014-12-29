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

#include "../include/thurs/thurs.hpp"
#include "thurs.os.hpp"

namespace thurs {

  Color::Color() {
    r = g = b = a = 255;
    m_tweaining = false;
  }
  
  Color::Color(uint8 _r, uint8 _g, uint8 _b, uint8 _a) {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
    m_tweaining = false;
  }

  Color::Color(uint8 _r, uint8 _g, uint8 _b) {
    r = _r;
    g = _g;
    b = _b;
    a = 255;
    m_tweaining = false;
  }

  Color::Color(uint8 rgb, uint8 _a) {
    r = g = b = rgb;
    a = _a;
    m_tweaining = false;
  }

  Color::Color(uint8 rgb) {
    r = g = b = rgb;
    a = 255;
    m_tweaining = false;
  }

  void Color::tween(Color to, uint16 durationMS) {
    m_tweenStart = getTime();
    m_duration = durationMS;

    m_sr = r;
    m_sg = g;
    m_sb = b;
    m_sa = a;

    m_tr = to.r;
    m_tg = to.g;
    m_tb = to.b;
    m_ta = to.a;

    m_tweaining = true;
  }

  void Color::update() {
    if (m_tweaining) {
      float t = (getTime() - m_tweenStart) / (float)m_duration;

      r = (m_sr + t * (m_tr - m_sr));
      g = (m_sg + t * (m_tg - m_sg));
      b = (m_sb + t * (m_tb - m_sb));
      a = (m_sa + t * (m_ta - m_sa));

      if (t >= 1.f) {
        m_tweaining = false;
        r = m_tr;
        g = m_tg;
        b = m_tb;
        a = m_ta;
        r = m_tr;
      }
    } 
  }

}
