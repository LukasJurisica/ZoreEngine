#include "graphics/opengl/GLVertexLayout.hpp"
#include "graphics/opengl/GLShader.hpp"
#include "graphics/opengl/GLBuffer.hpp"
#include "debug/Debug.hpp"
#include <glad/glad.h>

namespace zore {

	//========================================================================
	//	OpenGL Vertex Buffer Layout Class
	//========================================================================

	const unsigned int VertexDataTypeToGLDataType[] = {
		GL_BOOL, GL_BYTE, GL_UNSIGNED_BYTE, GL_INT, GL_UNSIGNED_INT, GL_FLOAT
	};

	GLVertexLayout::GLVertexLayout(const std::string& name, Shader* shader, const std::vector<VertexElement>& vertexElements, const std::vector<VertexElement>& instanceElements, unsigned int interval) : VertexLayout(name) {
		// Create the Vertex Array Object
		glCreateVertexArrays(1, &id);
		glBindVertexArray(id);

		// Get the ID of the Shader for retrieving Vertex Attribute Locations
		shaderID = reinterpret_cast<GLShader*>(shader)->GetShaderID();

		InitAttributes(vertexElements, 0, 0);
		InitAttributes(instanceElements, 1, interval);
	}

	void GLVertexLayout::Bind() const {
		glBindVertexArray(id);
	}

	void GLVertexLayout::Unbind() const {
		glBindVertexArray(0);
	}

	void GLVertexLayout::InitAttributes(const std::vector<VertexElement>& elements, unsigned int index, unsigned int interval) {
		unsigned int offset = 0;
		for (const VertexElement& e : elements) {
			// Verify that the element exists in the shader
			int loc = glGetAttribLocation(shaderID, e.name.c_str());
			if (loc == -1) {
				Logger::Warn("Invalid Buffer Element name \"" + e.name + "\" in layout: " + name);
				continue;
			}

			// Get the OpenGL representation of the data type
			unsigned int type = VertexDataTypeToGLDataType[static_cast<int>(e.type)];

			// Set the Vertex Attribute Format
			glEnableVertexArrayAttrib(id, loc);
			if (type == GL_DOUBLE)
				glVertexAttribLFormat(loc, e.count, type, offset);
			else if (type == GL_FLOAT || e.normalize)
				glVertexAttribFormat(loc, e.count, type, true, offset);
			else
				glVertexAttribIFormat(loc, e.count, type, offset);
			glVertexAttribBinding(loc, index);
			
			offset += VertexDataTypeSize[static_cast<int>(e.type)] * e.count;
		}
		glVertexBindingDivisor(index, interval);
	}
}