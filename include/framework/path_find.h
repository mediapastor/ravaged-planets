#pragma once

#include <framework/vector.h>

namespace fw {
struct path_node;

/** Class for finding a path between point "A" and point "B" in a grid. */
class path_find {
private:
  int _width;
  int _length;
  path_node *_nodes;
  int _run_no;

  path_node *get_node(fw::vector const &loc) const;
  bool is_passable(fw::vector const &start, fw::vector const &end) const;

public:
  path_find(int width, int length, std::vector<bool> const &passability);
  virtual ~path_find();

  /**
   * Finds a path between the given 'start' and 'end' vectors. We ignore the y component of the vectors and just look
   * at the (x,y) components. The 'path' is populated with the path we found and we'll assume the agent will travel
   * between it's current location and the first element of path, then to the second element and so on traveling in a
   * straight line each time.
   *
   * Returns true if a path was found, false if no path exists
   */
  virtual bool find(std::vector<fw::vector> &path, fw::vector const &start, fw::vector const &end);

  /**
   * Simplifies the given path by removing any unneeded nodes. For example, if you have an "L" shaped path where a
   * straight line will do, this will turn the path into a straight line. Also, if you have lots of nodes in a straight
   * line, this will remove all but the first & last, etc.
   *
   * Note: the path in full_path is not modified, but the simplified path is built up in new_path.
   */
  virtual void simplify_path(std::vector<fw::vector> const &full_path, std::vector<fw::vector> &new_path);
};

/**
 * This specialization of path_find adds some timing methods and it also keeps the final node structure around
 * for visualization
 */
class timed_path_find: public path_find {
public:
  timed_path_find(int width, int length, std::vector<bool> &passability);
  virtual ~timed_path_find();

  // the total time the last find() call took, in seconds.
  float total_time;

  virtual bool find(std::vector<fw::vector> &path, fw::vector const &start, fw::vector const &end);
};

}
