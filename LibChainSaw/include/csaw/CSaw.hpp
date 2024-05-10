#pragma once

#include <string>

namespace csaw
{
    struct ChainSawMessage : std::exception
    {
        ChainSawMessage(const char* file, size_t line, bool canRecover, const std::string& message);

        [[nodiscard]] char const* what() const override;

        const char* File;
        size_t Line;
        bool CanRecover;
        std::string Message;
    };
}

#define CSAW_MESSAGE(_CanRecover, _Message) throw csaw::ChainSawMessage(__FILE__, __LINE__, _CanRecover, _Message)
