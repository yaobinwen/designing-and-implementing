#pragma once

#include <stdexcept>
#include <string>

namespace ywen
{

class file_error_base : public std::runtime_error
{
public:
  /// Throws:
  /// - std::bad_alloc
  file_error_base(std::string const & fpath, int err_no) noexcept
    : std::runtime_error(""), m_fpath(fpath), m_err_no(err_no)
  {
    // Empty
  }

  // TODO(ywen): What about copy/move constructor/assignment/?
  // TODO(ywen): What about destructor?

  std::string
  fpath() const noexcept
  {
    // TODO(ywen): Does std::string copy assignment throw?
    return m_fpath;
  }

  int
  err_no() const noexcept
  {
    return m_err_no;
  }

private:
  std::string m_fpath;
  int m_err_no;
};

class file_open_error : public file_error_base
{
public:
  /// Throws:
  /// - std::bad_alloc:
  file_open_error(std::string const & fpath, int err_no) noexcept
    : file_error_base(fpath, err_no)
  {
    // Empty
  }
};

class file_close_error : public file_error_base
{
public:
  /// Throws:
  /// - std::bad_alloc:
  file_close_error(
    std::string const & fpath,
    int fclose_ret,
    int err_no) noexcept
    : file_error_base(fpath, err_no), m_fclose_ret(fclose_ret)
  {
    // Empty
  }

  int
  fclose_ret() const noexcept
  {
    return m_fclose_ret;
  }

private:
  int m_fclose_ret;
};

}  // namespace ywen
