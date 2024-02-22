#include <random>

using namespace std;

static std::random_device seed;
static std::default_random_engine generator(seed());
