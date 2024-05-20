#pragma once

#include <string>

namespace csaw
{
    template <typename T>
    class Expect
    {
    public:
        Expect(T result)
            : m_IsError(false), m_Result(result)
        {
        }

        explicit Expect(const std::string& message)
            : m_Message(message)
        {
        }

        explicit operator bool() const
        {
            return !m_IsError;
        }

        [[nodiscard]] const T& Get() const
        {
            return m_Result;
        }

        [[nodiscard]] std::string Msg() const
        {
            return m_Message;
        }

    private:
        bool m_IsError = true;
        T m_Result;
        std::string m_Message;
    };
}
