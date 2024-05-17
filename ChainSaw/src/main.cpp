#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <csaw/codegen/Builder.hpp>
#include <csaw/lang/Parser.hpp>

static const std::string EXE_NAME = "csaw";
static const std::string DESC_SHORT = "ChainSaw CLI";
static const std::string VERSION_STR = "1.0";

static int exit_cli(const bool pause_on_exit, const int code)
{
    if (pause_on_exit)
    {
        std::cout << "Press enter to exit" << std::endl;
        std::cin.get();
    }

    return code;
}

static int help()
{
    std::cout << EXE_NAME << " - " << DESC_SHORT << " [version " << VERSION_STR << "]" << std::endl;
    std::cout << std::endl;
    std::cout << "USAGE" << std::endl;
    std::cout << '\t' << EXE_NAME << " [options] file... [-- jit args]" << std::endl;
    std::cout << std::endl;
    std::cout << "OPTIONS" << std::endl;
    std::cout << '\t' << "-c, --code:            emit the parsed code to the standard output" << std::endl;
    std::cout << '\t' << "-e, --entry:           specify the jit entry function name; defaults to main" << std::endl;
    std::cout << '\t' << "-h, --help:            show this help message" << std::endl;
    std::cout << '\t' << "-i, --include:         add a directory to the include search paths" << std::endl;
    std::cout << '\t' << "-j, --jit, -r, --run:  run the compiled program through a jit using the provided program args" << std::endl;
    std::cout << '\t' << "-o, --output:          specify the output file; no output file by default" << std::endl;
    std::cout << '\t' << "-p, --pause:           wait for enter after execution" << std::endl;
    std::cout << '\t' << "-q, --quiet:           do not output any compiler messages like errors and warnings (TODO)" << std::endl;
    std::cout << '\t' << "-s, --skip:            skip code generation, only parse files" << std::endl;
    std::cout << '\t' << "-t, --type:            specify the output file type; requires an output file to be set" << std::endl;
    std::cout << '\t' << "-v, --version, --info: show the program version info" << std::endl;
    std::cout << std::endl;
    std::cout << "To ship a value with an option, e.g for '-o', you just append the value, e.g '-ofilename.ext'. For '--output', use '--output=filename.ext'." << std::endl;
    std::cout << std::endl;

    return 0;
}

static void version()
{
    std::cout << EXE_NAME << " - " << DESC_SHORT << " [version " << VERSION_STR << "]" << std::endl;
}

static bool eat_flag(std::vector<std::string>& args, const std::vector<std::string>& flags)
{
    for (const auto& flag : flags)
        if (auto pos = std::ranges::find(args, flag); pos != args.end())
        {
            args.erase(pos);
            return true;
        }
    return false;
}

static bool eat_option(std::vector<std::string>& args, const std::vector<std::string>& flags, std::string& option)
{
    for (size_t i = 0; i < args.size(); ++i)
    {
        const auto& arg = args[i];
        for (const auto& flag : flags)
        {
            if (arg.starts_with(flag))
            {
                option = arg.substr(flag.length());
                args.erase(args.begin() + i);
                return true;
            }
        }
    }

    return false;
}

int main(const int argc, const char** argv)
{
    std::string executable = argv[0];

    bool emit_code;
    bool run_jit;
    bool pause_on_exit;
    bool quiet;
    bool skip_codegen;

    std::vector<std::string> include_paths;
    std::string output_file;
    std::string output_file_type;
    std::string entry = "main";

    std::vector<std::string> files;
    std::vector<const char*> jit_args;

    std::vector<std::string> args;
    size_t i;
    for (i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--")
        {
            ++i;
            break;
        }
        args.emplace_back(arg);
    }
    for (; i < argc; ++i)
        jit_args.push_back(argv[i]);

    if (args.empty() || eat_flag(args, {"-h", "--help"}))
        return help();

    emit_code = eat_flag(args, {"-c", "--code"});
    eat_option(args, {"-e", "--entry="}, entry);
    for (std::string name; eat_option(args, {"-i", "--include="}, name);) include_paths.push_back(name);
    run_jit = eat_flag(args, {"-j", "--jit", "-r", "--run"});
    eat_option(args, {"-o", "--output="}, output_file);
    pause_on_exit = eat_flag(args, {"-p", "--pause"});
    quiet = eat_flag(args, {"-q", "--quiet"});
    skip_codegen = eat_flag(args, {"-s", "--skip"});
    eat_option(args, {"-t", "--type="}, output_file_type);
    if (eat_flag(args, {"-v", "--version", "--info"})) version();

    for (const auto& arg : args)
        files.push_back(arg);
    args.clear();

    if (output_file.empty() && !output_file_type.empty())
    {
        std::cout << "Cannot use '--type' without providing an output file via '--output'. See help for more information." << std::endl;
        return exit_cli(pause_on_exit, -1);
    }

    if (files.empty())
    {
        std::cout << "Please provide at least one file to compile. See help for more information." << std::endl;
        return exit_cli(pause_on_exit, -1);
    }

    if (skip_codegen && run_jit)
    {
        std::cout << "Option '--skip' is not compatible with '--jit'. See help for more information." << std::endl;
        return exit_cli(pause_on_exit, -1);
    }

    csaw::Builder* builder = nullptr;
    if (!skip_codegen)
    {
        builder = csaw::Builder::Create();
        if (!builder)
            return exit_cli(pause_on_exit, -1);
    }

    auto callback = [emit_code, builder](const csaw::StatementPtr& ptr)
    {
        if (emit_code) std::cout << ptr << std::endl;
        if (builder) builder->Gen(ptr);
    };

    for (const auto& file : files)
    {
        std::ifstream stream(file);
        if (!stream)
        {
            std::cout << "Failed to open file '" << file << "'" << std::endl;
            continue;
        }

        auto name = std::filesystem::path(file).filename().string();
        name = name.substr(0, name.find_last_of('.'));
        if (builder && builder->BeginModule(name, file))
        {
            std::cout << "Failed to begin new module '" << name << "'" << std::endl;
            continue;
        }

        std::vector<std::filesystem::path> processed;
        csaw::ParseData data(file, stream, callback, include_paths, processed);
        csaw::Parser::Parse(data);

        if (builder && builder->EndModule())
        {
            std::cout << "Failed to end module '" << name << "'" << std::endl;
            continue;
        }
    }

    if (builder && run_jit)
    {
        jit_args.insert(jit_args.begin(), executable.c_str());
        auto code = builder->RunJIT(entry, static_cast<int>(jit_args.size()), jit_args.data());
        std::cout << "Exit Code " << code << std::endl;
        return exit_cli(pause_on_exit, code);
    }

    return exit_cli(pause_on_exit, 0);
}
