#pragma once

#include <string>

namespace csaw
{
    struct ChainSawMessage : std::exception
    {
        ChainSawMessage(const std::string& file, size_t line, const std::string& sourceFile, size_t sourceLine, bool canRecover, const std::string& message);

        [[nodiscard]] char const* what() const override;

        std::string File;
        size_t Line;

        std::string SourceFile;
        size_t SourceLine;

        bool CanRecover;
        std::string Message;
    };
}

#define CSAW_MESSAGE_(_CanRecover, _File, _Line, _Message) throw csaw::ChainSawMessage(__FILE__, __LINE__, _File, _Line, _CanRecover, _Message)
#define CSAW_MESSAGE_STMT(_CanRecover, _Statement, _Message) throw csaw::ChainSawMessage(__FILE__, __LINE__, (_Statement).File, (_Statement).Line, _CanRecover, _Message)
#define CSAW_MESSAGE_NONE(_CanRecover, _Message) throw csaw::ChainSawMessage(__FILE__, __LINE__, "", 0, _CanRecover, _Message)
