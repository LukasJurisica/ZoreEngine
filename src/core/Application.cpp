#include "core/Application.hpp"

#include <glad/glad.h>

#include "graphics/Shader.hpp"
#include "graphics/Buffer.hpp"
#include "core/main.hpp"

Application::Application() {}

static GLenum SDTtoGLDT(ShaderDataType type) {
	switch(type) {
	case ShaderDataType::Int:
		return GL_INT;
	case ShaderDataType::Float:
		return GL_FLOAT;
	case ShaderDataType::Mat:
		return GL_FLOAT;
	case ShaderDataType::Bool:
		return GL_BOOL;
	}
}

void Application::mainLoop() {
	std::map<std::string, int> options;
	options["width"] = 2560;
	options["height"] = 1440;
	options["fullscreen"] = 1;
	options["vsync"] = 0;

	window.init(options);
	RenderEngine engine = RenderEngine();
	engine.setClearColour(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

	Shader* shader = Shader::create("shader.vs", "shader.fs");
	shader->bind();

	// Opengl Stuff

	float vertices[3 * 6] = {
		-0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f
	};
	VertexBuffer* vbo = VertexBuffer::create(vertices, sizeof(vertices));

	{
		BufferLayout layout = {
			{ ShaderDataType::Float, 3, "position", false },
			{ ShaderDataType::Float, 3, "colour", false }
		};

		vbo->setLayout(layout);
	}
	//unsigned int vao;
	//glGenVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	int index = 0;
	for (const auto& element : vbo->getLayout()) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, element.count, SDTtoGLDT(element.type), element.normalized, layout.getStride(), (const void*)element.offset);
		index++;
	}


	uint32_t indices[3] = { 0, 1, 2 };
	IndexBuffer* ibo = IndexBuffer::create(indices, 3);

	while (!window.shouldClose()) {
		engine.clearScreen();

		//glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, nullptr);

		//std::cout << (window.keyPressed(KEY_H) ? "true" : "false") << std::endl;

		window.update();
	}
}

Application::~Application() {}