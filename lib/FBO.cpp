#include "FBO.h"
#include <cstddef>

FBO::FBO(float width,float height) {

	// Set width and height
	this->width = width;
	this->height = height;

	// Generate a quad to render the texture on
	this->generateQuad();

	// Generate a frame buffer
	glGenFramebuffers(1, &this->fboId);

	// Bind fbo
	this->bind();

	// Create Texture and attach to fbo
	glGenTextures(1, &this->texture0Id);
	glBindTexture(GL_TEXTURE_2D, this->texture0Id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->texture0Id, 0); 


	// Render buffer, use depth and stencil
	glGenRenderbuffers(1, &this->rboId);
	glBindRenderbuffer(GL_RENDERBUFFER, this->rboId); 
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->width, this->height);  
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach depth and stencil buffer to fbo
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->rboId);


	// Unbind fbo
	this->unbind();
}

void FBO::generateQuad() {
	float quadVertices[] = { 
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->quadVaoId);
    glGenBuffers(1, &this->quadVboId);
    glBindVertexArray(this->quadVaoId);
    glBindBuffer(GL_ARRAY_BUFFER, this->quadVboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void FBO::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fboId);  

}

void FBO::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);  

}

void FBO::render() {
	glBindVertexArray(this->quadVaoId);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture0Id);
	glDrawArrays(GL_TRIANGLES, 0, 6);  
}

void FBO::dispose() {
	glDeleteVertexArrays(1, &this->quadVaoId);
    glDeleteBuffers(1, &this->quadVboId);
	glDeleteRenderbuffers(1, &this->rboId);
	glDeleteTextures(1, &this->texture0Id);
	glDeleteFramebuffers(1, &this->fboId);
}