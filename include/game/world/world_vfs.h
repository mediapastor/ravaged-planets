#pragma once

#include <fstream>
#include <memory>
#include <boost/filesystem.hpp>

namespace fw {
class bitmap;
}

namespace game {

/**
 * A "summary" of a single map file. Things like the name, size and so on, useful for displaying
 * in a list.
 */
class world_summary {
private:
  std::string _name;
  mutable bool _extra_loaded;
  mutable std::string _description;
  mutable std::string _author;
  mutable int _width;
  mutable int _height;
  mutable std::shared_ptr<fw::bitmap> _screenshot;
  mutable int _num_players;

  // this is called when you request any of the "extra" info (which is anything other than the name), we load
  // up the data on-demand.
  void ensure_extra_loaded() const;

  // parse the <mapdesc> file and populate our extra stuff.
  void parse_mapdesc_file(boost::filesystem::path const &filename) const;

public:
  world_summary();
  world_summary(world_summary const &copy);
  ~world_summary();

  // initialize our properties from the given map file (or directory)
  void initialize(std::string map_file);

  std::string get_name() const {
    return _name;
  }
  std::string get_description() const {
    ensure_extra_loaded();
    return _description;
  }
  std::string get_author() const {
    ensure_extra_loaded();
    return _author;
  }
  int get_width() const {
    ensure_extra_loaded();
    return _width;
  }
  int get_height() const {
    ensure_extra_loaded();
    return _height;
  }
  std::shared_ptr<fw::bitmap> get_screenshot() const {
    ensure_extra_loaded();
    return _screenshot;
  }
  int get_num_players() const {
    ensure_extra_loaded();
    return _num_players;
  }
};

/**
 * Represents a named entry in the "world file".
 */
class world_file_entry {
private:
  std::string _full_path;
  std::fstream _stream;
  bool _for_write;

  void copy(world_file_entry const &copy);
  void ensure_open(bool throw_on_error = true);

public:
  world_file_entry(std::string full_path, bool for_write);
  world_file_entry(world_file_entry const &copy);
  ~world_file_entry();

  world_file_entry &operator =(world_file_entry const &copy);

  // gets the full path to this file
  std::string get_full_path() const {
    return _full_path;
  }

  // writes num_bytes into the file
  void write(void const *buffer, int num_bytes);

  // writes a line of text, encoded as UTF-8, to the file.
  void write(std::string const &line);

  // reads num_bytes from the file into the buffer
  void read(void *buffer, int num_bytes);

  // returns a value that indicates whether the file exists or not
  bool exists();

  // closes the file, if we have it open
  void close();
};

/**
 * Represents a "world file", which is a collection of \ref world_file_entry objects.
 */
class world_file {
private:
  std::string _path;

public:
  world_file(std::string path);

  world_file_entry get_entry(std::string name, bool for_write);
};

/**
 * This class represents a "virtual file system" containing the world file(s). Basically this just
 * combines the world files in the install directory and the user's personal directory.
 */
class world_vfs {
public:
  world_vfs();
  ~world_vfs();

  // gets a list of all the maps
  std::vector<world_summary> list_maps();

  // opens a new world_file with the complete details of the given map
  world_file open_file(std::string name, bool for_writing = false);
};

}
