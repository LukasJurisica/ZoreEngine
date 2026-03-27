#shaderstage compute

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in; // Local work group size
layout(location = 0) uniform float a; // Uniform input a
layout(location = 1) uniform float b; // Uniform input b
layout(std430, binding = 1) buffer result { 
    writeonly restrict float c; 
};

void main() {
    // This simple shader adds a and b and stores the result in c.
    // gl_GlobalInvocationID can be used as an index for array processing.
    c = a + b; 
}