#ifndef EXPLODE_HLSL
#define EXPLODE_HLSL

#include "ShaderDeclarations.hlsl"
#include "Random.hlsl"

#define MAX_PARTICLES 1024

#define MIN_VEL -10.0f
#define MAX_VEL 10.0f

RWStructuredBuffer<ParticleData> particles : register(u0);

[numthreads(MAX_PARTICLES, 1, 1)]
void main(uint ThreadID : SV_GroupIndex , uint3 GroupID : SV_GroupID)
{
	if(ThreadID < MAX_PARTICLES)
	{
		RandGenerator generator;
		generator.SeedRand(GameTime * GroupID.x + ThreadID);

		uint index = GroupID.x * MAX_PARTICLES + ThreadID;

		particles[index].position = gravityPos;

		float3 newVel;
		newVel.x = generator.Rand(MIN_VEL, MAX_VEL);
		newVel.y = generator.Rand(MIN_VEL, MAX_VEL);
		newVel.z = generator.Rand(MIN_VEL, MAX_VEL);

		newVel = normalize(newVel);

		newVel *= generator.Rand(MIN_VEL, MAX_VEL);

		particles[index].velocity = newVel;
	}
}

#endif