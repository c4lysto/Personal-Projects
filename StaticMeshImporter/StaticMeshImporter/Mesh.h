#ifndef MESH_H
#define MESH_H

class Model;

enum MeshTexture {MESH_TEX_DIFFUSE, MESH_TEX_NORMAL, MESH_TEX_SPECULAR, MESH_TEX_MAX};

class Mesh
{
	friend class Model;

private:


public:
	Mesh(void);
	~Mesh(void);
};

#endif