#ifndef MESH_H
#define	MESH_H

#include <maya/MStatus.h>
#include <string>
#include <vector>

enum VertexTypeEnum {VERTTYPE_VERT, VERTTYPE_VERTCLR, VERTTYPE_VERTNORMUV, VERTTYPE_VERTNORMTANUV, 
					 VERTTYPE_ANIMCLR, VERTTYPE_ANIMNORMUV, VERTTYPE_ANIMNORMTANUV, NUM_VERTEX_TYPES};

struct VertNormTanUV
{
	union
	{
		float position[3];

		struct
		{
			float x, y, z;
		};
	};

	union
	{
		float normal[3];

		struct
		{
			float Nx, Ny, Nz;
		};
	};

	union
	{
		float tangent[3];

		struct
		{
			float Tx, Ty, Tz;
		};
	};

	union
	{
		float UV[2];

		struct
		{
			float u, v;
		};
	};

	bool operator==(const VertNormTanUV& rhs)
	{
		return !memcmp(this, &rhs, sizeof(VertNormTanUV));
	}
};

struct Triangle
{
	union
	{
		unsigned int indices[3];

		struct
		{
			unsigned int index0, index1, index2;
		};
	};
};

struct Mesh
{
	// members
	std::string m_sName;
	std::vector<std::string> m_vTextures;
	std::vector<VertNormTanUV> m_vVertices;
	std::vector<Triangle> m_vTriangles;

	// methods
	Mesh(void){}
	~Mesh(void){}	
};

#endif