#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <csaw/codegen/Builder.hpp>
#include <csaw/lang/Parser.hpp>

int main(int argc, char** argv)
{
    std::string exec = argv[0];
    std::map<std::string, std::string> options;
    std::vector<std::string> flags;
    std::string file;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg.find_first_of("--") == 0)
        {
            const auto opt = arg.substr(2);
            if (const auto pos = opt.find_first_of('='); pos != std::string::npos)
                options[opt.substr(0, pos)] = opt.substr(pos + 1);
            else flags.push_back(opt);

            continue;
        }

        if (!file.empty())
        {
            std::cerr
                    << "Failed to parse args because file name already is set. Use '"
                    << exec
                    << " --help' for more information."
                    << std::endl;
            return 1;
        }

        file = arg;
    }

    if (std::ranges::find(flags, "help") != flags.end())
    {
        std::cout
                << "Usage: " << exec << " [<flag|option>...] <file>" << std::endl
                << "Flags:" << std::endl
                << "\t--help: show this text" << std::endl
                << "Options:" << std::endl
                << "\t--include=<path>...: set include paths as comma seperated list" << std::endl;
        return 0;
    }

    if (file.empty())
    {
        std::cerr << "No input file. Use '" << exec << " --help' for more information." << std::endl;
        return 1;
    }

    std::filesystem::path filepath(file);
    std::ifstream stream(filepath);
    if (!stream.is_open())
    {
        std::cerr << "Failed to open stream from '" << filepath << "'" << std::endl;
        return 1;
    }

    std::vector<std::string> includePaths;
    includePaths.push_back(std::filesystem::absolute(exec).parent_path().string());
    includePaths.push_back(absolute(filepath).parent_path().string());
    auto include = options["include"];
    while (!include.empty())
    {
        const auto pos = include.find_first_of(',');
        if (pos == std::string::npos)
            break;

        includePaths.push_back(include.substr(0, pos));
        include = include.substr(pos + 1);
    }
    if (!include.empty())
        includePaths.push_back(include);

    csaw::Builder builder(filepath.filename().string());
    csaw::Parser::Parse(stream, [&builder](const csaw::StatementPtr& ptr)
    {
        // std::cout << ptr << std::endl;
        builder.Gen(ptr);
    }, includePaths);

    stream.close();

    std::error_code err;
    auto outstream = llvm::raw_fd_ostream("output.ll", err);
    if (err)
    {
        std::cerr << err.message() << std::endl;
        return 1;
    }

    builder.GetModule().print(outstream, nullptr);

    return 0;
}
