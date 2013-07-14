#ifndef PARTICLEFLOAT_HLSL
#define	PARTICLEFLOAT_HLSL

#include "ShaderDeclarations.hlsl"

#define MAX_PARTICLES 1024

RWStructuredBuffer<ParticleData> particles : register(u0);

[numthreads(MAX_PARTICLES, 1, 1)]
void main( uint ThreadID : SV_GroupIndex , uint3 GroupID : SV_GroupID)
{
	if(ThreadID < MAX_PARTICLES)
	{
		uint index = GroupID.x * MAX_PARTICLES + ThreadID;

		particles[index].position += particles[index].velocity * fElapsedTime;

		if(fElapsedTime == 0)
			particles[index].velocity = 0;
	}
}

#endif