![Logo](bin/logo.png) ThursUI 
=======

**Straight-forward, pretty, and skinnable C++ UI library for games and 3d applications.**

![Screenshot](screenshot.png)

*This is very much a work in progress. While it works (more or less), it's not very robust, and is very hackish. Also note that there are currently no optimizations done on the code what-so-ever..* 

**Currently supported controls:**
  * Button
  * Progressbar
  * Slider
  * List Box
  * Checkbox
  * Label
  * Dropdown
  * Text input box
  * Rectangle (can be used to display images)
  * Image Grid (useful for e.g. inventories in RPG's)
  * Multi line text field with formatting
  * Skill bar (for RPG's and similar games - supports 1..n clickable icons with optional cooldown indicators)
  * Window

**Noteable features:**
  * JSON-based CSS-style skinning - no texture maps required (textures are of course supported)
  * Skins are animated - each UI state (such as active and hover) can be skinned separately, and Thurs will animate between them when the state changes
  * Designed around signals/slots (powered by sigslot.h)
  * Light on dependencies
  * Liberally licensed (Simplified BSD for now, might switch to zlib) 

As of now, there's only a renderer back-end for [NanoVG](https://github.com/memononen/nanovg). It should be easy to write a custom renderer - take a look in [`include/thurs/renderers/thurs.renderer.nanovg.hpp`](include/thurs/renderers/thurs.renderer.nanovg.hpp) for a place to start. 

Documentation and an actual stable release incoming.

## Building

Build using `cmake`, e.g. running `cmake -DCMAKE_BUILD_TYPE=Release && make` in the project folder.
Run `make install` if you want to copy the files to the respective bin and lib folders on your system.

### Dependencies

Thurs depend on the following libraries:
  * [jsoncpp](https://github.com/open-source-parsers/jsoncpp)

Note that you also need to link with the libraries needed to support the rendering back-end in your application. 
The default output of the makefile is a thin library, so you need to link with jsoncpp in your final application also.

### Installing
Either copy `include/thurs` and the compiled library from `lib` to your preferred paths, or add `include` and `lib` to your path variable or as search paths in your IDE if you're using one. Running `make install` will move things into `/usr/local/include` and `/usr/local/lib`.

## License

[Simplified BSD](LICENSE).
