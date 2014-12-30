ThursUI
=======

![Logo](bin/logo.png)

**Straight-forward, lightweight, pretty, skinnable, immediate-mode UI library.**

This is very much a work in progress. Widgets/controls are added on a "I need a xxx right now"-basis as I continue development of my game and engine. 

Currently supports the following controls:
  * Button
  * Progressbar

Currently there's only a renderer back-end for [NanoVG](https://github.com/memononen/nanovg). Should be easy to write a custom renderer - take a look in [`include/thurs/renderers/thurs.renderer.nanovg.hpp`](include/renderers/thurs/thurs.renderer.nanovg.hpp) for a place to start.

<insert gif here>

## Hello world

## Motivation
So why another UI library, you might ask? Well, when I finally reached the point where I wanted a UI in my game, I tried the usual suspects and had major troubles getting everything to compile and work on my box. If I can't get a library working within an hour of downloading it, I cross it of the list and look for an alternative. A lot of the libraries are largely undocumented. A lot of them are old and unmaintained. I wanted something a bit more "modern". Something that allowed me, as a programmer, to create something that looks OK. That's just not gonna happen with a library that still relies on texture maps for skinning. Hopefully it will happen with this library. So there you have it.

## Building

I've only built on OS X so far, but the code should work fine on *NX and Windows. If compiling for windows, you're gonna have to create a VS project file to compile it, or use MinGW. The included makefile might work for Linux, but I have yet to test it. Will use a cross-platform build system eventually.

### Dependencies

Thurs depend on the following libraries to compile:
  * libjson

Note that you also need to link with the libraries needed to support the rendering back-end in your application.

### Compiling


### Installing
Either copy `include/thurs` and the compiled library from `lib` to your preferred paths, or add `include` and `lib` to your path variable or as search paths in your IDE if you're using one. Running `make install` will move things into `/usr/local/include` and `/usr/local/lib`.

## Documentation
  
  * Getting started: installing, compiling, and running samples
  * Skinning

## Planned features


## License

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