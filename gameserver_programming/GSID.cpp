#include "PCH.h"
#include "GSID.h"

#include <random>

static std::random_device sRandomDevice;
static std::mt19937_64 eng(sRandomDevice());
static std::uniform_int_distribution<uint64> sUID;

GSID::GSID()
	: mID(sUID(eng))
{

}
