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

		float3 camToParticle = particlePos - CameraPos;
		float dotVal = dot(CameraToGravity, camToParticle);

		particlePos = CameraPos + normalize(camToParticle) * CAMERA_OFFSET;//CameraPos + (camToParticle * dotVal);*/

		float3 vecToGravity = normalize(gravityPos - particlePos);
		vecToGravity *= GRAVITY_STRENGTH;

		float3 particleVelocity = particles[index].velocity + vecToGravity * fElapsedTime;

		particlePos += particleVelocity * fElapsedTime;

		particles[index].position = particlePos;
		particles[index].velocity = particleVelocity;

		/*if(particles[index].position.x > 15.0f)
		{
			particles[index].position.x = 15.0f;
			particles[index].velocity.x *= -1;
		}
		else if(particles[index].position.x < -5.0f)
		{
			particles[index].position.x = -5.0f;
			particles[index].velocity.x *= -1;
		}

		if(particles[index].position.y > 15.0f)
		{
			particles[index].position.y = 15.0f;
			particles[index].velocity.y *= -1;
		}
		else if(particles[index].position.y < -5.0f)
		{
			particles[index].position.y = -5.0f;
			particles[index].velocity.y *= -1;
		}

		if(particles[index].position.z > 15.0f)
		{
			particles[index].position.z = 15.0f;
			particles[index].velocity.z *= -1;
		}
		else if(particles[index].position.z < -5.0f)
		{
			particles[index].position.z = -5.0f;
			particles[index].velocity.z *= -1;
		}*/

		/*if((abs(particles[ThreadID].position.x) > 5.0f) ||
			(abs(particles[ThreadID].position.y) > 5.0f) ||
			(abs(particles[ThreadID].position.z > 5.0f)))
		{
			particles[ThreadID].position.x = 0.0f;
			particles[ThreadID].position.y = 0.0f;
			particles[ThreadID].position.z = 0.0f;
		}*/
	}
}

#endif