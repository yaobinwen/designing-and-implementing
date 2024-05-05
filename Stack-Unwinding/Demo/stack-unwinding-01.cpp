#include <iostream>
#include <stdexcept>

class MyExcept
{
public:
  enum CopyThrows : int
  {
    COPY_CONSTRUCTOR_NO_THROW = 0,
    COPY_CONSTRUCTOR_THROWS = 1
  };

public:
  // Default constructor.
  constexpr MyExcept(CopyThrows copy_throws) noexcept;

  // Copy constructor that may throw exceptions.
  MyExcept(MyExcept const &other) noexcept(false);

  // Copy assignment operator that may throw exceptions.
  MyExcept &operator=(MyExcept const &other) noexcept(false);

private:
  void _copy(MyExcept const &other) noexcept(false);

private:
  CopyThrows _copy_throws;
};

constexpr MyExcept::MyExcept(CopyThrows copy_throws) noexcept
    : _copy_throws(copy_throws)
{
  std::cout << "Calling MyExcept::MyExcept()" << std::endl;
}

MyExcept::MyExcept(MyExcept const &other) noexcept(false)
{
  std::cout << "Calling MyExcept::MyExcept(MyExcept const &)" << std::endl;

  this->_copy(other);
}

MyExcept &MyExcept::operator=(MyExcept const &other) noexcept(false)
{
  std::cout << "Calling MyExcept::operator=(MyExcept const &)" << std::endl;

  this->_copy(other);

  return *this;
}

void MyExcept::_copy(MyExcept const &other) noexcept(false)
{
  this->_copy_throws = other._copy_throws;

  if (COPY_CONSTRUCTOR_THROWS == this->_copy_throws)
  {
    std::cout << "MyExcept::_copy: throwing an std::runtime_error..." << std::endl;
    throw std::runtime_error("MyExcept: failed to copy from other");
  }
}

class A
{
public:
  constexpr A() noexcept
  {
    std::cout << "Calling A::A()" << std::endl;
  }

  A(A const &) = delete;
  A &operator=(A const &) = delete;

  ~A() noexcept
  {
    std::cout << "Calling A::~A()" << std::endl;
  }
};

int main(int argc, char *argv[])
{
  try
  {
    A a1;

    throw MyExcept(MyExcept::COPY_CONSTRUCTOR_NO_THROW);

    A a2;
  }
  catch (MyExcept e)
  {
    std::cout << "Handling exception of type `MyExcept`..." << std::endl;
    std::cout << "Exception is handled." << std::endl;
  }

  try
  {
    A a1;

    throw MyExcept(MyExcept::COPY_CONSTRUCTOR_THROWS);

    A a2;
  }
  catch (MyExcept const &e)
  {
    std::cout << "Handling exception of type `MyExcept`..." << std::endl;
    std::cout << "Exception is handled." << std::endl;
  }

  try
  {
    A a1;

    throw MyExcept(MyExcept::COPY_CONSTRUCTOR_THROWS);

    A a2;
  }
  catch (...)
  {
    std::cout << "Handling exception of general type..." << std::endl;
    std::cout << "Exception is handled." << std::endl;
  }

  try
  {
    A a1;

    throw MyExcept(MyExcept::COPY_CONSTRUCTOR_THROWS);

    A a2;
  }
  catch (MyExcept e)
  {
    std::cout << "Handling another exception of type `MyExcept`..." << std::endl;
    std::cout << "Exception is handled." << std::endl;
  }

  return 0;
}
