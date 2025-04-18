#version 330 core

out vec4 FragColor;

in vec3 FragPos;

uniform samplerCube cubemap;

void main()
{
    vec3 viewDir = normalize(FragPos);

    FragColor = texture(cubemap, viewDir);
    // FragColor = vec4(sin(viewDir), 1.0);
}
