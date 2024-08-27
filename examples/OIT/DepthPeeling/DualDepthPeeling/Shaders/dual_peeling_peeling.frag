#version 330 core
layout(location = 0) out vec2 fDepth;
layout(location = 1) out vec4 FragColor1;
layout(location = 2) out vec4 FragColor2;

uniform sampler2D DepthTexture;
uniform sampler2D FrontColorTexture;

uniform vec2 viewSize;   ///< 视口分辨率
uniform vec4 outColor;

// 定义最大深度为1.0
#define MAX_DEPTH 1.0

in vec4 vColor;
in vec2 vTexCoord;

void main()
{
    /// Texture Coord
    vec2 TexCoord = gl_FragCoord.xy / viewSize;

    vec2 depthRange = texture(DepthTexture,TexCoord).xy;
    vec4 preFrontColor = texture(FrontColorTexture,TexCoord).xyzw;

    fDepth.xy = depthRange;
    FragColor1 = preFrontColor;
    FragColor2 = vec4(0.0);

    /// current fragment depth
    float frageDepth = gl_FragCoord.z;

    /// 当前像素上次剥离的最近深度
    float nearestDepth = -depthRange.x;
    /// 当前像素剥离的最远深度
    float farestDepth = depthRange.y;

    // alpha = 1.0−(1.0−forwardTemp.w)×(1.0−color.a) = color.a+forwardTemp.w − forwardTemp.w×color.a;

    float dstAlpha = preFrontColor.w;
    if(frageDepth < nearestDepth || frageDepth > farestDepth)  /// 表示当前像素被剥离或者此处空白
    {
        fDepth = vec2(-MAX_DEPTH);
        return ;
    }

    if(frageDepth > nearestDepth && frageDepth < farestDepth) ///< 当前像素的深度在两层之间
    {
        ///  先记录深度 这些层不应该在此时被剥离 只处理深度等于上一次的最小深度与最远深度
        fDepth = vec2(-frageDepth,frageDepth);
        return ;
    }

    fDepth = vec2(-MAX_DEPTH);  ///< 标记当前层已经被剥离了

    /// 剥离层
    if(frageDepth == nearestDepth)
    {
        // FragColor1.xyz = outColor.rgb * outColor.a * (1.0 - dstAlpha) + FragColor1.rgb * FragColor1.a;
        // FragColor1.a = outColor.a + dstAlpha - dstAlpha * outColor.a;

        /// 上述部分与下面等价 透明度展开之后为上面部分
        /// 第一次剥离过程
        /// FragColor1 在初始化前后层深度之的时候 第一次剥离前面层时 FragColor1的rgb == vec3(0.0), FragColor1.xyz += outColor.rgb * outColor.a * (1.0 - dstAlpha);
        /// FragColor1 = preFrontColor = vec4(0.0), FragColor1.xyz += outColor.rgb * outColor.a * (1.0 - dstAlpha) =>
        // FragColor1.xyz = vec4(0.0) + outColor.rgb * outColor.a * (1.0 - 0.0);

        FragColor1.xyz += outColor.rgb * outColor.a * (1.0 - dstAlpha);
        FragColor1.a = 1.0 - (1.0 - dstAlpha) * (1.0 - outColor.a);
    }
    else
    {
        FragColor2 += outColor;  /// 累计颜色值与透明度
    }
}

