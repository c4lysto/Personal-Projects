#include "FileTranslator.h"

#include <maya/MItDag.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MFnSet.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MGlobal.h>

void* FileTranslator::creator()
{
	return new FileTranslator;
}

MStatus FileTranslator::writer(const MFileObject& file,  const MString& optionsString, FileAccessMode mode)
{
	switch(mode)
	{
	// export all
	case kExportAccessMode:
		{
			ExportAll(file);
		}
		break;

	// Export Selected
	case kExportActiveAccessMode:
		{
			ExportSelected(file);
		}
		break;
	}

	return MStatus::kSuccess;
}

MStatus FileTranslator::ExportAll(const MFileObject& file)
{
	std::ofstream outputFile(file.fullName().asChar(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

	if(outputFile.is_open())
	{
		MStatus status;

		MItDag meshIter(MItDag::kDepthFirst, MFn::kMesh, &status);

		if(MStatus::kFailure == status)
		{
			outputFile.close();
			cout << "Failed To Create Mesh Iterator" << endl;
			return MStatus::kFailure;
		}

		MDagPath meshPath;

		for(; !meshIter.isDone(); meshIter.next())
		{
			if(MStatus::kSuccess == meshIter.getPath(meshPath))
			{
				if(MStatus::kSuccess == ExportMesh(meshPath))
				{
					cout << "\nMesh Exported!\nDag Path: " << meshPath.fullPathName().asChar() << endl;
				}
				else
				{
					cout << "\nError: Failed to Export Mesh!\nDag Path: " << meshPath.fullPathName().asChar() << endl;
				}
			}
			else
			{
				cout << "Failed to Obtaint Current Mesh Path" << endl;
			}
		}

		if(MStatus::kSuccess == WriteToBinary(outputFile))
		{
			MGlobal::displayInfo("Meshes Exported Successfully!");
		}
		else
		{
			MGlobal::displayInfo("Error: Meshes Exported Unsuccessfully!");
		}

		outputFile.close();
	}
	else
	{
		cout << "Failed to Open Output File." << endl;
		return MStatus::kFailure;
	}

	return MStatus::kSuccess;
}

MStatus FileTranslator::ExportSelected(const MFileObject& file)
{
	std::ofstream outputFile(file.fullName().asChar(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);

	if(outputFile.is_open())
	{
		MSelectionList selectionList;

		if(MStatus::kFailure == MGlobal::getActiveSelectionList(selectionList))
		{
			cout << "Error: Failed to Get Active Selection List!" << endl;
			return MStatus::kFailure;
		}

		MStatus status;
		MItSelectionList selectionIter(selectionList, MFn::kMesh, &status);

		if(MStatus::kFailure == status)
		{
			cout << "Error: Failed to create Selection List Iterator!" << endl;
			return MStatus::kFailure;
		}

		MDagPath meshPath;
		for(selectionIter.reset(); !selectionIter.isDone(); selectionIter.next())
		{
			if(MStatus::kSuccess == selectionIter.getDagPath(meshPath))
			{
				if(MStatus::kSuccess == ExportMesh(meshPath))
				{
					cout << "\nMesh Exported!\nDag Path: " << meshPath.fullPathName().asChar() << endl;
				}
				else
				{
					cout << "\nError: Failed to Export Mesh!\nDag Path: " << meshPath.fullPathName().asChar() << endl;
				}
			}
			else
			{
				cout << "Error: Failed to get Dag Path of Current Selected Mesh!";
				continue;
			}
		}

		if(MStatus::kSuccess == WriteToBinary(outputFile))
		{
			MGlobal::displayInfo("Meshes Exported Successfully!");
		}
		else
		{
			MGlobal::displayInfo("Error: Meshes Exported Unsuccessfully!");
		}

		outputFile.close();
	}
	else
	{
		cout << "Failed to Open Output File." << endl;
		return MStatus::kFailure;
	}


	return MStatus::kSuccess;
}

MStatus FileTranslator::ExportMesh(MDagPath& meshPath)
{
	MStatus status;

	MFnMesh currMesh(meshPath, &status);

	if(MStatus::kFailure == status)
	{
		cout << "Error: Failed to Acquire Mesh From Dag Path!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MPointArray vertices;
	if(MStatus::kFailure == currMesh.getPoints(vertices, MSpace::kWorld))
	{
		cout << "Error: Failed to Acquire Vertices from Mesh!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MFloatVectorArray normals;
	if(MStatus::kFailure == currMesh.getNormals(normals, MSpace::kWorld))
	{
		cout << "Error: Failed to Acquire Normals from Mesh!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MFloatVectorArray tangents;
	if(MStatus::kFailure == currMesh.getTangents(tangents, MSpace::kWorld))
	{
		cout << "Error: Failed to Acquire Tangents from Mesh!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MFloatArray U, V;
	if(MStatus::kFailure == currMesh.getUVs(U, V))
	{
		cout << "Error: Failed to Acquire UV Coordinates from Mesh!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MItMeshPolygon polygon(meshPath);

	Mesh outMesh;
	outMesh.m_sName = currMesh.name().asChar();
	GetTextureNames(currMesh, outMesh);

	VertNormTanUV vertex;
	Triangle triangle;

	int vertIndex, normIndex, tanIndex, UVIndex;
	unsigned int unPolyVertCount, unVertNum;
	std::vector<VertNormTanUV>::iterator vertexIterator;

	std::vector<unsigned int> polyIndices;

	for(polygon.reset(); !polygon.isDone(); polygon.next())
	{
		unPolyVertCount = polygon.polygonVertexCount();
		
		polyIndices.resize(unPolyVertCount);

		for(unsigned int i = 0; i < unPolyVertCount; ++i)
		{
			vertIndex = polygon.vertexIndex(i);
			normIndex = polygon.normalIndex(i);
			tanIndex = polygon.tangentIndex(i);
			polygon.getUVIndex(i, UVIndex);
			
			vertex.x = (float)vertices[vertIndex].x;
			vertex.y = (float)vertices[vertIndex].y;
			vertex.z = (float)vertices[vertIndex].z;

			vertex.Nx = normals[normIndex].x;
			vertex.Ny = normals[normIndex].y;
			vertex.Nz = normals[normIndex].z;

			vertex.Tx = tangents[tanIndex].x;
			vertex.Ty = tangents[tanIndex].y;
			vertex.Tz = tangents[tanIndex].z;

			vertex.u = U[UVIndex];
			vertex.v = V[UVIndex];

			for(unVertNum = 0; unVertNum < outMesh.m_vVertices.size(); ++unVertNum)
			{
				if(vertex == outMesh.m_vVertices[unVertNum])
				{
					polyIndices[i] = unVertNum;
					break;
				}
			}

			if(outMesh.m_vVertices.size() == unVertNum)
			{
				polyIndices[i] = (unsigned int)outMesh.m_vVertices.size();
				outMesh.m_vVertices.push_back(vertex);
			}

			if(i < 3)
				triangle.indices[i] = polyIndices[i];
		}

		outMesh.m_vTriangles.push_back(triangle);

		// if we are messing with a quad and not a triangle
		if(4 == unPolyVertCount)
		{
			triangle.index0 = polyIndices[0];
			triangle.index1 = polyIndices[2];
			triangle.index2 = polyIndices[3];

			outMesh.m_vTriangles.push_back(triangle);
		}
	}

	m_vMeshes.push_back(outMesh);

	return MStatus::kSuccess;
}

MStatus FileTranslator::GetTextureNames(MFnMesh& fnMesh, Mesh& currMesh)
{
	MStatus status;
	MObjectArray meshSets, meshComponents;
	unsigned int numSets;

	std::vector<std::string>& meshTextures = currMesh.m_vTextures;

	fnMesh.getConnectedSetsAndMembers(0, meshSets, meshComponents, true);
	numSets = meshSets.length();

	if(numSets > 1)
		--numSets;

	for(unsigned int i = 0; i < numSets; ++i)
	{
		MObject set = meshSets[i];
		MObject component = meshComponents[i];
		MFnSet fnSet(set);

		// Make sure this is a polygonal set
		MItMeshPolygon meshPolygonIt(fnMesh.dagPath(), component, &status);
		if(MStatus::kFailure == status)
			continue;

		MFnDependencyNode fnNode(set);
		MPlug shaderPlug = fnNode.findPlug("surfaceShader");
		if(shaderPlug.isNull())
			continue;

		MPlugArray connectedPlugs;
		shaderPlug.connectedTo(connectedPlugs, true, false, &status);
		if(MStatus::kFailure == status)
			continue;
		if(1 != connectedPlugs.length())
			continue;

		MPlug colorPlug = MFnDependencyNode(connectedPlugs[0].node()).findPlug("color", &status);
		if(MStatus::kFailure == status)
			continue;

		MItDependencyGraph itDG(colorPlug, MFn::kFileTexture, MItDependencyGraph::kUpstream, MItDependencyGraph::kBreadthFirst, MItDependencyGraph::kNodeLevel, &status);
		if(MStatus::kFailure == status)
			continue;

		itDG.disablePruningOnFilter();
		if(itDG.isDone())
			continue;

		MObject textureNode = itDG.thisNode();
		MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName");
		MString fullTexturePath("");

		filenamePlug.getValue(fullTexturePath);

		std::string textureName = fullTexturePath.asChar();

		// get only the texture name
		textureName.erase(0, textureName.find_last_of('/') + 1);
		textureName.shrink_to_fit();

		// make sure this texture is not a repeat
		size_t textureIndex = 0;
		for(; textureIndex < meshTextures.size(); ++textureIndex)
		{
			if(textureName == meshTextures[i].c_str())
				break;
		}

		// we have a new texture
		if(textureIndex == meshTextures.size())
			meshTextures.push_back(textureName);
	}

	return MStatus::kSuccess;
}

MStatus FileTranslator::WriteToBinary(std::ofstream& outputFile)
{
	unsigned int unNameSize, unNumTextures, 
				 unNumVerts, unNumPolygons,
				 unTexNameLength, unVertexType = VERTTYPE_VERTNORMTANUV,
				 unVertexStride = sizeof(VertNormTanUV);

	for(size_t i = 0; i < m_vMeshes.size(); ++i)
	{
		// write out size of the mesh's name
		unNameSize = (unsigned int)m_vMeshes[i].m_sName.length() + 1;
		outputFile.write((char*)&unNameSize, sizeof(unsigned int));

		// write out Mesh's name
		outputFile.write(m_vMeshes[i].m_sName.data(), unNameSize);


		// write out the number of textures we will be using
		std::vector<std::string>& textureList = m_vMeshes[i].m_vTextures;

		unNumTextures = (unsigned int)textureList.size();
		outputFile.write((char*)&unNumTextures, sizeof(unsigned int));

		for(unsigned int texIndex = 0; texIndex < unNumTextures; ++texIndex)
		{
			// write out string length
			unTexNameLength = (unsigned int)textureList[texIndex].size();
			outputFile.write((char*)&unTexNameLength, sizeof(unsigned int));

			// write out file name
			outputFile.write(textureList[texIndex].data(), unTexNameLength);
		}

		// write out the vertex type (enum identical to one used here)
		outputFile.write((char*)&unVertexType, sizeof(unsigned int));

		// write out Vertex Stride
		outputFile.write((char*)&unVertexStride, sizeof(unsigned int));

		// write out Number of Vertices this mesh has
		std::vector<VertNormTanUV>& vertList = m_vMeshes[i].m_vVertices;

		unNumVerts = (unsigned int)vertList.size();
		outputFile.write((char*)&unNumVerts, sizeof(unsigned int));

		// write out all vertices
		for(unsigned int vertNum = 0; vertNum < vertList.size(); ++vertNum)
		{
			outputFile.write((char*)&vertList[vertNum], sizeof(vertList[vertNum]));
		}


		// write out Number of Polygon this mesh has
		unNumPolygons = (unsigned int)m_vMeshes[i].m_vTriangles.size();
		outputFile.write((char*)&unNumPolygons, sizeof(unsigned int));

		std::vector<Triangle>& triList = m_vMeshes[i].m_vTriangles;
		for(unsigned int triIndex = 0; triIndex < triList.size(); ++triIndex)
		{
			outputFile.write((char*)&triList[triIndex], sizeof(triList[triIndex]));
		}
	}

	return MStatus::kSuccess;
}