
#include <functional>

#include <framework/framework.h>
#include <framework/gui/builder.h>
#include <framework/gui/gui.h>
#include <framework/gui/window.h>
#include <framework/gui/button.h>
#include <framework/gui/static_widget.h>
#include <game/application.h>
#include <game/screens/screen.h>
#include <game/world/terrain.h>

#include <game/editor/windows/new_map.h>
#include <game/editor/windows/message_box.h>
#include <game/editor/editor_screen.h>

namespace ed {

using namespace fw::gui;
using namespace std::placeholders;

new_map_window *new_map = nullptr;

new_map_window::new_map_window() : _wnd(nullptr) {
}

new_map_window::~new_map_window() {
}

void new_map_window::initialize() {
  _wnd = builder<window>(sum(pct(50), px(-100)), sum(pct(50), px(-100)), px(200), px(100))
          << window::background("frame") << widget::visible(false)
      << (builder<static_widget>(px(10), px(10), sum(pct(100), px(-20)), px(18)) << static_widget::text("Size:"))
      << (builder<static_widget>(sum(pct(50), px(-8)), px(30), px(16), px(20)) << static_widget::text("x"))
      << (builder<button>(sum(pct(100), px(-180)), sum(pct(100), px(-28)), px(80), px(20)) << button::text("Create")
          << widget::click(std::bind(&new_map_window::ok_clicked, this, _1)))
      << (builder<button>(sum(pct(100), px(-90)), sum(pct(100), px(-28)), px(80), px(20)) << button::text("Cancel")
  << widget::click(std::bind(&new_map_window::cancel_clicked, this, _1)));
  fw::framework::get_instance()->get_gui()->attach_widget(_wnd);
}

void new_map_window::show() {
  _wnd->set_visible(true);
}

void new_map_window::hide() {
  _wnd->set_visible(false);
}

bool new_map_window::ok_clicked(widget *w) {
  _wnd->set_visible(false);

  int width = 4;
  int height = 4;
//  try {
//    width = boost::lexical_cast<int>(get_child("NewMap/Width")->getText().c_str());
//    height = boost::lexical_cast<int>(get_child("NewMap/Height")->getText().c_str());
//  } catch (boost::bad_lexical_cast &) {
//    message_box->show("Invalid Parameters", "Width and Height must be an integer.");
//    return true;
//  }

  editor_screen::get_instance()->new_map(width * rp::terrain::PATCH_SIZE, width * rp::terrain::PATCH_SIZE);
  return true;
}

bool new_map_window::cancel_clicked(widget *w) {
  _wnd->set_visible(false);
  return true;
}

}