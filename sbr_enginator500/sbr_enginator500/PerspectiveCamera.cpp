#include "PerspectiveCamera.h"

#include "Common.h"	// For strToVec3() etc.

#include<glm\gtx\euler_angles.hpp>
#include <glm\gtc\matrix_inverse.hpp>	// For inversetranspose()

using 	glm::vec4;

// CONSTRUCTORS

// Create camera using the properties read from XML file
//PerspectiveCamera::PerspectiveCamera(std::map<string, string>& propertyList, Rasterizer* rasterizer)
PerspectiveCamera::PerspectiveCamera(std::map<string, string>& propertyList, Rasterizer* rasterizer)
{
	setFromPropList(propertyList, rasterizer);
}

// Set camera parameters, and calculate inverse of camera matrix for camera/viewing transformation
// Up vector is assumed (0, 1, 0)
// FOV is considered as full, not half. So, half of it should be taken in the code
PerspectiveCamera::PerspectiveCamera(Rasterizer* rasterizer, const vec3& campos, const vec3& lookat, float fov, float distN, float distF, float ratio)
{
	setCamera(rasterizer, campos, lookat, vec3(0, 1, 0), fov, distN, distF, ratio);
}

// Set camera parameters, and calculate inverse of camera matrix for camera/viewing transformation
// Up vector is specified
// FOV is considered as full, not half. So, half of it should be taken in the code
PerspectiveCamera::PerspectiveCamera(Rasterizer* rasterizer, const vec3& campos, const vec3& camUp, const vec3& lookat, float fov, float distN, float distF, float ratio)
{
	setCamera(rasterizer, campos, lookat, camUp, fov, distN, distF, ratio);
}

// PUBLIC METHODS
// Same thing with Camera(std::map<string, string>& propertyList, Rasterizer* rasterizer) constructor
// TODO FIND A BETTER SOLUTION FOR THIS
void PerspectiveCamera::setFromPropList(std::map<string, string>& propertyList, Rasterizer* rasterizer)
{
	// Set default values in case they are not set in the XML
	float fov = 90.f, distN = 1.f, distF = 3.f, ratio = 1.33f;
	vec3 up(0, 1, 0);

	vec3 pos, lookAt;

	if (propertyList.find("position") != propertyList.end())	// position property is set
	{
		float arr[3];

		strToVec3(propertyList.find("position")->second, arr);

		pos.x = arr[0];
		pos.y = arr[1];
		pos.z = arr[2];
	}

	if (propertyList.find("lookAt") != propertyList.end())	// lookAt property is set
	{
		float arr[3];

		strToVec3(propertyList.find("lookAt")->second, arr);

		lookAt.x = arr[0];
		lookAt.y = arr[1];
		lookAt.z = arr[2];
	}

	if (propertyList.find("upVector") != propertyList.end())	// upVector property is set
	{
		float arr[3];

		strToVec3(propertyList.find("upVector")->second, arr);

		up.x = arr[0];
		up.y = arr[1];
		up.z = arr[2];
	}

	if (propertyList.find("fov") != propertyList.end())	// fov property is set
	{
		fov = stof(propertyList.find("fov")->second);
	}

	if (propertyList.find("distanceNear") != propertyList.end())	// distanceNear property is set
	{
		distN = stof(propertyList.find("distanceNear")->second);
	}

	if (propertyList.find("distanceFar") != propertyList.end())	// distanceFar property is set
	{
		distF = stof(propertyList.find("distanceFar")->second);
	}

	if (propertyList.find("ratio") != propertyList.end())	// ratio property is set
	{
		ratio = stof(propertyList.find("ratio")->second);
	}

	setCamera(rasterizer, pos, lookAt, up, fov, distN, distF, ratio);

}	// End of setFromPropList()


// PRIVATE METHODS
void PerspectiveCamera::setCamera(Rasterizer* rasterizer, const vec3& campos, const vec3& lookat, const vec3& camUp, float fov, float distN, float distF, float ratio)
{
	mRasterizer = rasterizer;

	// Set parameters
	mCamPos = campos;
	mLookAt = lookat;

	mfov = fov;
	mdNear = distN;
	mdFar = distF;
	mRatio = ratio;

	mTranslation = glm::vec4(0, 0, 0, 1);

	// Generate basis vectors - RIGHT-HANDED COORDINATE SYSTEM IS USED
	// Get +Z direction
	vec3 camZ = mCamPos - mLookAt;
	camZ = glm::normalize(camZ);

	// Get +X direction
	vec3 upV = glm::normalize(camUp);	// For being sure
	vec3 camX = glm::cross(upV, camZ);

	// Get +Y direction
	vec3 camY = glm::cross(camZ, camX);	// Ideally it is normalized

	// Set camera matrix - GLM USES COLUMN MAJOR ORDER: 0 -> a[0,0], 1 -> a[1,0], 2 -> a[0,1], 3-> a[1,1] etc.
	// C = [u, v, w, e; 0 0 0 1] -> C = [camX, camY, camZ, camPos; 0, 0, 0, 1]

	mInvCamMatrix[0] = glm::vec4(camX, 0);		// Set first column
	mInvCamMatrix[1] = glm::vec4(camY, 0);		// Set second column
	mInvCamMatrix[2] = glm::vec4(camZ, 0);		// Set third column
	mInvCamMatrix[3] = glm::vec4(mCamPos, 1);	// Set fourth column

	// Take inverse of camera matrix
	mInvCamMatrix = glm::inverse(mInvCamMatrix);

	// Generate projection matrix

	// From songho.ca: http://www.songho.ca/opengl/gl_projectionmatrix.html
	float tangent = (float)tan((mfov * 0.5));
	float height = mdNear * tangent;
	float width = height * mRatio;


	float top = height;
	float bottom = -height;
	float right = width;
	float left = -width; // left = -top * ratio = bottom * ratio

	// Perspective projection matrix:
	// 2n/(r-l),	0,		(r+l)/(r-l),	0
	// 0,		2n /(t-b),	(t+b)/(t-b),	0
	// 0,			0,		-(f+n)/(f-n),	-2fn/(f-n)
	// 0,			0,			-1,			0

	// MORT
	mProjectionMatrix[0] = glm::vec4(((2 * mdNear) / (right - left)), 0, 0, 0);	// Set first column
	mProjectionMatrix[1] = glm::vec4(0, ((2 * mdNear) / (top - bottom)), 0, 0);	// Set second column
	mProjectionMatrix[2] = glm::vec4(((right + left) / (right - left)), ((top + bottom) / (top - bottom)), ((-mdFar - mdNear) / (mdFar - mdNear)), -1);	// Set third column
	mProjectionMatrix[3] = glm::vec4(0, 0, ((-2 * mdFar * mdNear) / (mdFar - mdNear)), 0);

}	// End of setCamer()



// Move camera
void PerspectiveCamera::moveCamera(const glm::vec3& direction, float deltaTime)
{
	//float speed = deltaTime; //* 0.5f;

	//mTranslation = glm::vec4((direction * speed), 1);

	//mat4x4 transformation;
	//transformation[0] = glm::vec4(1, 0, 0, 0);
	//transformation[1] = glm::vec4(0, 1, 0, 0);
	//transformation[2] = glm::vec4(0, 0, 1, 0);
	//transformation[3] = mTranslation;

	////mat4x4 transformationI;
	////transformationI[0] = glm::vec4(6, 6, 6, 0);
	////transformationI[1] = glm::vec4(6, 6, 6, 0);
	////transformationI[2] = glm::vec4(6, 6, 6, 0);
	////transformationI[3] = glm::vec4(3, 3, 3, 1);	//mTranslation;

	////transformationI = transformation * transformationI;

	//mInvCamMatrix = transformation * mInvCamMatrix;


	//std::cout << "Translation: " << glm::to_string(mTranslation) << std::endl;
}

// Rotate Camera
void PerspectiveCamera::rotateCamera(const float yaw, const float pitch, const float roll)
{
	//glm::mat4 R = glm::yawPitchRoll(glm::radians(yaw), glm::radians(pitch), glm::radians(roll));
	//mInvCamMatrix = mInvCamMatrix * R;  // ?  R * mInvCamMatrix
}

// Transform vertices of the given mesh list to the camera space
void PerspectiveCamera::transformToCameraSpace(const std::vector<Mesh*>& sourceMeshes, std::vector<TransformedMesh>& transformedMesh) //std::vector<std::vector<Vertex>>& transformedVerts)
{
	// ALGORITH:
	// 1. Take the face
	// 2. Check whether it's visible or not
	// 3. If it is visible, transform all vertices and normal, and put into TransformedMesh

	float EPSILON = 1e-09;


	int faceCounter = 0;

	// For each mesh in the scene
	for (int meshId = 0; meshId < sourceMeshes.size(); meshId++)
	{
		int vertexId = 0;
		const Mesh* nextMesh = sourceMeshes[meshId];
		glm::mat3 inverseTranspose = glm::inverseTranspose(glm::mat3(mInvCamMatrix * nextMesh->mTransformation));

		// For each face of the mesh
		for (int faceId = 0; faceId < nextMesh->mFace.size(); faceId++)
		{
			const Face* nextFace = &nextMesh->mFace[faceId];

			// Culling process
			vec3 transformedNormal = inverseTranspose * nextFace->mFaceNormal;	// Transform the face normal

			// Get first vertex position for culling
			vec4 dumPos = vec4(nextMesh->mVert[nextFace->mIndex[0]].mPos, 1);
			vec4 dumPos2 = vec4(nextMesh->mVert[nextFace->mIndex[1]].mPos, 1);
			vec4 dumPos3 = vec4(nextMesh->mVert[nextFace->mIndex[2]].mPos, 1);


			vec4 testPos = mInvCamMatrix *  nextMesh->mTransformation * dumPos;


			float result = glm::dot(testPos.xyz(), transformedNormal);

			

		if (result > EPSILON)
		{
				faceCounter++;

				// 1st vertex
				Vertex v1;
				vec4 v1Pos = mProjectionMatrix * testPos;

				// Make clipping

				// Convert to NDC
				v1.mPos.x = v1Pos.x / v1Pos.w;
				v1.mPos.y = v1Pos.y / v1Pos.w;
				v1.mPos.z = v1Pos.z / v1Pos.w;

				v1.mNormal = inverseTranspose * nextMesh->mVert[nextFace->mIndex[0]].mNormal;	// TODO MAYBE KEEPING VEC4 is better?
				v1.mUV = nextMesh->mVert[nextFace->mIndex[0]].mUV;
				transformedMesh[meshId].mVert.push_back(v1);

				// 2nd vertex
				Vertex v2;

				vec4 v2Pos = mProjectionMatrix * mInvCamMatrix *  nextMesh->mTransformation * vec4(nextMesh->mVert[nextFace->mIndex[1]].mPos, 1);
				// Make clipping

				// Convert to NDC
				v2.mPos.x = v2Pos.x / v2Pos.w;
				v2.mPos.y = v2Pos.y / v2Pos.w;
				v2.mPos.z = v2Pos.z / v2Pos.w;

				
				v2.mNormal = inverseTranspose * nextMesh->mVert[nextFace->mIndex[1]].mNormal;	// TODO MAYBE KEEPING VEC4 is better?
				v2.mUV = nextMesh->mVert[nextFace->mIndex[1]].mUV;
				transformedMesh[meshId].mVert.push_back(v2);

				// 3rd vertex
				Vertex v3;

				vec4 v3Pos = mProjectionMatrix * mInvCamMatrix *  nextMesh->mTransformation * vec4(nextMesh->mVert[nextFace->mIndex[2]].mPos, 1);
				// Make clipping

				// Convert to NDC
				v3.mPos.x = v3Pos.x / v3Pos.w;
				v3.mPos.y = v3Pos.y / v3Pos.w;
				v3.mPos.z = v3Pos.z / v3Pos.w;

				v3.mNormal = inverseTranspose * nextMesh->mVert[nextFace->mIndex[2]].mNormal;
				v3.mUV = nextMesh->mVert[nextFace->mIndex[2]].mUV;
				transformedMesh[meshId].mVert.push_back(v3);

				// Set new face and vertices
				Face newFace(vertexId, vertexId + 1, vertexId + 2);	vertexId += 3;
				transformedMesh[meshId].mFace.push_back(newFace);

			}	// Unculled faces

		}	// End of for each face of the mesh

	}	// End of for each mesh in the scene


	int a = 666;
	printf("Face counter: %d\n", faceCounter);

	//// for each mesh in the source mesh list
	//for (int meshId = 0; meshId < sourceMeshes.size(); meshId++)
	//{
	//	Mesh* currentMesh = sourceMeshes[meshId];

	//	// For each vertex in the current mesh
	//	for (int vi = 0; vi < currentMesh->mVert.size(); vi++)
	//	{
	//		// Copy vertex
	//		Vertex copyVert(currentMesh->mVert[vi]);

	//		// Transform the position
	//		// ORDER: newVertexPos = PROJECTION * VIEW * MODEL * old/defaultVertexPos
	//		glm::vec4 clipPos = mProjectionMatrix * mInvCamMatrix *  currentMesh->mTransformation * glm::vec4(copyVert.mPos, 1.f);	// currentMesh->mTransformation

	//		// Back face culling
	//		// clipPos.dot( transformedNormal ) > 0 ==> draw this one

	//		// TODO FACE LIST DON VERTEX YERINE

	//		// Make clipping

	//		// Convert to NDC
	//		copyVert.mPos.x = clipPos.x / clipPos.w;
	//		copyVert.mPos.y = clipPos.y / clipPos.w;
	//		copyVert.mPos.z = clipPos.z / clipPos.w;

	//		// Put transformed vertex to the list
	//		transformedVerts[meshId].at(vi) = copyVert;

	//	}	// End of for each vertex

	//}	// End of for each mesh
}

// Draw the scene
void PerspectiveCamera::draw(const std::vector<TransformedMesh>& transformedMesh) //(const std::vector<Mesh*>& sourceMeshes, const std::vector<std::vector<Vertex>>& verts)
{
	Color red(1, 0, 0);
	Color green(0, 1, 0);
	Color blue(0, 0, 1);

	// For each mesh
	for (int mi = 0; mi < transformedMesh.size(); mi++)
	{
		//For each face of the mesh
		for (int fi = 0; fi < transformedMesh[mi].mFace.size(); fi++)
		{
			// Get next face
			Face nextFace = transformedMesh[mi].mFace[fi];

			// Get related vertices
			vec3 pos1 = transformedMesh[mi].mVert[nextFace.mIndex[0]].mPos;
			vec3 pos2 = transformedMesh[mi].mVert[nextFace.mIndex[1]].mPos;
			vec3 pos3 = transformedMesh[mi].mVert[nextFace.mIndex[2]].mPos;

			// Convert to windows position
			pos1.x = (pos1.x + 1.f) * (mRasterizer->mWidth  * 0.5f) + 0.f;
			pos1.y = (pos1.y + 1.f) * (mRasterizer->mHeight * 0.5f) + 0.f;
			pos1.z = ((mdFar - mdNear) * 0.5f * pos1.z) + ((mdFar + mdNear) * 0.5f);	// For Z-Buffer

			pos2.x = (pos2.x + 1.f) * (mRasterizer->mWidth  * 0.5f) + 0.f;
			pos2.y = (pos2.y + 1.f) * (mRasterizer->mHeight * 0.5f) + 0.f;
			pos2.z = ((mdFar - mdNear) * 0.5f * pos2.z) + ((mdFar + mdNear) * 0.5f);	// For Z-Buffer

			pos3.x = (pos3.x + 1.f) * (mRasterizer->mWidth  * 0.5f) + 0.f;
			pos3.y = (pos3.y + 1.f) * (mRasterizer->mHeight * 0.5f) + 0.f;
			pos3.z = ((mdFar - mdNear) * 0.5f * pos3.z) + ((mdFar + mdNear) * 0.5f);	// For Z-Buffer

			// Draw
			//mRasterizer->drawLine(pos1.x, pos1.y, red, pos2.x, pos2.y, green);
			//mRasterizer->drawLine(pos1.x, pos1.y, red, pos3.x, pos3.y, blue);
			//mRasterizer->drawLine(pos2.x, pos2.y, green, pos3.x, pos3.y, blue);

			mRasterizer->rasterTriangle(pos1.x, pos1.y, red, pos2.x, pos2.y, red, pos3.x, pos3.y, red);

		}	// End of for each face of the mesh

	}	// end of for each mesh


	//Color red(1, 0, 0);
	//Color green(0, 1, 0);
	//Color blue(0, 0, 1);

	//// For each mesh
	//for (int mi = 0; mi < sourceMeshes.size(); mi++)
	//{
	//	//For each face of the mesh
	//	for (int fi = 0; fi < sourceMeshes[mi]->mFace.size(); fi++)
	//	{
	//		// Get next face
	//		Face nextFace = sourceMeshes[mi]->mFace[fi];

	//		// Get related vertices
	//		vec3 pos1 = verts[mi].at(nextFace.mIndex[0]).mPos;
	//		vec3 pos2 = verts[mi].at(nextFace.mIndex[1]).mPos;
	//		vec3 pos3 = verts[mi].at(nextFace.mIndex[2]).mPos;

	//		// Convert to windows position
	//		pos1.x = (pos1.x + 1.f) * (mRasterizer->mWidth  * 0.5f) + 0.f;
	//		pos1.y = (pos1.y + 1.f) * (mRasterizer->mHeight * 0.5f) + 0.f;
	//		pos1.z = ((mdFar - mdNear) * 0.5f * pos1.z) + ((mdFar + mdNear) * 0.5f);	// For Z-Buffer

	//		pos2.x = (pos2.x + 1.f) * (mRasterizer->mWidth  * 0.5f) + 0.f;
	//		pos2.y = (pos2.y + 1.f) * (mRasterizer->mHeight * 0.5f) + 0.f;
	//		pos2.z = ((mdFar - mdNear) * 0.5f * pos2.z) + ((mdFar + mdNear) * 0.5f);	// For Z-Buffer

	//		pos3.x = (pos3.x + 1.f) * (mRasterizer->mWidth  * 0.5f) + 0.f;
	//		pos3.y = (pos3.y + 1.f) * (mRasterizer->mHeight * 0.5f) + 0.f;
	//		pos3.z = ((mdFar - mdNear) * 0.5f * pos3.z) + ((mdFar + mdNear) * 0.5f);	// For Z-Buffer

	//		// Draw
	//		/*mRasterizer->drawLine(pos1.x, pos1.y, red, pos2.x, pos2.y, red);
	//		mRasterizer->drawLine(pos1.x, pos1.y, red, pos3.x, pos3.y, red);
	//		mRasterizer->drawLine(pos2.x, pos2.y, green, pos3.x, pos3.y, red);*/

	//		mRasterizer->rasterTriangle(pos1.x, pos1.y, red, pos2.x, pos2.y, red, pos3.x, pos3.y, red);

	//	}	// End of for each face of the mesh

	//}	// end of for each mesh

}	// End of draw()