#version 450
layout(location = 0) out vec4	out_color;

layout(location = 0) in vec3	in_pos;
layout(location = 1) in vec4	in_color;

layout(binding = 0) uniform Camera {
  mat4 view;
} cam;

void main() {
	gl_Position = cam.view * vec4(in_pos, 1.0);
	out_color = in_color;
}
