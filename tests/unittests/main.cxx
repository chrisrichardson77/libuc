/**
 * Implements a main() method for a unit testing process
 */

#include <gmock/gmock.h>

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
