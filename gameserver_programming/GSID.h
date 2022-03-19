#pragma once

#include <xhash>

class GSID
{
public:
	GSID();
	GSID(uint64 id)
		: mID(id) {}

	operator uint64() { return mID; }
	operator const uint64() const { return mID; }

private:
	uint64 mID;
};

namespace std {

	template<>
	struct hash<GSID>
	{
		std::size_t operator()(const GSID& uuid) const
		{
			return hash<uint64>()((uint64)uuid);
		}
	};

}