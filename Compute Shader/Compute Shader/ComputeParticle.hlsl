#ifndef COMPUTEPARTICLE_HLSL
#define	COMPUTEPARTICLE_HLSL

#include "ShaderDeclarations.hlsl"

#define MAX_PARTICLES 1024
#define GRAVITY_STRENGTH 20.0f

#define CAMERA_OFFSET 10.0f

RWStructuredBuffer<ParticleData> particles : register(u0);

[numthreads(MAX_PARTICLES, 1, 1)]
void main( uint ThreadID : SV_GroupIndex , uint3 GroupID : SV_GroupID)
{
	if(ThreadID < MAX_PARTICLES)
	{
		uint index = GroupID.x * MAX_PARTICLES + ThreadID;

		float3 particlePos = particles[index].position;

		/*float3 camToParticle = particlePos - CameraPos;
		float dotVal = dot(CameraToGravity, camToParticle);

		particlePos = CameraPos + normalize(camToParticle) * CAMERA_OFFSET;//CameraPos + (camToParticle * dotVal);*/

		float3 vecToGravity = normalize(gravityPos - particlePos);
		vecToGravity *= (GRAVITY_STRENGTH * (GRAVITY_STRENGTH * 0.5f));

		float3 particleVelocity = particles[index].velocity + vecToGravity * fElapsedTime;

		particlePos += particleVelocity * fElapsedTime;

		particles[index].position = particlePos;
		particles[index].velocity = particleVelocity;
	}
}

#endif