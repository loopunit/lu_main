#import "Main.h"
#include <CrossWindow/Common/Init.h>
#include "emscripten/html5.h"

int main()
{
    xwin::init(0, nullptr);
    xmain(0, nullptr);
    return 0;
}