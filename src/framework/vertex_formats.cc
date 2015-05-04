//
// Copyright (c) 2008-2011, Dean Harding. All rights reserved.
//

#include <stddef.h>
#include <framework/vertex_formats.h>
#include <framework/graphics.h>

namespace fw {
namespace vertex {

#define OFFSET_OF(struct, member) \
  reinterpret_cast<void const *>(offsetof(struct, member))

static void xyz_setup() {
  FW_CHECKED(glVertexPointer(3, GL_FLOAT, sizeof(xyz), OFFSET_OF(xyz, x)));
}

std::function<void()> xyz::get_setup_function() {
  return &xyz_setup;
}

static void xyz_c_setup() {
  FW_CHECKED(glVertexPointer(3, GL_FLOAT, sizeof(xyz_c), OFFSET_OF(xyz_c, x)));
  FW_CHECKED(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(xyz_c), OFFSET_OF(xyz_c, colour)));
}

std::function<void()> xyz_c::get_setup_function() {
  return &xyz_c_setup;
}

static void xyz_uv_setup() {
  FW_CHECKED(glVertexPointer(3, GL_FLOAT, sizeof(xyz_uv), OFFSET_OF(xyz_uv, x)));
  FW_CHECKED(glClientActiveTexture(GL_TEXTURE0));
  FW_CHECKED(glTexCoordPointer(2, GL_FLOAT, sizeof(xyz_uv), OFFSET_OF(xyz_uv, u)));
}

std::function<void()> xyz_uv::get_setup_function() {
  return &xyz_uv_setup;
}

void xyz_c_uv_setup() {
  FW_CHECKED(glVertexPointer(3, GL_FLOAT, sizeof(xyz_c_uv), OFFSET_OF(xyz_c_uv, x)));
  FW_CHECKED(glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(xyz_c_uv), OFFSET_OF(xyz_c_uv, colour)));
  FW_CHECKED(glClientActiveTexture(GL_TEXTURE0));
  FW_CHECKED(glTexCoordPointer(2, GL_FLOAT, sizeof(xyz_c_uv), OFFSET_OF(xyz_c_uv, u)));
}

std::function<void()> xyz_c_uv::get_setup_function() {
  return &xyz_c_uv_setup;
}

void xyz_n_uv_setup() {
  FW_CHECKED(glVertexPointer(3, GL_FLOAT, sizeof(xyz_n_uv), OFFSET_OF(xyz_n_uv, x)));
  FW_CHECKED(glNormalPointer(GL_FLOAT, sizeof(xyz_n_uv), OFFSET_OF(xyz_n_uv, nx)));
  FW_CHECKED(glClientActiveTexture(GL_TEXTURE0));
  FW_CHECKED(glTexCoordPointer(2, GL_FLOAT, sizeof(xyz_n_uv), OFFSET_OF(xyz_n_uv, u)));
}

std::function<void()> xyz_n_uv::get_setup_function() {
  return &xyz_n_uv_setup;
}

void xyz_n_setup() {
  FW_CHECKED(glEnableVertexAttribArray(0));
  FW_CHECKED(glEnableVertexAttribArray(1));
  fw::debug << "glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, " << sizeof(fw::vertex::xyz_n) << ", "
      << OFFSET_OF(xyz_n_uv, x) << ")" << std::endl;
  FW_CHECKED(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(fw::vertex::xyz_n), OFFSET_OF(xyz_n_uv, x)));
  fw::debug << "glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, " << sizeof(fw::vertex::xyz_n) << ", "
      << OFFSET_OF(xyz_n_uv, nx) << ")" << std::endl;
  FW_CHECKED(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(fw::vertex::xyz_n), OFFSET_OF(xyz_n_uv, nx)));
}

std::function<void()> xyz_n::get_setup_function() {
  return &xyz_n_setup;
}

}
}
