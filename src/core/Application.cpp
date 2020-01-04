#include "core/Application.hpp"

#include "core/main.hpp"

void func(int i) {}

void evc(Event* event) {}

void evc2(Event* event) {
	std::cout << "An event occured!" << std::endl;
}

Application::Application() {
	std::map<std::string, int> options;
	options["width"] = 2560;
	options["height"] = 1440;
	options["fullscreen"] = 1;
	options["vsync"] = 0;

	m_window = std::unique_ptr<Window>(new Window(options, this));

	m_camera = std::unique_ptr<Camera>(new Camera(m_window->getAspectRatio()));

	m_engine = std::unique_ptr<RenderEngine>(new RenderEngine(m_camera));
	m_engine->setClearColour({ 0.2f, 0.2f, 0.2f, 1.0f });
}

void Application::handleEvent(Event* event) {
	switch (event->getEventType()) {
	case Event::EventType::windowResize:
		WindowResizeEvent* e = (WindowResizeEvent*)event;
		m_window->updateResolution(e->getWidth(), e->getHeight());
		m_camera->updateProjectionMatrix(m_window->getAspectRatio());
		break;
	}
	delete event;
}

void Application::setupScene() {
	// Shader
	m_shader.reset(Shader::create("shader.vs", "shader.fs"));

	// Buffer Layout
	std::vector<BufferElement> elements = {
		{ ShaderDataType::Float, 3, "position", false },
		{ ShaderDataType::Float, 3, "colour", false }
	};
	std::shared_ptr<BufferLayout> bufferLayout = std::make_shared<BufferLayout>(elements);

	// Vertex Buffer
	float vertices[4 * 6] = {
		-0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.0f,  1.0f,  1.0f,  1.0f
	};
	std::shared_ptr<VertexBuffer> squareVB;
	squareVB.reset(VertexBuffer::create(vertices, sizeof(vertices)));
	squareVB->setLayout(bufferLayout);

	// Index Buffer
	uint32_t indices[6] = { 0, 1, 2, 0, 2, 3 };
	std::shared_ptr<IndexBuffer> squareIB;
	squareIB.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

	// Vertex Array
	m_squareVA.reset(VertexArray::create());
	m_squareVA->addVertexBuffer(squareVB);
	m_squareVA->setIndexBuffer(squareIB);
}

void Application::updateScene(float deltaTime) {
	if (m_window->keyPressed(KEY_W))
		m_camPos.y += deltaTime * 1.0f;
	if (m_window->keyPressed(KEY_S))
		m_camPos.y -= deltaTime * 1.0f;
	if (m_window->keyPressed(KEY_A))
		m_camPos.x -= deltaTime * 1.0f;
	if (m_window->keyPressed(KEY_D))
		m_camPos.x += deltaTime * 1.0f;

	if (m_window->keyPressed(KEY_Q))
		m_camRot -= deltaTime * 90.0f;
	if (m_window->keyPressed(KEY_E))
		m_camRot += deltaTime * 90.0f;

	m_camera->setPosition(m_camPos);
	m_camera->setRotation(m_camRot);
}

void Application::mainLoop() {

	Timer timer;
	timer.start();

	while (!m_window->shouldClose()) {
		m_engine->clear();

		updateScene(timer.deltaTime());

		m_engine->begin();
		{
			m_engine->submit(m_squareVA, m_shader);
		}
		m_engine->end();

		//engine->flush();
		if (m_window->keyPressed(KEY_H))
			std::cout << timer.timeSinceStart() << std::endl;

		m_window->update();
	}
}

Application::~Application() {}