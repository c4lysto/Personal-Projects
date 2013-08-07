#ifndef FILETRANSLATOR_H
#define FILETRANSLATOR_H

#include <maya/MPxFileTranslator.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>

#include <fstream>
#include <vector>

#include "Mesh.h"

class FileTranslator : public MPxFileTranslator
{
private:
	std::vector<Mesh> m_vMeshes;

	MStatus ExportAll(const MFileObject& file);
	MStatus ExportSelected(const MFileObject& file);

	MStatus ExportMesh(MDagPath& meshPath);
	MStatus GetTextureNames(MFnMesh& fnMesh, Mesh& currMesh);

	MStatus WriteToBinary(std::ofstream& outputFile);

public:
	FileTranslator(void) {}
	virtual ~FileTranslator(void) {}

	static void* creator();
	virtual MStatus writer(const MFileObject& file,  const MString& optionsString, FileAccessMode mode);

	virtual bool haveWriteMethod () const {return true;}
	virtual MString defaultExtension () const {return "smsh";}
	virtual MString filter () const {return "*.smsh";}
};

#endif