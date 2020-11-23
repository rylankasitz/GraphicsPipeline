#include "stdafx.h"

//#define GEOM_SHADER 0

#include "CGInterface.h"
#include "vector.h"
#include "bbox.h"
#include "scene.h"
#include "framebuffer.h"
#include "tmesh.h"

#include <iostream>

using namespace std;

CGInterface::CGInterface() {};

void CGInterface::PerSessionInit() {

	glEnable(GL_DEPTH_TEST);
	cgContext = cgCreateContext();

	CGprofile latestVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
	CGprofile latestGeometryProfile = cgGLGetLatestProfile(CG_GL_GEOMETRY);
	CGprofile latestPixelProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);

	if (latestGeometryProfile == CG_PROFILE_UNKNOWN) {
		cerr << "ERROR: geometry profile is not available" << endl;
		exit(0);
	}

	cgGLSetOptimalOptions(latestGeometryProfile);
	CGerror Error = cgGetError();
	if (Error) {
		cerr << "CG ERROR: " << cgGetErrorString(Error) << endl;
	}

	cout << "Info: Latest GP Profile Supported: " << cgGetProfileString(latestGeometryProfile) << endl;
	cout << "Info: Latest VP Profile Supported: " << cgGetProfileString(latestVertexProfile) << endl;
	cout << "Info: Latest FP Profile Supported: " << cgGetProfileString(latestPixelProfile) << endl;

	geometryCGprofile = latestGeometryProfile;
	vertexCGprofile = latestVertexProfile;
	pixelCGprofile = latestPixelProfile;
}

bool ShaderOneInterface::PerSessionInit(CGInterface* cgi) {

	geometryProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE,
		"CG/shaderOne.cg", cgi->geometryCGprofile, "GeometryMain", NULL);
	if (geometryProgram == NULL) {
		CGerror Error = cgGetError();
		cerr << "Shader One Geometry Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}

	vertexProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE,
		"CG/shaderOne.cg", cgi->vertexCGprofile, "VertexMain", NULL);
	if (vertexProgram == NULL) {
		CGerror Error = cgGetError();
		cerr << "Shader One Vertex Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}

	fragmentProgram = cgCreateProgramFromFile(cgi->cgContext, CG_SOURCE,
		"CG/shaderOne.cg", cgi->pixelCGprofile, "FragmentMain", NULL);
	if (fragmentProgram == NULL) {
		CGerror Error = cgGetError();
		cerr << "Shader One Fragment Program COMPILE ERROR: " << cgGetErrorString(Error) << endl;
		cerr << cgGetLastListing(cgi->cgContext) << endl << endl;
		return false;
	}

	// load programs
	cgGLLoadProgram(geometryProgram);
	cgGLLoadProgram(vertexProgram);
	cgGLLoadProgram(fragmentProgram);

	// build some parameters by name such that we can set them later...
	vertexModelViewProj = cgGetNamedParameter(vertexProgram, "modelViewProj");
	geometryModelViewProj = cgGetNamedParameter(geometryProgram, "modelViewProj");

	fragmentTexture = cgGetNamedParameter(fragmentProgram, "texture");
	fragmentHasTexture = cgGetNamedParameter(fragmentProgram, "hasTexture");

	fragmentLight = cgGetNamedParameter(fragmentProgram, "light");
	fragmentFloor = cgGetNamedParameter(fragmentProgram, "quadFloor");
	fragmentBox1 = cgGetNamedParameter(fragmentProgram, "box1");
	fragmentBox2 = cgGetNamedParameter(fragmentProgram, "box2");
	fragmentBox3 = cgGetNamedParameter(fragmentProgram, "box3");

	return true;
}

void ShaderOneInterface::PerFrameInit() {

	cgGLSetStateMatrixParameter(vertexModelViewProj,
		CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	cgGLSetStateMatrixParameter(geometryModelViewProj,
		CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

	//cgGLSetParameter1f(fragmentHasTexture, (int)currentMesh->material.texture.textureHandle);
	//cgGLSetParameter1f(fragmentTexture, (int)currentMesh->material.texture.textureHandle);

	cgGLSetParameterArray3f(fragmentLight, 0, 4, (float*)scene->rm->meshes["arealight"].vertices.data());
	cgGLSetParameterArray3f(fragmentFloor, 0, 2, (float*)scene->rm->meshes["ground"].BoundingBox.Corners);

	cgGLSetParameterArray3f(fragmentBox1, 0, 36, (float*)scene->rm->meshes["cube1"].verticesArr);
	cgGLSetParameterArray3f(fragmentBox2, 0, 36, (float*)scene->rm->meshes["cube2"].verticesArr);
	cgGLSetParameterArray3f(fragmentBox3, 0, 36, (float*)scene->rm->meshes["cube3"].verticesArr);
}

void ShaderOneInterface::PerFrameDisable() {
}


void ShaderOneInterface::BindPrograms() {

	cgGLBindProgram(geometryProgram);
	cgGLBindProgram(vertexProgram);
	cgGLBindProgram(fragmentProgram);

}

void CGInterface::DisableProfiles() {

	cgGLDisableProfile(vertexCGprofile);
	cgGLDisableProfile(geometryCGprofile);
	cgGLDisableProfile(pixelCGprofile);

}

void CGInterface::EnableProfiles() {

	cgGLEnableProfile(vertexCGprofile);
	cgGLEnableProfile(geometryCGprofile);
	cgGLEnableProfile(pixelCGprofile);

}

