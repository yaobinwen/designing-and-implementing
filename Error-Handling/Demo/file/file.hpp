#pragma once

#include <cstdio>
#include <string>

namespace ywen
{

class file
{
public:
  static file
  open(std::string const & fpath);

  file();

  file(std::string const & fpath);

  ~file();

  void
  open_read();

  void
  open_write();

  void
  open_append();

  /// Throws:
  /// - file_close_error:
  void
  close();

  bool
  is_open() const noexcept;

private:
  std::string m_fpath;
  std::FILE * m_file;
};

}  // namespace ywen
