#version 450 core
layout(location = 0) out vec4 FragColor;

in vec2 vTexCoord;

uniform sampler2D lightTexture;
uniform vec2 Size;

float EDGE_THRESHOLD_MIN = 0.031;
float EDGE_THRESHOLD_MAX = 0.125;

void main()
{
    vec4 centerColor = texture(lightTexture,vTexCoord);  ///< ???????????

    /// TODO? others
    float lumaCenter = centerColor.a;   ///< ????????
    float lumaDown = textureOffset(lightTexture,vTexCoord,ivec2(0,-1)).a;   ///< ???????
    float lumaUp = textureOffset(lightTexture,vTexCoord,ivec2(0,1)).a;      ///< ???????
    float lumaLeft = textureOffset(lightTexture,vTexCoord,ivec2(-1,0)).a;   ///< ???????
    float lumaRight = textureOffset(lightTexture,vTexCoord,ivec2(1,0)).a;   ///< ???????

    /// ????????
    float lumaMin = min(lumaCenter,min(min(lumaDown,lumaUp),min(lumaLeft,lumaRight)));
    float lumaMax = max(lumaCenter,max(max(lumaDown,lumaUp),max(lumaLeft,lumaRight)));

    // Compute the delta.  -- ??? ?????
    float lumaRange = lumaMax - lumaMin;  ///< ????

    if(centerColor.rgb == vec3(37.0 / 255.0, 37.0 / 255.0, 38.0 / 255.0))
    {
        FragColor = centerColor;
        return ;
    }

    if(lumaRange < max(EDGE_THRESHOLD_MIN,lumaMax*EDGE_THRESHOLD_MAX))
    {
        FragColor = vec4(37.0 / 255.0, 37.0 / 255.0, 38.0 / 255.0,1.0);  ///< set as clear color
        FragColor = vec4(0.0,0.0,0.0,1.0);  ///< set as clear color
        return ;
    }

    /// TODO: do FXAA
    float lumaDownLeft = textureOffset(lightTexture,vTexCoord,ivec2(-1,-1)).a;
    float lumaUpRight = textureOffset(lightTexture,vTexCoord,ivec2(1,1)).a;
    float lumaUpLeft = textureOffset(lightTexture,vTexCoord,ivec2(-1,1)).a;
    float lumaDownRight = textureOffset(lightTexture,vTexCoord,ivec2(1,-1)).a;

    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;

    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;


    // Compute an estimation of the gradient along the horizontal and vertical axis.
    float edgeHorizontal =  abs(-2.0 * lumaLeft + lumaLeftCorners)  + abs(-2.0 * lumaCenter + lumaDownUp ) * 2.0    + abs(-2.0 * lumaRight + lumaRightCorners);
    float edgeVertical =    abs(-2.0 * lumaUp + lumaUpCorners)      + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0  + abs(-2.0 * lumaDown + lumaDownCorners);

    // Is the local edge horizontal or vertical ?
    bool isHorizontal = (edgeHorizontal >= edgeVertical);

    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;
    // Compute gradients in this direction.
    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;

    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    float gradientScaled = 0.25*max(abs(gradient1),abs(gradient2));
    float stepLength = isHorizontal ? 1.0 / Size.y : 1.0 / Size.x;
    float lumaLocalAverage = 0.0;
    if(is1Steepest)
    {
        // Switch the direction
        stepLength = - stepLength;
        lumaLocalAverage = 0.5*(luma1 + lumaCenter);
    }
    else
    {
        lumaLocalAverage = 0.5*(luma2 + lumaCenter);
    }

    vec2 currentUv = vTexCoord;
    if(isHorizontal)
    {
        currentUv.y += stepLength * 0.5;
    }
    else
    {
        currentUv.x += stepLength * 0.5;
    }

    vec3 finalColor = texture(lightTexture,currentUv).rgb;
    FragColor.rgb = finalColor;

}


#if  0
void main()
{
    vec4 centerColor = texture(lightTexture,vTexCoord);  ///< 当前采样的像素点的颜色

    /// TODO： others
    float lumaCenter = centerColor.a;   ///< 当前采样点的亮度
    float lumaDown = textureOffset(lightTexture,vTexCoord,ivec2(0,-1)).a;   ///< 下面像素的亮度
    float lumaUp = textureOffset(lightTexture,vTexCoord,ivec2(0,1)).a;      ///< 上面像素的亮度
    float lumaLeft = textureOffset(lightTexture,vTexCoord,ivec2(-1,0)).a;   ///< 左边像素的亮度
    float lumaRight = textureOffset(lightTexture,vTexCoord,ivec2(1,0)).a;   ///< 右边像素的亮度

    /// 计算最大最小亮度
    float lumaMin = min(lumaCenter,min(min(lumaDown,lumaUp),min(lumaLeft,lumaRight)));
    float lumaMax = max(lumaCenter,max(max(lumaDown,lumaUp),max(lumaLeft,lumaRight)));

    // Compute the delta.  -- 对比度 局部对比度
    float lumaRange = lumaMax - lumaMin;  ///< 亮度差值

    if(centerColor.rgb == vec3(37.0 / 255.0, 37.0 / 255.0, 38.0 / 255.0))
    {
        FragColor = centerColor;
        return ;
    }

    if(lumaRange < max(EDGE_THRESHOLD_MIN,lumaMax*EDGE_THRESHOLD_MAX))
    {
        FragColor = vec4(37.0 / 255.0, 37.0 / 255.0, 38.0 / 255.0,1.0);  ///< set as clear color
        FragColor = vec4(0.0,0.0,0.0,1.0);  ///< set as clear color
        return ;
    }

    /// TODO: do FXAA
    /// 计算亮度变化的梯度方向

    float lumaDownLeft = textureOffset(lightTexture,vTexCoord,ivec2(-1,-1)).a;
    float lumaUpRight = textureOffset(lightTexture,vTexCoord,ivec2(1,1)).a;
    float lumaUpLeft = textureOffset(lightTexture,vTexCoord,ivec2(-1,1)).a;
    float lumaDownRight = textureOffset(lightTexture,vTexCoord,ivec2(1,-1)).a;

    float filterFactor = 2.0 * (lumaDown + lumaUp + lumaLeft + lumaRight) + lumaDownLeft + lumaUpRight + lumaUpLeft + lumaDownRight;
    filterFactor *= 1.0/12.0f;

    filterFactor = abs(filterFactor - lumaCenter);
    filterFactor = clamp(filterFactor,0.0f,1.0f);

    float blendFactor = smoothstep(0.0f,1.0f,filterFactor);
    blendFactor = blendFactor * blendFactor;

    float horizontal = abs(lumaDown + lumaUp - 2 * lumaCenter) * 2 +
    abs(lumaUpRight + lumaDownRight - 2 * lumaRight) +
    abs(lumaUpLeft + lumaDownLeft - 2 * lumaLeft);
    float vertical = abs(lumaLeft + lumaRight - 2 * lumaCenter) * 2 +
    abs(lumaUpLeft + lumaUpRight - 2 * lumaUp) +
    abs(lumaDownLeft + lumaDownRight - 2 * lumaDown);

    bool isHorizontal = horizontal >= vertical;

    /// blend step
    float pixelStep = isHorizontal ? 1.0f / 600.0f: 1.0 / 800.0f;

    /// gradient dir
    float pLuminance = isHorizontal ? lumaUp : lumaRight;  ///< right & up
    float nLuminance = isHorizontal ? lumaDown : lumaLeft; ///< left & down

    float pGradient = abs(pLuminance - lumaCenter);  /// H
    float nGradient = abs(nLuminance - lumaCenter);  /// V


    if (pGradient < nGradient)
    {
        pixelStep = -pixelStep;
    }

    vec2 uv = vTexCoord;

    if(isHorizontal)
    {
        uv.y += pixelStep * blendFactor;
        // FragColor = vec4(1.0,0.0,0.0,1.0);
        // return ;
    }
    else
    {
        uv.x += pixelStep * blendFactor;
    }

    FragColor = texture(lightTexture,uv);
    // FragColor = vec4(abs(pixelStep) * blendFactor);
    FragColor.a = lumaCenter;
}

#endif
