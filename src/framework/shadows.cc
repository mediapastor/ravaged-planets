#include <boost/foreach.hpp>

#include <framework/shadows.h>
#include <framework/shader.h>
#include <framework/graphics.h>
#include <framework/texture.h>
#include <framework/framework.h>
#include <framework/timer.h>

namespace fw {

light_camera::light_camera() {
  //set_projection_matrix(cml::constantsf::pi() / 8.0f, 1.0f, 100.0f, 500.0f);
  set_projection_matrix(cml::constantsf::pi() / 8.0f, 1.0f, 200.0f, 500.0f);
}

light_camera::~light_camera() {
}

//---------------------------------------------------------------------------------------------------------
// this is a static list of shadow map textures, so we don't have create/destroy them
// over and over (as shadow_sources get created/destroyed)
static std::list<std::shared_ptr<texture>> g_shadow_maps;

shadow_source::shadow_source() :
    _shadowmap(0), _real_camera(0) {
}

shadow_source::~shadow_source() {
  if (_shadowmap) {
    g_shadow_maps.push_front(_shadowmap);
  }
}

void shadow_source::initialize() {
  if (g_shadow_maps.empty()) {
    _shadowmap = std::shared_ptr<texture>(new texture());
    _shadowmap->create(1024, 1024, true);
  } else {
    _shadowmap = g_shadow_maps.front();
    g_shadow_maps.pop_front();
  }
}

void shadow_source::begin_scene() {
  framework *frmwrk = fw::framework::get_instance();

  // set the camera to our light's camera, and update it
  _real_camera = framework::get_instance()->get_camera();
  frmwrk->set_camera(&_camera);
  _camera.update(framework::get_instance()->get_timer()->get_frame_time());

  frmwrk->get_graphics()->set_render_target(_shadowmap.get());

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE) {
    fw::debug << "Framebuffer is not complete, expect errors: " << status << std::endl;
  }
}

void shadow_source::end_scene() {
  framework *frmwrk = fw::framework::get_instance();

  // reset the render target and camera back to the "real" one
  frmwrk->get_graphics()->set_render_target(nullptr);
  frmwrk->set_camera(_real_camera);
}

}