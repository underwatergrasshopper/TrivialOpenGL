#include <stdio.h>
#include <TrivialTestKit.h>
#include <TrivialOpenGL.h>

void TestDummy() {
    TTK_ASSERT(true);
}

int main() {
    TTK_ADD_TEST(TestDummy, 0);
    return !TTK_Run();
}