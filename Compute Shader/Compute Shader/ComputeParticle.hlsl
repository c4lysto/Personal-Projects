#ifndef COMPUTEPARTICLE_HLSL
#define	COMPUTEPARTICLE_HLSL

#include "ShaderDeclarations.hlsl"

#define MAX_PARTICLES 1024
#define GRAVITY_STRENGTH 500.0f

#define CAMERA_OFFSET 10.0f
#define PARTICLE_MASS 10.0f
static const float PARTICLE_WEIGHT = (PARTICLE_MASS * GRAVITY_STRENGTH);

RWStructuredBuffer<ParticleData> particles : register(u0);

[numthreads(MAX_PARTICLES, 1, 1)]
void main( uint ThreadID : SV_GroupIndex , uint3 GroupID : SV_GroupID)
{
	if(ThreadID < MAX_PARTICLES)
	{
		uint index = GroupID.x * MAX_PARTICLES + ThreadID;

		float3 particlePos = particles[index].position;

		float3 vecToGravity = normalize(gravityPos - particlePos);

		float3 force = vecToGravity * GRAVITY_STRENGTH;


		float3 savePos = particlePos; 

		// now compute the new position using velocity verlet:  r(t+dt)=2*r(t)-r(t-dt)+a(t)*dt*dt 
		// that is new position= 2*(current position)-(previous position)+ (current acceleration)dt*dt
		//													acceleration
		particlePos = 2 * particlePos - particles[index].prevPos + force * fDeltaTime * fDeltaTime;

		float3 camToParticle = particlePos - CameraPos;
		if(length(camToParticle) > CAMERA_FAR_CLIP)
		{
			particlePos = CameraPos + normalize(camToParticle) * CAMERA_FAR_CLIP;
			savePos = particlePos;
		}

		particles[index].position = particlePos;
		particles[index].prevPos = savePos;
		particles[index].force = force;
	}
}

#endif