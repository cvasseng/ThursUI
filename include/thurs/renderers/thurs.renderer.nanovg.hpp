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

#ifndef h__thurs_renderer_nanovg_
#define h__thurs_renderer_nanovg_

#define NANOVG_GL3_IMPLEMENTATION

#include <nanovg.h>
#include <nanovg_gl.h>
#include <stdio.h>

#include "../thurs.renderer.hpp"

namespace thurs {

  class RendererNanoVG : public Renderer {
  public:
    RendererNanoVG() {
      m_inited = false;
      m_vg = 0;
    }

    //Initialize the renderer
    bool init() {
      m_vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
      
      if (m_vg == NULL) {
        printf("error creating nanovg context\n");
        return false;
      }

      m_font = nvgCreateFont(m_vg, "sans", "Roboto-Regular.ttf");
      nvgCreateFont(m_vg, "sans-bold", "Roboto-Bold.ttf");
      nvgCreateFont(m_vg, "sans-light", "Roboto-Light.ttf");

      m_inited = true;
      return true;
    }

    //Begin rendering
    bool begin(uint16 width, uint16 height, float aspect) {
      if (!m_inited) return false;

      nvgBeginFrame(m_vg, width, height, aspect);
      nvgScale(m_vg, 1.f, 1.f);
      return true;
    }

    //End rendering
    bool end() {
      if (!m_inited) return false;

      nvgEndFrame(m_vg);
      return true;
    }

    //Render text - the implementation should keep track of loaded fonts etc.
    bool renderText(Skin::SkinClass::Attributes &skinClass, const std::string& text, float x, float y, float bx, float by) {
      if (!m_inited) return false;


      static int alignment = 0;
      alignment = 0;

      if (skinClass.hTextAlign == HA_LEFT) {
        alignment |= NVG_ALIGN_LEFT;
      } else if (skinClass.hTextAlign == HA_RIGHT) {
        alignment |= NVG_ALIGN_RIGHT;
        x += bx;
      } else if (skinClass.hTextAlign == HA_CENTER) {
        alignment |= NVG_ALIGN_CENTER;
        x += (bx / 2.f);
      }

      if (skinClass.vTextAlign == VA_TOP) {
        alignment |= NVG_ALIGN_TOP;
      } else if (skinClass.vTextAlign == VA_CENTER) {
        alignment |= NVG_ALIGN_MIDDLE;
        y += (by / 2.f);
      } else if (skinClass.vTextAlign == VA_BOTTOM) {
        alignment |= NVG_ALIGN_BOTTOM;
        y += by;
      } 

      nvgTextAlign(m_vg, alignment);
      
      nvgFontFace(m_vg, skinClass.font.c_str());

      if (skinClass.hasTextStroke) {
        //nvgStrokeWidth(m_vg, skinClass.strokeWidth);
       /* nvgFillColor(m_vg, nvgRGBA(skinClass.stroke.r, skinClass.stroke.g, skinClass.stroke.b, skinClass.stroke.a));
        //nvgFontBlur(m_vg, skinClass.strokeWidth);
        nvgFontSize(m_vg, skinClass.textSize + 2);
        nvgText(m_vg, p.x - 1, p.y - 1, text.c_str(), NULL);
        nvgFontBlur(m_vg, 0.f);*/
       // nvgStroke(m_vg);
      }

      nvgFontSize(m_vg, skinClass.textSize);

      nvgFillColor(m_vg, nvgRGBA(skinClass.textFill.r, skinClass.textFill.g, skinClass.textFill.b, skinClass.textFill.a));
    
      nvgText(m_vg, x, y, text.c_str(), NULL);
      return true;
    }
    
    //Render a rectangle
    bool renderRect(Skin::SkinClass::Attributes &skinClass, float x, float y, float sw, float sh) {
      if (!m_inited) return false;

      NVGcolor col = nvgRGBA(skinClass.fill.r, skinClass.fill.g, skinClass.fill.b, skinClass.fill.a);
      //NVGpaint bg = nvgLinearGradient(m_vg, pos.x, pos.y, pos.x, pos.y + size.y, nvgRGBA(255,255,255,isBlack(col)?16:64), nvgRGBA(0,0,0,isBlack(col)?16:64));

      nvgBeginPath(m_vg);
      nvgRoundedRect(m_vg, x, y, sw, sh, skinClass.cornerRadius);
 
      if (!skinClass.hasImage || skinClass.hasFill) {
        nvgFillColor(m_vg, col);
        nvgFill(m_vg);
      }

      if (skinClass.hasImage) {
        //int w = size.x, h= size.y;
        //nvgImageSize(m_vg, skinClass.imageHandle, &w, &h);
        NVGpaint img = nvgImagePattern(m_vg, x, y, sw, sh, 0.f, skinClass.imageHandle, col.a);
        nvgFillPaint(m_vg, img);
        nvgFill(m_vg);
      } 

      if (skinClass.hasGradient) {
        NVGpaint g = nvgLinearGradient(m_vg, x, y, x, y + sh, 
                          nvgRGBA(skinClass.gradientA.r, skinClass.gradientA.g, skinClass.gradientA.b, skinClass.gradientA.a),
                          nvgRGBA(skinClass.gradientB.r, skinClass.gradientB.g, skinClass.gradientB.b, skinClass.gradientB.a)
                     );
        nvgFillPaint(m_vg, g);
        nvgFill(m_vg);
      }

      if (skinClass.hasStroke) {
        nvgStrokeWidth(m_vg, skinClass.strokeWidth);
        nvgStrokeColor(m_vg, nvgRGBA(skinClass.stroke.r, skinClass.stroke.g, skinClass.stroke.b, skinClass.stroke.a));
        nvgStroke(m_vg);
      }

      return true;
    }

    bool setScissor(const Vector2f& pos, const Vector2f& size) {
      if (!m_inited) return false;

      nvgScissor(m_vg, pos.x, pos.y, size.x, size.y);
      return true;
    }

    //Clear the scissor rect
    bool clearScissor() {
      if (!m_inited) return false;
      nvgResetScissor(m_vg);
      return true;
    }

    uint32 loadImage(const std::string& filename) {
      if (!m_inited) return 0;
      return nvgCreateImage(m_vg, filename.c_str(), 0);
    }

    float getTextHeight(Skin::SkinClass::Attributes &skinClass, const std::string& text) {
      nvgFontSize(m_vg, skinClass.textSize);
      nvgFontFace(m_vg, skinClass.font.c_str());

      float ascender, descender, lineh;
      nvgTextMetrics(m_vg, &ascender, &descender, &lineh);
      return lineh;
    }

    float getTextWidth(Skin::SkinClass::Attributes &skinClass, const std::string& text) {
      nvgFontSize(m_vg, skinClass.textSize);
      return nvgTextBounds(m_vg, 0.f, 0.f, text.c_str(), 0, 0);
    }
  protected:
    //Context
    NVGcontext* m_vg;
    //Inited?
    bool m_inited;

    //Cheat
    uint32 m_font;
  private:
    int isBlack(NVGcolor col) {
      if (col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f ) {
        return 1;
      }
      return 0;
    }
  };

}

#endif
