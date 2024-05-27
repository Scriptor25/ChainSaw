#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <csaw/ChainSaw.hpp>

#include "csaw/Builder.hpp"
#include "csaw/Parser.hpp"

static const std::string NAME = "csaw";
static const std::string DESCRIPTION = "ChainSaw CLI";

static void show_version()
{
    int major, minor, patch;
    csaw::GetVersion(major, minor, patch);
    std::cout << NAME << " - " << DESCRIPTION << " [version " << major << '.' << minor << '.' << patch << "]" << std::endl;
}

static int show_help()
{
    show_version();
    std::cout << std::endl;
    std::cout << "USAGE" << std::endl;
    std::cout << '\t' << NAME << " [options] file..." << std::endl;
    std::cout << std::endl;
    std::cout << "OPTIONS" << std::endl;
    std::cout << '\t' << "-a <arg>  add <arg> to the jit args" << std::endl;
    std::cout << '\t' << "-e <dir>  emit generated ir to <dir>/<filename>.ll" << std::endl;
    std::cout << '\t' << "-h        display help" << std::endl;
    std::cout << '\t' << "-i <dir>  add <dir> to the include search paths" << std::endl;
    std::cout << '\t' << "-j        run JIT" << std::endl;
    std::cout << '\t' << "-o <file> write output to <file>" << std::endl;
    std::cout << '\t' << "-v        display version" << std::endl;
    return 0;
}

int main(const int argc, const char** argv, const char** env)
{
    std::vector<const char*> jit_args;
    std::vector<std::string> include_dirs;
    std::vector<std::string> input_files;
    bool run_jit = false;
    std::string output_file;
    std::string emit_ir_directory;

    jit_args.push_back(argv[0]);

    for (size_t i = 1; i < argc; ++i)
    {
        if (const std::string arg_str = argv[i]; arg_str == "-a")
        {
            jit_args.push_back(argv[++i]);
        }
        else if (arg_str == "-e")
        {
            emit_ir_directory = argv[++i];
        }
        else if (arg_str == "-h")
        {
            return show_help();
        }
        else if (arg_str == "-i")
        {
            include_dirs.emplace_back(argv[++i]);
        }
        else if (arg_str == "-j")
        {
            run_jit = true;
        }
        else if (arg_str == "-o")
        {
            output_file = argv[++i];
        }
        else if (arg_str == "-v")
        {
            show_version();
        }
        else input_files.push_back(arg_str);
    }

    if (input_files.empty())
        return show_help();

    csaw::Builder builder;
    csaw::ParseCallback callback = [&builder](const csaw::StatementPtr& ptr) { builder.Gen(ptr); };

    int error = 0;

    for (const auto& file : input_files)
    {
        const auto module_name = std::filesystem::path(file).filename().string();
        builder.BeginModule(module_name, file);

        std::ifstream stream(file);
        std::vector<std::filesystem::path> processed;
        csaw::ParseData data(file, stream, callback, include_dirs, processed);
        error |= csaw::Parser::Parse(data);

        error |= builder.EndModule(emit_ir_directory);
    }

    /*if (!error)
        error |= builder.LinkModules();
    else std::cout << "Skip linking modules because of error" << std::endl;*/

    if (!output_file.empty())
    {
        if (!error)
            error |= builder.OutputModules(output_file, llvm::CGFT_ObjectFile);
        else std::cout << "Skip output because of error" << std::endl;
    }

    if (run_jit)
    {
        if (!error)
            error |= builder.RunJIT(static_cast<int>(jit_args.size()), jit_args.data(), env);
        else std::cout << "Skip JIT because of error" << std::endl;
    }

    return error;
}
