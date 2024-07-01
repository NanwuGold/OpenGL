#version 330 core
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;
//layout(points, max_vertices = 6) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 screenSize; // 屏幕尺寸

in vec3 vPos[];

void generateAxis(vec3 origin,vec3 dir)
{
    // 投影原点
    vec4 projOrigin = projection * view * model * vec4(origin, 1.0);
    vec4 projDir = projection * view * model * vec4(origin + dir, 1.0);

    // 计算方向向量的屏幕空间长度
    vec2 screenDir = (projDir.xy / projDir.w - projOrigin.xy / projOrigin.w) * 0.5 * screenSize;

    // 设置所需的像素长度
    float desiredLength = 50.0; // 50像素长度
    float scale = desiredLength / length(screenDir);

    // 缩放方向向量
    vec3 scaledDir = dir * scale;

    // 投影缩放后的方向向量
    projDir = projection * view * model * vec4(origin + scaledDir, 1.0);

    // 原点顶点
     gl_Position = projOrigin;
    EmitVertex();

    // 方向顶点
    gl_Position = projDir;
    EmitVertex();

    EndPrimitive();
}

void main()
{
    vec3 originPoint = vPos[0];
    vec3 x = vPos[1];
    vec3 y = vPos[2];
    vec3 z = normalize(cross(vPos[1],vPos[2])).xyz;
    x = normalize(cross(y,z));
    y = normalize(cross(x,z));

    generateAxis(originPoint,vPos[1]); // 红色X轴
    generateAxis(originPoint,vPos[2]); // 绿色Y轴
    generateAxis(originPoint,z); // 蓝色Z轴
}