#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <csaw/ChainSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/lang/Parser.hpp>

static const std::string NAME = "csaw";
static const std::string DESCRIPTION = "ChainSaw CLI";

static int exit_cli(const bool pause_on_exit, const int code)
{
    if (pause_on_exit)
    {
        std::cout << "Press enter to exit" << std::endl;
        std::cin.get();
    }

    return code;
}

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
    std::cout << '\t' << NAME << " [options] file... [-- jit args]" << std::endl;
    std::cout << std::endl;
    std::cout << "OPTIONS" << std::endl;
    std::cout << '\t' << "-b, --build:           specify the directoy for the build/output files" << std::endl;
    std::cout << '\t' << "-c, --code:            emit the parsed code to the standard output" << std::endl;
    std::cout << '\t' << "-e, --entry:           specify the jit entry function name; defaults to main" << std::endl;
    std::cout << '\t' << "-h, --help:            show this help message" << std::endl;
    std::cout << '\t' << "-i, --include:         add a directory to the include search paths" << std::endl;
    std::cout << '\t' << "-j, --jit:             run the compiled program through a jit using the provided program args" << std::endl;
    std::cout << '\t' << "-l, --link:            if the binary output files should be linked into a executable, specify the filename" << std::endl;
    std::cout << '\t' << "-L, --lib:             add libraries to be linked against" << std::endl;
    std::cout << '\t' << "-o, --output:          specify the type of the output files; possible values are 'll' (llvm ir), 'obj' (binary) and 'asm' (assembly), default is 'obj'" << std::endl;
    std::cout << '\t' << "-p, --pause:           wait for enter after execution" << std::endl;
    std::cout << '\t' << "-r, --run:             alias for -j, --jit" << std::endl;
    std::cout << '\t' << "-v, --version:         show the program version info" << std::endl;
    std::cout << std::endl;
    std::cout << "For options with values, use either '-oValue' or '--option=Value'." << std::endl;

    return 0;
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

    if (args.empty())
        return show_help();

    std::string build;
    std::string entry = "main";
    std::vector<std::string> includes;
    std::string link;
    std::vector<std::string> libs;
    std::string output = "obj";

    eat_option(args, {"-b", "--build="}, build); // -b, --build
    bool code = eat_flag(args, {"-c", "--code"}); // -c, --code
    eat_option(args, {"-e", "--entry="}, entry); // -e, --entry
    bool help = eat_flag(args, {"-h", "--help"}); // -h, --help
    for (std::string include; eat_option(args, {"-i", "--include="}, include);) // -i, --include
        includes.push_back(include);
    bool jit = eat_flag(args, {"-j", "--jit", "-r", "--run"}); // -j, --jit, -r, --run
    eat_option(args, {"-l", "--link="}, link); // -l, --link
    for (std::string lib; eat_option(args, {"-L", "--lib="}, lib);) // -i, --include
        libs.push_back(lib);
    eat_option(args, {"-o", "--output="}, output); // -o, --output
    bool pause = eat_flag(args, {"-p", "--pause"}); // -p, --pause
    bool version = eat_flag(args, {"-v", "--version"}); // -v, --version

    if (help)
        return show_help();

    if (version)
        show_version();

    for (const auto& arg : args)
        files.push_back(arg);
    args.clear();

    if (files.empty())
    {
        std::cout << "Please provide at least one file to compile. See help for more information." << std::endl;
        return exit_cli(pause, -1);
    }

    const auto builder = new csaw::Builder();

    auto callback = [builder, code](const csaw::StatementPtr& ptr)
    {
        if (code) std::cout << ptr << std::endl;
        builder->Gen(ptr);
    };

    for (const auto& file : files)
    {
        std::cout << "[Build] " << file << std::endl;

        std::ifstream stream(file);
        if (!stream)
        {
            std::cout << "Failed to open source file" << std::endl;
            continue;
        }

        auto name = std::filesystem::path(file).filename().string();
        name = name.substr(0, name.find_last_of('.'));
        builder->BeginModule(name, file);

        std::vector<std::filesystem::path> processed;
        csaw::ParseData data(file, stream, callback, includes, processed);
        csaw::Parser::Parse(data);

        builder->EndModule();
    }

    if (!build.empty())
    {
        builder->Output(build, output);
    }

    if (jit)
    {
        jit_args.insert(jit_args.begin(), executable.c_str());
        auto exit_code = builder->RunJIT(entry, static_cast<int>(jit_args.size()), jit_args.data());
        std::cout << "[JIT] Exit Code " << exit_code << std::endl;
        return exit_cli(pause, exit_code);
    }

    return exit_cli(pause, 0);
}
