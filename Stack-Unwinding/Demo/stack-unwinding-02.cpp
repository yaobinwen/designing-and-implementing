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
    throw std::runtime_error("MyExcept::_copy(): failed to copy from other");
  }
}

class A
{
public:
  enum DestructorThrows : int
  {
    DESTRUCTOR_NO_THROW = 0,
    DESTRUCTOR_THROWS = 1
  };

public:
  constexpr A(DestructorThrows destruct_throws) noexcept;

  A(A const &) = delete;
  A &operator=(A const &) = delete;

  ~A() noexcept(false);

private:
  DestructorThrows _destruct_throws;
};

constexpr A::A(DestructorThrows destruct_throws) noexcept
    : _destruct_throws(destruct_throws)
{
  std::cout << "Calling A::A()" << std::endl;
}

A::~A() noexcept(false)
{
  std::cout << "Calling A::~A()" << std::endl;
  if (DESTRUCTOR_THROWS == this->_destruct_throws)
  {
    throw std::runtime_error("A::~A(): failed to destruct A");
  }
}

int main(int argc, char *argv[])
{
  try
  {
    A a(A::DESTRUCTOR_THROWS);
  }
  catch (std::runtime_error const &e)
  {
    std::cout << "Handling std::runtime_error thrown from A::~A()" << std::endl;
    std::cout << "Exception is handled." << std::endl;
  }

  try
  {
    A a1(A::DESTRUCTOR_NO_THROW);

    throw MyExcept(MyExcept::COPY_CONSTRUCTOR_NO_THROW);

    A a2(A::DESTRUCTOR_NO_THROW);
  }
  catch (MyExcept const &e)
  {
    std::cout << "Handling exception of type `MyExcept`..." << std::endl;
    std::cout << "Exception is handled." << std::endl;
  }

  try
  {
    A a1(A::DESTRUCTOR_THROWS);

    throw MyExcept(MyExcept::COPY_CONSTRUCTOR_NO_THROW);

    A a2(A::DESTRUCTOR_NO_THROW);
  }
  catch (MyExcept const &e)
  {
    std::cout << "Handling another exception of type `MyExcept`..." << std::endl;
    std::cout << "Exception is handled." << std::endl;
  }

  return 0;
}
