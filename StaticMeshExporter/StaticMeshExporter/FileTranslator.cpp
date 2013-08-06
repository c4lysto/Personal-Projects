#include "FileTranslator.h"

#include <maya/MItDag.h>
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshPolygon.h>
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
					if(MStatus::kSuccess == WriteMeshToBinary(outputFile))
					{
						cout << "\nMesh Exported!\nDag Path: " << meshPath.fullPathName().asChar() << endl;
					}
					else
					{
						cout << "Mesh Failed to write to binary file!\nDag Path: " << meshPath.fullPathName().asChar() << endl;
					}
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
					if(MStatus::kSuccess == WriteMeshToBinary(outputFile))
					{
						cout << "\nMesh Exported!\nDag Path: " << meshPath.fullPathName().asChar() << endl;
					}
					else
					{
						cout << "Mesh Failed to write to binary file!\nDag Path: " << meshPath.fullPathName().asChar() << endl;
					}
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

	MFnMesh mesh(meshPath, &status);

	if(MStatus::kFailure == status)
	{
		cout << "Error: Failed to Acquire Mesh From Dag Path!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MPointArray vertices;
	if(MStatus::kFailure == mesh.getPoints(vertices, MSpace::kWorld))
	{
		cout << "Error: Failed to Acquire Vertices from Mesh!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MFloatVectorArray normals;
	if(MStatus::kFailure == mesh.getNormals(normals, MSpace::kWorld))
	{
		cout << "Error: Failed to Acquire Normals from Mesh!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MFloatVectorArray tangents;
	if(MStatus::kFailure == mesh.getTangents(tangents, MSpace::kWorld))
	{
		cout << "Error: Failed to Acquire Tangents from Mesh!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MFloatArray U, V;
	if(MStatus::kFailure == mesh.getUVs(U, V))
	{
		cout << "Error: Failed to Acquire UV Coordinates from Mesh!" << endl;
		cout << "Info: Error Occurred Before File Writing, File Corruption Averted." << endl;
		return MStatus::kFailure;
	}

	MItMeshPolygon polygon(meshPath);

	Mesh outMesh;
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



	return MStatus::kSuccess;
}

MStatus FileTranslator::WriteMeshToBinary(std::ofstream& outputFile)
{
	return MStatus::kSuccess;
}