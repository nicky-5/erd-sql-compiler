#include <stdio.h>

#include "backend/code-generation/generator.h"
#include "backend/support/logger.h"
#include "backend/support/shared.h"
#include "frontend/syntactic-analysis/bison-parser.h"

CompilerState state;

const int main(const int argumentCount, const char** arguments) {
    for (int i = 0; i < argumentCount; ++i) {
        LogInfo("Argument %d: '%s'", i, arguments[i]);
    }

    LogInfo("Compiling...\n");
    const int result = yyparse();
    switch (result) {
        case 0:
            if (state.succeed) {
                LogInfo("Compiled successfully.\n");
                Generator(state.program);
            } else {
                LogError("Linker error.");
                return -1;
            }
            break;
        case 1:
            LogError("Syntax error.");
            break;
        case 2:
            LogError("Memory error.");
            break;
        default:
            LogError("Unknown error (code %d).", result);
    }
    LogInfo("Fin.");
    return result;
}
