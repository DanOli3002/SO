#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

static std::string shell_quote(const std::string &s)
{
    std::string out = "'";
    for (char c : s) {
        if (c == '\'') {
            // close, insert escaped single quote, reopen
            out += "'\\''"; // becomes: '\'' when interpreted by the shell
        } else {
            out.push_back(c);
        }
    }
    out += "'";
    return out;
}

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <N> <prog> [args...]\n";
        return 1;
    }

    // parse N
    long n = 0;
    try {
        n = std::stol(argv[1]);
    } catch (...) {
        std::cerr << "Invalid number: " << argv[1] << "\n";
        return 2;
    }
    if (n <= 0) {
        std::cerr << "N must be >= 1\n";
        return 3;
    }

    // parse command and optional inputs
    // semantics supported:
    // 1) test_prog N prog input1 input2 ...   (no prog args, tokens after prog are stdin inputs)
    // 2) test_prog N prog -- input1 input2 ... (no prog args, explicit separator)
    // 3) test_prog N prog arg1 arg2 -- input1 input2 ... (prog args before --, inputs after)
    
    // program is argv[2]
    std::ostringstream prog_cmd;
    prog_cmd << shell_quote(argv[2]);

    // find `--` if present
    int dashdash = -1;
    for (int j = 3; j < argc; ++j) if (std::string(argv[j]) == "--") { dashdash = j; break; }

    std::vector<std::string> inputs;

    if (dashdash >= 0) {
        // there is a `--`: tokens 3..dashdash-1 are prog args (if any), tokens after are inputs
        for (int j = 3; j < dashdash; ++j) {
            prog_cmd << ' ' << shell_quote(argv[j]);
        }
        for (int j = dashdash + 1; j < argc; ++j) inputs.emplace_back(argv[j]);
    } else {
        // no `--`: treat tokens after prog (argv[3..]) as stdin inputs
        for (int j = 3; j < argc; ++j) inputs.emplace_back(argv[j]);
    }

    // build the shell command using seq and a while-read loop
    std::ostringstream cmd;
    cmd << "seq 1 " << n << " | while read _; do ";

    if (!inputs.empty()) {
    // printf the inputs and pipe them into the program for each run
    // use a quoted format string so the shell treats the format as one token
    cmd << "printf '%s\\n' ";
        // add each quoted input as an argument to printf
        for (size_t j = 0; j < inputs.size(); ++j) {
            cmd << shell_quote(inputs[j]);
            if (j + 1 < inputs.size()) cmd << ' ';
        }
        cmd << " | ";
    }

    cmd << prog_cmd.str() << "; done";

    // debug: print the generated shell command
    std::cerr << "[test_prog] executing: " << cmd.str() << "\n";

    // execute command via system
    int rc = system(cmd.str().c_str());
    if (rc == -1) {
        std::cerr << "Failed to execute shell\n";
        return 4;
    }
    // return program exit status (shifted) - return 0 on success
    if (WIFEXITED(rc))
        return WEXITSTATUS(rc);
    return rc;
}
