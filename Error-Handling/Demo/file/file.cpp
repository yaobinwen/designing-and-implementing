#include <cerrno>

#include "exception.hpp"
#include "file.hpp"

namespace ywen
{

file::file() : m_file(nullptr)
{
  // Empty
}

file::file(std::string const & fpath)
{
  // TODO(ywen): Implement me!
}

file::~file()
{
  if (m_file)
  {
    this->close();
  }
}

void
file::open_read()
{
  // Eventually, POSIX `open` is called. Its page details the possible errors:
  // https://pubs.opengroup.org/onlinepubs/9699919799/functions/open.html
  // https://manpages.ubuntu.com/manpages/trusty/man2/open.2.html
  //
  // TODO(ywen): I need to study the behavior of POSIX `open` to understand
  // how it may fail in each case, and then implement the corresponding
  // exception.
  std::FILE * fp = std::fopen(m_fpath.data(), "r");
  if (nullptr == fp)
  {
    throw file_open_error(m_fpath, errno);
  }
}

void
file::open_write()
{
}

void
file::open_append()
{
}

void
file::close()
{
  int ret = std::fclose(m_file);

  // Per [1], "Whether or not the operation succeeds, the stream is no longer
  // associated with a file," so we set `m_file` to `nullptr` to indicate the
  // file has been closed and should not be used anymore.
  // [1]: https://en.cppreference.com/w/c/io/fclose
  m_file = nullptr;

  if (EOF == ret)
  {
    // If the closure failed, we throw an exception to indicate the error.
    throw file_close_error(m_fpath, ret, errno);
  }
}

bool
file::is_open() const noexcept
{
  return (nullptr != m_file);
}

}  // namespace ywen
