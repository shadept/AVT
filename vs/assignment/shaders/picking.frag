#version 330 core

out vec4 out_Color;

uniform vec3 PickingColor;
 
void main(void)
{
    out_Color = vec4(PickingColor, 1.0);
}