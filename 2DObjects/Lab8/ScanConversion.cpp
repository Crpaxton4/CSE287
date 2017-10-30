#include <algorithm>
#include "ScanConversion.h"

extern FrameBuffer colorBuffer;

void setPixel(int x, int y, const color &C) {
	int X = x + colorBuffer.getWindowWidth()/2;
	int Y = y + colorBuffer.getWindowHeight()/2;
	if (X >= 0 && X < colorBuffer.getWindowWidth() &&
		Y >= 0 && Y < colorBuffer.getWindowHeight()) {
		colorBuffer.setPixel(X, Y, C);
	}
}

void drawVerticalLine(int x, int bottom, int top, const color &rgba) {
	if (bottom > top) {
		std::swap(bottom, top);
	}
	for (int y = bottom; y <= top; y++) {
		setPixel(x, y, rgba);
	}
}

void drawHorizontalLine(int y, int left, int right, const color &rgba) {
	if (left > right) {
		std::swap(left, right);
	}
	for (int x = left; x <= right; x++) {
		setPixel(x, y, rgba);
	}
}

void drawAxis() {
	int W = colorBuffer.getWindowWidth();
	int H = colorBuffer.getWindowHeight();
	int W2 = W / 2;
	int H2 = H / 2;
	drawHorizontalLine(0, -W2, W2, red);
	drawVerticalLine(0, -H2, H2, green);
}

void drawBresenhamLine(const glm::vec3 &p1, const glm::vec3 &p2, const color &rgba) {
	int x0 = (int)p1.x;
	int y0 = (int)p1.y;
	int x1 = (int)p2.x;
	int y1 = (int)p2.y;

	int dx = std::abs(x1 - x0);
	int dy = std::abs(y1 - y0);

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;

	int err = dx - dy;

	setPixel(x0, y0, rgba);

	while (x0 != x1 && y0 != y1) {
		int e2 = err << 1;

		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}

		setPixel(x0, y0, rgba);
	}
}

void drawLine(const glm::vec3 &A, const glm::vec3 &B, const color &rgba) {
	if (A.x == B.x) {
		drawVerticalLine((int)A.x, (int)A.y, (int)B.y, rgba);
	} else if (A.y == B.y) {
		drawHorizontalLine((int)A.y, (int)A.x, (int)B.x, rgba);
	} else {
		drawBresenhamLine(A, B, rgba);
	}
}

struct edge {
	float yMin, yMax;
	float m, x, b;
	float oneOverM;

	glm::vec3 p1, p2;

	bool isHorizontal, isVertical;

	edge(const glm::vec3 &v1, const glm::vec3 &v2) {
		if (v1.y > v2.y) {
			p1 = v2;
			p2 = v1;
		} else {
			p1 = v1;
			p2 = v2;
		}

		isHorizontal = p1.y == p2.y;
		m = isHorizontal ? 0 : m = (p2.y - p1.y) / (p2.x - p1.x);
		oneOverM = m == 0 ? 0 : 1.0f / m;

		isVertical = p1.x == p2.x;

		yMin = p1.y;
		yMax = p2.y;
		x = p1.x;
		b = p1.y - m*p1.x;
	}
};

void drawWirePolygon(const std::vector<glm::vec3> &pts, const color &rgba) {
	for (unsigned int i = 1; i < pts.size(); i++) {
		drawLine(pts[i - 1], pts[i], rgba);
	}
	drawLine(pts[0], pts[pts.size() - 1], rgba);
}

void sortEdges(std::vector<edge> &edgeList) {
	sort(edgeList.begin(), edgeList.end(),
		[](const edge &e1, const edge &e2) { return e1.yMin < e2.yMin; });
}

void drawFilledPolygon(const std::vector<glm::vec3> &vertices, const color &rgba) {
	if (vertices.size() < 3) {
		return;
	}
	// vector holding all edges in polygon
	std::vector<edge> edgeList;

	// create edges from vector of vertices and add to edgeList
	for (unsigned int i = 0; i<vertices.size() - 1; i++) {
		if (!edge(vertices[i], vertices[i + 1]).isHorizontal) edgeList.push_back(edge(vertices[i], vertices[i + 1]));
	}
	// connect edge from last point to first point
	if (!edge(vertices[vertices.size() - 1], vertices[0]).isHorizontal) edgeList.push_back(edge(vertices[vertices.size() - 1], vertices[0]));

	sortEdges(edgeList);
	float scanLine = edgeList[0].yMin;

	// for all the edges in the vector
	while (edgeList.size() > 1) {
		// while the scanLine hasn't exceeded max y value of either edge
		while ((scanLine <= edgeList[0].yMax) && (scanLine <= edgeList[1].yMax)) {
			// placeholders for x values
			float xLeft, xRight;

			// if the edge is not vertical, use slope to calculate x
			if (edgeList[0].p1.x != edgeList[0].p2.x) {
				xLeft = (scanLine - edgeList[0].b) / edgeList[0].m;
			} else { // if vertical, use the x value without slope
				xLeft = edgeList[0].p1.x;
			}

			// if the edge is not vertical, use slope to calculate x
			if (edgeList[1].p1.x != edgeList[1].p2.x) {
				xRight = (scanLine - edgeList[1].b) / edgeList[1].m;
			} else { // if vertical, use the x value without slope
				xRight = edgeList[1].p1.x;
			}

			drawHorizontalLine((int)scanLine, (int)xLeft, (int)xRight, rgba);
			scanLine++;
		}

		// when scanLine exceeds the max of either edge, erase it
		if (scanLine > edgeList[0].yMax) {
			edgeList.erase(edgeList.begin() + 0); // scanline exceeded first edge
		} else {
			edgeList.erase(edgeList.begin() + 1); // scanline exceeded second edge
		}

		sortEdges(edgeList);
	}
}
