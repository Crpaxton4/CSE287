#include <iostream>
#include <glm\glm.hpp>
#include "BasicIncludesAndDefines.h"

/*
* Answer questions here.
*/

/*
* Creates a mat2 and loads it with the following matrix:
* | 2  1 |
* | 4 -2 |
* Prints that matrix, in addition to it's inverse. From a prior class:
* The inverse is:
* | 0.25   0.125 |
* | 0.50  -0.250 |
*/
void basicTest() {
	std::cout << "basic test" << std::endl;
}

// Returns the elements in a particular row. Assumes the row number is valid.
glm::vec3 getRow(const glm::mat3 &mat, int row) {
	return glm::vec3(0, 0, 0);
}

// Returns the elements in a particular column. Assumes the column number is valid.
glm::vec3 getCol(const glm::mat3 &mat, int col) {
	return glm::vec3(0, 0, 0);
}

// Returns true if the matrix is invertible, false otherwise.
bool isInvertible(const glm::mat2 &mat) {
	return true;
}

// Returns true if the matrix is invertible, false otherwise.
bool isInvertible(const glm::mat3 &mat) {
	return true;
}

// Consider the linear system: Mx = y, where M is a 3x3 matrix and x and y are
// length 3 column vectors. This function solves for x and returns that vector.
// Returns the zero vector if M is singular.
glm::vec3 solveLinearSystem(const glm::mat3 &M, const glm::vec3 &y) {
	return glm::vec3(0, 0, 0);
}

// Adds all the matrices in the vector of matrices. If the vector is empty,
// return the zero matrix. That is, the value of M[0]+M[1]+..M[n-1] is returned.
glm::mat3 addMatrices(const std::vector<glm::mat3> &M) {
	return glm::mat3(0,0,0,0,0,0,0,0,0);
}

// Multiples all the matrices in the vector of matrices. This should be done
// sequentially using glm matrix multiplication. In other words, perform:
// M[0]*M[1]*...M[n-1],  where * denotes matrix multiplication. Returns,
// the identify matrix if the vector is empty.
glm::mat3 multiplyMatrices(const std::vector<glm::mat3> &M) {
	return glm::mat3(0, 0, 0, 0, 0, 0, 0, 0, 0);
}

// Multiplies a 3x3 matrix with a 3x1 column vector. That is, the value of
// M*x is returned.
glm::vec3 multiplyMatrixAndVertex(const glm::mat3 &M, const glm::vec3 &x) {
	return glm::vec3(0, 0, 0);
}

// Returns the result of M[0]*M[1]*...M[n-1]*x
glm::vec3 multiplyMatricesAndVertex(const std::vector<glm::mat3> &M, const glm::vec3 &x) {
	return glm::vec3(0, 0, 0);
}

// Returns the vector containing: <M*x[0], M*x[1], ... M*x[n-1]>
std::vector<glm::vec3> multiplyMatrixAndVertices(const glm::mat3 &M, const std::vector<glm::vec3> &verts) {
	std::vector<glm::vec3> result;
	result.push_back(glm::vec3(0, 0, 0));
	return result;
}

// Returns the vector containing: <BIGMAT*x[0], BIGMAT*x[1], ... BIGMAT*x[n-1]>,
// where BIGMAT = M[0]*M[1]*...M[n-1]
std::vector<glm::vec3> multiplyMatricesAndVertices(const std::vector<glm::mat3> &M, const std::vector<glm::vec3> &verts) {
	std::vector<glm::vec3> result;
	result.push_back(glm::vec3(0, 0, 0));
	return result;
}

// This might be helpful for myinverse.
// Multiples each element in a row by a factor.
// For example, the call: multiplyRow(M, 0, 0.5), where
// M = | 2  1 1 0 |
//     | 4 -2 0 1 |
// Should alter M so that is now contains :
// M = | 1  0.5 0.5 0 |
//     | 4 -2   0   1 |
void multiplyRow(glm::mat4x2 &mat, int row, float factor) {
}

// This might be helpful for myinverse.
// Increments a destination row by a multiple of a source row.
// For example, the call: addRows(M, 1, 0, -4), where
// M = | 1  0.5 0.5 0 |
//     | 4 -2   0   1 |
// Should alter M so that is now contains :
// M = | 1  0.5  0.5 0 |
//     | 0 -4   -2   1 |
void addRows(glm::mat4x2 &M, int destRow, int sourceRow, float mult) {
}

glm::mat2 myinverse(const glm::mat2 &M) {
	glm::mat4x2 augmentedMatrix;
	// now, execute the operations performed, by hand, in last class:

	return glm::mat2(0,0,0,0);
}

int main(int argc, char** argv) {
	glm::vec3 V1(1, 2, 3);
	glm::vec3 V2(-1, -2, -3);

	glm::mat2 M2x2A(2, 4, 1, -2);

	glm::mat3 M3x3A(1,0,3,2,1,1,3,2,0);
	glm::mat3 M3x3B(1, 2, 3, 2, 1, 0, 0, 1, 0);
	glm::mat3 M3x3C(10, 0, 0, 0, 20, 0, 0, 0, 30);

	std::vector<glm::mat3> vMats = { M3x3A, M3x3B, M3x3C };
	std::vector<glm::vec3> vVecs = { V1, V2, V2, V1 };


	std::cout << "V1:" << std::endl;
	print(V1);
	std::cout << "V2:" << std::endl;
	print(V2);

	std::cout << "M2x2A:" << std::endl;
	print(M2x2A);
	std::cout << "M3x3A:" << std::endl;
	print(M3x3A);
	std::cout << "M3x3B:" << std::endl;
	print(M3x3B);
	std::cout << "M3x3C:" << std::endl;
	print(M3x3C);

	basicTest();

	print(getRow(M3x3A, 0));
	print(getRow(M3x3A, 1));
	print(getRow(M3x3A, 2));

	print(getCol(M3x3A, 0));
	print(getCol(M3x3A, 1));
	print(getCol(M3x3A, 2));

	std::cout << isInvertible(M2x2A) << std::endl;
	std::cout << isInvertible(M3x3A) << std::endl;

	print(solveLinearSystem(M3x3A, V1));

	print(addMatrices(vMats));
	print(multiplyMatrices(vMats));

	print(multiplyMatrixAndVertex(M3x3A, V1));
	print(multiplyMatricesAndVertex(vMats, V1));
	print(multiplyMatrixAndVertices(M3x3A, vVecs));
	print(multiplyMatricesAndVertices(vMats, vVecs));

	print(myinverse(M2x2A));

	return 0;
}
