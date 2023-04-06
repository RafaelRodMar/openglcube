#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glu.h>

// Define the dimensions of the window
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Define the vertex coordinates of the cube
GLfloat vertices[] = {
	// front
	-1.0, -1.0,  1.0,
	 1.0, -1.0,  1.0,
	 1.0,  1.0,  1.0,
	-1.0,  1.0,  1.0,
	// top
	-1.0,  1.0,  1.0,
	 1.0,  1.0,  1.0,
	 1.0,  1.0, -1.0,
	-1.0,  1.0, -1.0,
	// back
	 1.0, -1.0, -1.0,
	-1.0, -1.0, -1.0,
	-1.0,  1.0, -1.0,
	 1.0,  1.0, -1.0,
	 // bottom
	 -1.0, -1.0, -1.0,
	  1.0, -1.0, -1.0,
	  1.0, -1.0,  1.0,
	 -1.0, -1.0,  1.0,
	 // left
	 -1.0, -1.0, -1.0,
	 -1.0, -1.0,  1.0,
	 -1.0,  1.0,  1.0,
	 -1.0,  1.0, -1.0,
	 // right
	  1.0, -1.0,  1.0,
	  1.0, -1.0, -1.0,
	  1.0,  1.0, -1.0,
	  1.0,  1.0,  1.0,
};

// Define the texture coordinates of the cube
GLfloat texCoords[2 * 4 * 6] = {
	// front
	0.0, 0.0,
	1.0, 0.0,
	1.0, 1.0,
	0.0, 1.0,
};

// Define the indices of the cube triangles
GLuint indices[] = {
	// front
	 0,  1,  2,
	 2,  3,  0,
	 // top
	  4,  5,  6,
	  6,  7,  4,
	  // back
	   8,  9, 10,
	  10, 11,  8,
	  // bottom
	  12, 13, 14,
	  14, 15, 12,
	  // left
	  16, 17, 18,
	  18, 19, 16,
	  // right
	  20, 21, 22,
	  22, 23, 20
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

	for (int i = 1; i < 6; i++)
		memcpy(&texCoords[i * 4 * 2], &texCoords[0], 2 * 4 * sizeof(GLfloat));

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Textured Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	SDL_GL_CreateContext(window);

	// Initialize OpenGL
	glMatrixMode(GL_PROJECTION);
	//set the projection matrix
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
		int texOffset = 0;
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 36; i += 3) {
			glTexCoord2fv(texCoords + texOffset);
			glVertex3fv(&vertices[indices[i] * 3]);
			glTexCoord2fv(texCoords + 2 + texOffset);
			glVertex3fv(&vertices[indices[i + 1] * 3]);
			glTexCoord2fv(texCoords + 4 + texOffset);
			glVertex3fv(&vertices[indices[i + 2] * 3]);
			texOffset += 4;
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

