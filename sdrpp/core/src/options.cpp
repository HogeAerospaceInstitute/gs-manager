#include <options.h>
#include <spdlog/spdlog.h>
#include <stdlib.h>

namespace options {
    CMDLineOptions opts;

    void loadDefaults() {
#ifdef _WIN32
        opts.root = ".";
        opts.showConsole = false;
#else
        std::string homedir = getenv("HOME");
        opts.root = homedir + "/.config/sdrpp";
#endif
    }

    bool parse(int argc, char *argv[]) {
        for (int i = 1; i < argc; i++) {
            char* arg = argv[i];
            if (!strcmp(arg, "-r") || !strcmp(arg, "--root")) {
                if (i == argc - 1) { return false; }
                opts.root = argv[++i];
            }
            else if (!strcmp(arg, "-s") || !strcmp(arg, "--show-console")) {
                opts.showConsole = true;
            }
            else if (!strcmp(arg, "--server")) {
                opts.serverMode = true;
            }
            else {
                spdlog::error("Invalid command line option: {0}", arg);
                return false;
            }
        }
        return true;
    }
}