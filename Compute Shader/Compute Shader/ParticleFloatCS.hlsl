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

		float3 prevPos = particles[index].position;
		float3 particlePos = 2 * particles[index].position - particles[index].prevPos;
		
		if(any(particles[index].force))
		{
			particlePos += particles[index].force * 0.5f * fDeltaTime * fDeltaTime;
			particles[index].force = 0;
		}

		float3 camToParticle = particlePos - CameraPos;
		if(length(camToParticle) > CAMERA_FAR_CLIP)
		{
			particlePos = CameraPos + normalize(camToParticle) * CAMERA_FAR_CLIP;
			float3 savePos = prevPos;
			prevPos = particlePos;
			particlePos = savePos;
		}

		particles[index].position = particlePos;
		particles[index].prevPos = prevPos;
	}
}

#endif