#version 450
layout(location = 0) out vec2	out_uv;
layout(location = 1) out vec4	out_color;

layout(location = 0) in vec2	in_pos;
layout(location = 1) in vec2	in_uv;
layout(location = 2) in vec4	in_color;

layout(push_constant) uniform uPushConstant {
    vec2 uScale;
    vec2 uTranslate;
} pc;

void main() {
	gl_Position = vec4(in_pos * pc.uScale + pc.uTranslate, 0.5, 1.0);
	out_color = in_color;
  out_uv = in_uv;
}
