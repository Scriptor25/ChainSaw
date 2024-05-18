#pragma once

#include <string>

namespace csaw
{
    struct ChainSawMessage : std::exception
    {
        ChainSawMessage(const std::string& source_file, size_t source_line, const std::string& file, size_t line, const std::string& message);

        [[nodiscard]] char const* what() const noexcept override;

        std::string SourceFile;
        size_t SourceLine;

        std::string Filename;
        size_t Line;
        std::string Message;
    };

    void MakeMessage(const std::string& source_file, size_t source_line, const std::string& file, size_t line, bool can_recover, const std::string& message);
}

#define CSAW_MESSAGE_(_CanRecover, _File, _Line, _Message) csaw::MakeMessage(__FILE__, __LINE__, _File, _Line, _CanRecover, _Message)
#define CSAW_MESSAGE_STMT(_CanRecover, _Statement, _Message) csaw::MakeMessage(__FILE__, __LINE__, (_Statement).Filename, (_Statement).Line, _CanRecover, _Message)
