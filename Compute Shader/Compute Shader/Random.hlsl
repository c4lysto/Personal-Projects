#ifndef RANDOM_HLSL
#define RANDOM_HLSL

// Code cited from NVIDIA Direct3D SDK 10 Code Samples
// URL: http://developer.download.nvidia.com/SDK/10/direct3d/samples.html - Lightning Project

// Slightly altered for my needs

#define RANDOM_IA 16807
#define RANDOM_IM 2147483647
#define RANDOM_AM (1.0f/float(RANDOM_IM))
#define RANDOM_IQ 127773
#define RANDOM_IR 2836
#define RANDOM_MASK 123459876

struct RandGenerator
{
	int random_x;

	float Rand()
	{
		int k;
		float ans;

		random_x ^= RANDOM_MASK;								//XORing with MASK allows use of zero and other
		k = random_x / RANDOM_IQ;								//simple bit patterns for idum.
		random_x = RANDOM_IA * (random_x - k * RANDOM_IQ ) - RANDOM_IR * k;	//Compute idum=(IA*idum) % IM without overif
		if (random_x < 0) 
			random_x += RANDOM_IM;					//flows by Schrage’s method.

		ans = RANDOM_AM * random_x;					//Convert idum to a floating result.
		random_x ^= RANDOM_MASK;					//Unmask before return.

		return ans;
	}

	void SeedRand(int value)
	{
		random_x = value;
		Rand();
	}

	float Rand(float low, float high)
	{
		float v = Rand();
		return low * (1.0f - v) + high * v;
	}
};

#endif