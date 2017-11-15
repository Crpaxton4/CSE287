#include "PerVertexOperations.h"


// Pipeline transformation matrices
glm::mat4 PerVertex::modelingTransformation;
glm::mat4 PerVertex::viewingTransformation;
glm::mat4 PerVertex::projectionTransformation;
glm::mat4 PerVertex::viewportTransformation;
glm::vec3 PerVertex::eyePositionInWorldCoords;

bool PerVertex::perVertexLightingEnabled = false;
Render_Mode PerVertex::polygonRenderMode = FILL;

// Normalized device coordinate horizontal and vertical limits
const int PerVertex::xNdcMin = -1, PerVertex::yNdcMin = -1, PerVertex::xNdcMax = 1, PerVertex::yNdcMax = 1;

// View port limits
float PerVertex::xViewportMin, PerVertex::yViewportMin, PerVertex::xViewportMax, PerVertex::yViewportMax;

// Planes describing the normalized device coordinates view volume
std::vector<Plane> PerVertex::ndcPlanes{ Plane(glm::vec3(0, 1, 0), glm::vec3(0, -1, 0)),
Plane(glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0)),
Plane(glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)),
Plane(glm::vec3(-1, 0, 0), glm::vec3(1, 0, 0)),
Plane(glm::vec3(0, -1, 0), glm::vec3(0, 1, 0)),
Plane(glm::vec3(0, 0, -1), glm::vec3(0, 0, 1)) };


// Clips a convex polygon against a plane.
std::vector<VertexData> PerVertex::clipAgainstPlane(std::vector<VertexData> & verts, Plane & plane) {
	std::vector<VertexData> output;

	if (verts.size() > 2) {

		verts.push_back(verts[0]);

		for (unsigned int i = 1; i < verts.size(); i++) {

			bool v0In = plane.insidePlane(verts[i - 1]);
			bool v1In = plane.insidePlane(verts[i]);

			if (v0In && v1In) {

				output.push_back(verts[i]);
			} else if (v0In || v1In) {

				VertexData I = plane.findIntersection(verts[i - 1], verts[i]);
				output.push_back(I);

				if (!v0In && v1In) {

					output.push_back(verts[i]);
				}
			}
		}
	}

	return output;

} // end clipAgainstPlane


  // Break general convex polygons into triangles.
std::vector<VertexData> PerVertex::triangulate(const std::vector<VertexData> & poly) {
	std::vector<VertexData> triangles;
	triangles.push_back(poly[0]);
	triangles.push_back(poly[1]);
	triangles.push_back(poly[2]);

	for (unsigned int i = 2; i < poly.size() - 1; i++) {

		triangles.push_back(poly[0]);
		triangles.push_back(poly[i]);
		triangles.push_back(poly[i + 1]);
	}

	return triangles;

} // end triangulate


  // Clip polygons against a a normalized view volume. Vertices should be in 
  // clip coordinates.
std::vector<VertexData> PerVertex::clipPolygon(const std::vector<VertexData> & clipCoords) {
	std::vector<VertexData> ndcCoords;

	if (clipCoords.size()>2) {

		std::vector<VertexData> polygon;

		for (unsigned int i = 0; i<clipCoords.size() - 2; i += 3) {

			polygon.push_back(clipCoords[i]);
			polygon.push_back(clipCoords[i + 1]);
			polygon.push_back(clipCoords[i + 2]);

			for (Plane plane : ndcPlanes) {
				polygon = clipAgainstPlane(polygon, plane);
			}

			if (polygon.size() > 3) {
				polygon = triangulate(polygon);
			}

			for (VertexData v : polygon) {
				ndcCoords.push_back(v);
			}
			polygon.clear();
		}
	}

	return ndcCoords;

} // end clip


  // Clip line segments against a a normalized view volume. Vertices should be in 
  // clip coordinates.
std::vector<VertexData> PerVertex::clipLineSegments(const std::vector<VertexData> & clipCoords) {
	std::vector<VertexData> ndcCoords;

	if (clipCoords.size()>1) {

		for (unsigned int i = 0; i < clipCoords.size() - 1; i += 2) {

			VertexData v0 = clipCoords[i];
			VertexData v1 = clipCoords[i + 1];

			bool outsideViewVolume = false;

			for (unsigned int i = 0; i < ndcPlanes.size(); i++) {
				//for (Plane plane : ndcPlanes) {

				Plane plane = ndcPlanes[i];

				bool v0In = ndcPlanes[i].insidePlane(v0);
				bool v1In = ndcPlanes[i].insidePlane(v1);

				if (v0In == false && v1In == false) {

					outsideViewVolume = true;
					break; // Line segment is entirely clipped

				} else if (v0In == true && v1In == false) {

					v1 = ndcPlanes[i].findIntersection(v0, v1);
				} else if (v0In == false && v1In == true) {

					v0 = ndcPlanes[i].findIntersection(v0, v1);
				}
			}

			if (outsideViewVolume == false) {
				ndcCoords.push_back(v0);
				ndcCoords.push_back(v1);
			}
		}
	}

	return ndcCoords;

} // end clip

  // Remove all triangles that are not facing the view point.
  // Should be performed in orthographic coordinates such as normalized device coordinates.
std::vector<VertexData> PerVertex::removeBackwardFacingTriangles(const std::vector<VertexData> & triangleVerts) {
	std::vector<VertexData> fowardFacingTriangles;

	const glm::vec3 viewDirection(0.0f, 0.0f, -1.0f);

	for (unsigned int i = 0; i < triangleVerts.size() - 2; i += 3) {

		glm::vec3 n = findUnitNormal(triangleVerts[i].position.xyz, triangleVerts[i + 1].position.xyz, triangleVerts[i + 2].position.xyz);

		if (glm::dot(viewDirection, n) <= 0.0) {

			fowardFacingTriangles.push_back(triangleVerts[i]);
			fowardFacingTriangles.push_back(triangleVerts[i + 1]);
			fowardFacingTriangles.push_back(triangleVerts[i + 2]);
		}
	}

	return fowardFacingTriangles;

} // end removeBackwardFacingTriangles


  // Applies a transformation in the form of a matrix to a list of vertices.
std::vector<VertexData> PerVertex::transformVerticesToWorldCoordinates(const glm::mat4 & modelMatrix, const std::vector<VertexData> & vertices) {
	// Create 3 x 3 matrix for transforming normal vectors to world coordinates
	glm::mat3 modelingTransfomationForNormals(modelMatrix);
	modelingTransfomationForNormals = glm::transpose(glm::inverse(modelingTransfomationForNormals));

	std::vector<VertexData> transformedVertices;

	for (VertexData v : vertices) {

		glm::vec3 n = modelingTransfomationForNormals * v.normal;

		VertexData vt(modelMatrix * v.position, v.material, n);

		// Save the world position separately for use in per pixel lighting calculations
		vt.worldPosition = vt.position.xyz;

		transformedVertices.push_back(vt);
	}

	return transformedVertices;

} // end transformVertices


void PerVertex::applyLighting(std::vector<VertexData> & worldCoords) {
	for (int i = 0; i < (int)worldCoords.size(); i++) {

		float alpha = worldCoords[i].material.a;
		color totalLight = color(0.0);

		for (unsigned int j = 0; j < lights.size(); j++) {

			totalLight += lights[j]->illuminate(eyePositionInWorldCoords, worldCoords[i].worldPosition, worldCoords[i].normal, worldCoords[i].material);
		}

		worldCoords[i].material = totalLight;
		worldCoords[i].material.a = alpha;
	}

} // end applyLighting


  // Applies a transformation in the form of a matrix to a list of vertices.
std::vector<VertexData> PerVertex::transformVertices(const glm::mat4 & transMatrix, const std::vector<VertexData> & vertices) {
	std::vector<VertexData> transformedVertices;

	for (VertexData v : vertices) {

		VertexData vt(transMatrix * v.position, v.material, v.normal);

		// Save the world position separately for use in per pixel lighting calculations
		vt.worldPosition = v.worldPosition;

		transformedVertices.push_back(vt);
	}

	return transformedVertices;

} // end transformVertices


  // Tranforms triangle vertices from world to view port coordinate via eye, clip, and normalized device coordinates. 
  // Vertices are clipped and backfaces are culled. Lighting calculations are performed in World coordinates.
void PerVertex::processTriangleVertices(const std::vector<VertexData> & objectCoords) {
	// Modeling tranformation
	std::vector<VertexData> worldCoords = transformVerticesToWorldCoordinates(modelingTransformation, objectCoords);

	if (perVertexLightingEnabled == true) {

		// Perform lighting calculations in World coordinates
		applyLighting(worldCoords);
	}

	// Viewing Transformation
	std::vector<VertexData> eyeCoords = transformVertices(viewingTransformation, worldCoords);

	// Projections Transformation
	std::vector<VertexData> projCoords = transformVertices(projectionTransformation, eyeCoords);

	std::vector<VertexData> clipCoords;

	// Perspective division
	for (VertexData v : projCoords) {
		if (v.position.w >= 0)
			v.position /= v.position.w;
		else {
			v.position /= -v.position.w;
			v.position.z = -std::abs(v.position.z);
		}
		clipCoords.push_back(v);
	}

	// Backface Cullling
	clipCoords = removeBackwardFacingTriangles(clipCoords);

	// Clipping
	std::vector<VertexData> ndcCoords = clipPolygon(clipCoords);

	// Window Transformation
	std::vector<VertexData> windowCoords = transformVertices(viewportTransformation, ndcCoords);

	if (polygonRenderMode == FILL) {

		drawManyFilledTriangles(windowCoords);
	} else {

		drawManyWireFrameTriangles(windowCoords);
	}

} // end processTriangleVertices


  // Tranforms line segments from world to view port coordinate via eye, clip, and normalized device coordinates. 
  // Vertices are clipped . Lighting calculations are performed in World coordinates.
void PerVertex::processLineSegments(const std::vector<VertexData> & objectCoords) {
	// Modeling Transformation
	std::vector<VertexData> worldCoords = transformVerticesToWorldCoordinates(modelingTransformation, objectCoords);

	// Lines do not have meaningful normal vectors. Performing little calculations does not make sense.

	// Viewing Transformation
	std::vector<VertexData> eyeCoords = transformVertices(viewingTransformation, worldCoords);

	// Projection Transformation
	std::vector<VertexData> projCoords = transformVertices(projectionTransformation, eyeCoords);

	std::vector<VertexData> clipCoords;

	// Perspective division
	for (VertexData v : projCoords) {
		if (v.position.w >= 0)
			v.position /= v.position.w;
		else {
			v.position /= -v.position.w;
			v.position.z = -std::abs(v.position.z);
		}
		clipCoords.push_back(v);
	}

	// Clipping
	std::vector<VertexData> ndcCoords = clipLineSegments(clipCoords);

	// Window Transformation
	std::vector<VertexData> windowCoords = transformVertices(viewportTransformation, ndcCoords);

	drawManyLines(windowCoords);

} // end linePipeline