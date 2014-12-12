#ifndef BITSET_H
#define BITSET_H

template<u32 numBits>
class BitSet
{
	CompileTimeAssert(numBits>0, "Bit Set - Invalid Bit Set Size!");

private:
	u8 m_Bits[((numBits-1) >> 3) + 1];

	__forceinline u8& GetAssociatedByte(u32 bit)
	{
		return m_Bits[bit >> 3];
	}

	__forceinline u32 GetLocalBit(u32 bit)
	{
		return (1 << (bit & 0x7));
	}

public:
	BitSet()
	{
		ClearAllBits();
	}

	__forceinline void SetBit(u32 bit)
	{
		Assert(bit < numBits, "Bit Field - Trying to Set An Invalid Bit (%d), Max Bits: %d", bit, numBits);
		GetAssociatedByte(bit) |= GetLocalBit(bit);
	}

	__forceinline void ClearBit(u32 bit)
	{
		Assert(bit < numBits, "Bit Field - Trying to Set An Invalid Bit (%d), Max Bits: %d", bit, numBits);
		GetAssociatedByte(bit) &= ~GetLocalBit(bit);
	}

	__forceinline void SetBit(u32 bit, bool bSetBit)
	{
		Assert(bit < numBits, "Bit Field - Trying to Set An Invalid Bit (%d), Max Bits: %d", bit, numBits);
		bSetBit ? SetBit(bit) : ClearBit(bit);
	}

	__forceinline bool IsSet(u32 bit)
	{
		Assert(bit < numBits, "Bit Field - Trying to Set An Invalid Bit (%d), Max Bits: %d", bit, numBits);
		GetAssociatedByte(bit) & GetLocalBit(bit);
	}

	__forceinline void ToggleBit(u32 bit)
	{
		Assert(bit < numBits, "Bit Field - Trying to Set An Invalid Bit (%d), Max Bits: %d", bit, numBits);
		GetAssociatedByte(bit) ^= GetLocalBit(bit);
	}

	__forceinline void ClearAllBits()
	{
		memset(m_Bits, 0, sizeof(m_Bits));
	}

	__forceinline void SetAllBits()
	{
		memset(m_Bits, 0xFF, sizeof(m_Bits));
	}
};

#endif // BITSET_H