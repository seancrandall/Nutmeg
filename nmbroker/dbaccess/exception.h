#ifndef NUTMEG_EXCEPTION_H
#define NUTMEG_EXCEPTION_H

#include <exception>
#include <string>

namespace Nutmeg
{

class Exception : public std::exception
{
  public:
    explicit Exception(const std::string &message) : m_message(message) {}

    virtual ~Exception() noexcept {}

    // Returns the error message
    virtual const char *what() const noexcept override { return m_message.c_str(); }

  private:
    std::string m_message;
};

class DatabaseReadError : public Exception
{
  public:
    explicit DatabaseReadError(const std::string &message) : Exception("Database Read Error: " + message) {}
};

class DatabaseWriteError : public Exception
{
  public:
    explicit DatabaseWriteError(const std::string &message) : Exception("Database Write Error: " + message) {}
};

class InputFailedCheck : public Exception
{
  public:
    explicit InputFailedCheck(const std::string &message) : Exception("Input Failed Check: " + message) {}
};

class DatabaseCallError : public Exception
{
  public:
    explicit DatabaseCallError(const std::string &message)
        : Exception("A stored procedure call to the database failed: " + message)
    {
    }
};
} // namespace Nutmeg

#endif // NUTMEG_EXCEPTION_H
