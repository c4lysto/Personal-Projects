#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>
#include "FileTranslator.h"

using std::cout;
using std::endl;

MStatus initializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin plugin(obj, "Static Mesh Exporter", "1.0");

	status = plugin.registerFileTranslator("Static Mesh Exporter", NULL, FileTranslator::creator);

	if(!status)
		MGlobal::displayInfo("Failed to Initialize Static Mesh Exporter!");
	else
		MGlobal::displayInfo("Initialized Static Mesh Exporter.");
	
	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;

	MFnPlugin plugin(obj);

	status = plugin.deregisterFileTranslator("Static Mesh Exporter");

	if(!status)
		MGlobal::displayInfo("Failed to UnInitialize Static Mesh Exporter!");

	return status;
}