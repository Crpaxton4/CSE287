#include "FrameBuffer.h"

/**
* Constructor. Allocates memory for storing pixel values.
*/
FrameBuffer::FrameBuffer( const int width, const int height) {
	setFrameBufferSize(width, height);

} // end FrameBuffer constructor


/**
* Deallocates dynamically memory associated with the class.
*/
FrameBuffer::~FrameBuffer(void)
{
	// Free the memory associated with the color buffer
	delete [] colorBuffer;
	delete[] depthBuffer;

} // end FrameBuffer destructor


/**
* Sizes the color buffer to match the window size. Deallocates any
* memory that was previsouly allocated.
*/
void FrameBuffer::setFrameBufferSize( const int width, const int height) {

	// Save the dimensions of the window
	window.width = width;
    window.height = height;

	// Set pixel storage modes. 
	// (https://www.opengl.org/archives/resources/features/KilgardTechniques/oglpitfall/)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// Free the memory previously associated with the color buffer
	delete [] colorBuffer;
	delete[] depthBuffer;

	// Allocate the color buffer to match the size of the window
	colorBuffer = new GLubyte[width*BYTES_PER_PIXEL*height];
	depthBuffer = new float[width*height];

} // end setFrameBufferSize


/**
* Sets the color to which the window will be cleared. Does NOT
* actually clear the window
*/
void FrameBuffer::setClearColor( const color & clear ) {

	clearColor[0] = (GLubyte)(clear.r * 255.0f);
	clearColor[1] =	(GLubyte)(clear.g * 255.0f);
	clearColor[2] =	(GLubyte)(clear.b * 255.0f);
	clearColor[3] =	(GLubyte)(clear.a * 255.0f);

} // end setClearColor


/**
* Clears the window to the clear color.
*/
void FrameBuffer::clearColorAndDepthBuffers() {

	for(int y = 0; y < window.height ; ++y) {
		for(int x = 0; x < window.width; ++x) {

			std::memcpy( colorBuffer + BYTES_PER_PIXEL * (x + y * window.width ), 
						 clearColor,  BYTES_PER_PIXEL );
			//depthBuffer[y * window.width + x] = 1.0f;
		
		}
	}

} // end clearFrameBuffer


/**
* Copies memory into frame buffer and updates the window
* using an OpenGL command.
*/
void FrameBuffer::showColorBuffer()
{
	// Insure raster position is lower left hand corner of the window. (OpenGL command)
	glRasterPos2d(-1, -1);

	// Copy color buffer to raster (OpenGL command)
	glDrawPixels( window.width, window.height, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer );
	
	// Flush all drawing commands and swapbuffers (Glut command)
	glutSwapBuffers();

} // end showFrameBuffer


/**
* Sets an individual pixel value in the color buffer. Origin (0,0)
* is the lower left hand corner of the window.
*/
void FrameBuffer::setPixel(const int x, const int y, const color & rgba) {
	color clampedColor = clamp(rgba, 0.0f, 1.0f);

	GLubyte c[] = { (GLubyte)(clampedColor.r * 255),
		(GLubyte)(clampedColor.g * 255),
		(GLubyte)(clampedColor.b * 255),
		(GLubyte)(clampedColor.a * 255) };

	std::memcpy(colorBuffer + BYTES_PER_PIXEL * (x+ y * window.width), c, BYTES_PER_PIXEL);
	//setPixel( x, y, c);
} // end setPixel
