#include "gtest/gtest.h"

extern "C"{
    void initEnvironment();
}

int main(int argc, char **argv) {
    initEnvironment();
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
