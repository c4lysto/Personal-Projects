#ifndef HASHSTRING_H
#define HASHSTRING_H
#include "GlobalDefines.h"

class HashString
{
private:
	u32 m_Hash;

	u32 ComputeHash(const char* szString);

public:
	HashString() : m_Hash(0) {}
	HashString(const u32& rhs) : m_Hash(rhs) {}
	HashString(u32&& rhs) : m_Hash(rhs) {}
	HashString(HashString&& rhs) : m_Hash(rhs.m_Hash) {}
	HashString(const char* szString);

	const HashString& operator=(const HashString& rhs);
	const HashString& operator=(const u32& rhs);

	bool operator==(const HashString& rhs) {return m_Hash == rhs.m_Hash;}
	bool operator==(const u32& rhs) {return m_Hash == rhs;}

	bool operator!=(const HashString& rhs) {return m_Hash != rhs.m_Hash;}
	bool operator!=(const u32& rhs) {return m_Hash != rhs;}

	operator u32() {return m_Hash;}
};

#endif // HASHSTRING_H