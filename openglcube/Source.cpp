/*This code loads a texture from file using SDL and sets up an OpenGL scene with
a textured cube. It uses the gluPerspective() function to set up the projection
matrix, and the gluLookAt() function to position the camera. The texture is bound
using glBindTexture(), and the cube is drawn using glBegin() and glEnd() with texture
coordinates and vertex indices. Finally, the buffer is swapped using SDL_GL_SwapWindow().*/

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// Define the dimensions of the window
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Define the vertex coordinates of the cube
GLfloat vertices[] = {
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f
};

// Define the texture coordinates of the cube
GLfloat texCoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

// Define the indices of the cube triangles
GLuint indices[] = {
	0, 1, 2, 2, 3, 0,
	1, 5, 6, 6, 2, 1,
	5, 4, 7, 7, 6, 5,
	4, 0, 3, 3, 7, 4,
	3, 2, 6, 6, 7, 3,
	4, 5, 1, 1, 0, 4
};

// Load a texture from file and return its ID
GLuint loadTexture(const char* filename) {
	SDL_Surface* surface = SDL_LoadBMP(filename);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
	SDL_FreeSurface(surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return textureID;
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Textured Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);

	// Initialize OpenGL
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// Load the texture
	GLuint textureID = loadTexture("texture.bmp");

	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set the camera position
		glLoadIdentity();
		gluLookAt(3.0f, 3.0f, 3.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		// Bind the texture
		glBindTexture(GL_TEXTURE_2D, textureID);

		// Draw the cube
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 36; i += 3) {
			glTexCoord2fv(texCoords);
			glVertex3fv(&vertices[indices[i] * 3]);
			glTexCoord2fv(texCoords + 2);
			glVertex3fv(&vertices[indices[i + 1] * 3]);
			glTexCoord2fv(texCoords + 4);
			glVertex3fv(&vertices[indices[i + 2] * 3]);
		}
		glEnd();

		// Swap the buffer and update the window
		SDL_GL_SwapWindow(window);
	}

	// Clean up
	glDeleteTextures(1, &textureID);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

