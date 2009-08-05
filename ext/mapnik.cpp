// Include the Ruby headers and goodies
#include "ruby.h"

// define before any includes
#define BOOST_SPIRIT_THREADSAFE

#include <mapnik/map.hpp>
#include <mapnik/datasource_cache.hpp>
#include <mapnik/font_engine_freetype.hpp>
#include <mapnik/agg_renderer.hpp>
#include <mapnik/filter_factory.hpp>
#include <mapnik/color_factory.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/config_error.hpp>
#include <mapnik/load_map.hpp>
#include <iostream>

extern "C" {
  using namespace mapnik;

  // Defining a space for information and references about the module to be stored internally
  VALUE Mapnik = Qnil;
  VALUE TileRenderer = Qnil;

  // Prototype for the initialization method - Ruby calls this, not you
  void Init_mapnik();

  VALUE method_render_tile(VALUE self, VALUE tile);

  // The initialization method for this module
  void Init_mapnik() {
    Mapnik = rb_define_module("Mapnik");
    TileRenderer = rb_define_class_under(Mapnik, "TileRenderer", rb_cObject);
    rb_define_method(TileRenderer, "render_tile", (VALUE(*)(...))method_render_tile, 1);
  }

  // Our 'test1' method.. it simply returns a value of '10' for now.
  VALUE method_render_tile(VALUE self, VALUE tile) 
  {
    datasource_cache::instance()->register_datasources("/usr/local/lib/mapnik/input");

    VALUE layer = rb_funcall(tile, rb_intern("layer"), 0);

    VALUE raw_mapfile = rb_funcall(layer, rb_intern("mapfile"), 0);
    const char *mapfile = StringValuePtr(raw_mapfile);

    VALUE raw_size = rb_funcall(tile, rb_intern("size"), 0);

    Map map(FIX2INT(RARRAY(raw_size)->ptr[0]), FIX2INT(RARRAY(raw_size)->ptr[1]));

    load_map(map, mapfile);
  
    VALUE raw_bbox = rb_funcall(tile, rb_intern("bounding_box"), 0);

    map.zoomToBox(Envelope<double>(
          NUM2DBL(rb_funcall(raw_bbox, rb_intern("min_x"), 0)),
          NUM2DBL(rb_funcall(raw_bbox, rb_intern("min_y"), 0)),
          NUM2DBL(rb_funcall(raw_bbox, rb_intern("max_x"), 0)),
          NUM2DBL(rb_funcall(raw_bbox, rb_intern("max_y"), 0))));

    Image32 image(map.getWidth(), map.getHeight());
    agg_renderer<Image32> ren(map, image);
    ren.apply();

    // save_to_file<ImageData32>(buf.data(),"hello.png","png");
    std::string s = save_to_string(image, "png");
    return rb_str_new(s.data(), s.size());
    // return rb_str_new2((const char*)image.raw_data());
  }
}

