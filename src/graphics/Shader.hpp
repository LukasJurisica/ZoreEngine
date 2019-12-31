#pragma once

class Shader {
public:
	virtual ~Shader() {}

	virtual void bind() = 0;

	static Shader* create(const char* vertpath, const char* fragpath);
};