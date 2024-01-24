#version 450 core
layout(location = 0) out vec4 FragColor;

in vec2 vTexCoord;
uniform sampler2D lightTexture;
uniform vec2 Size;

float EDGE_THRESHOLD_MIN = 0.031;
float EDGE_THRESHOLD_MAX = 0.125;

#define QUALITY(q) ((q) < 5 ? 1.0 : ((q) > 5 ? ((q) < 10 ? 2.0 : ((q) < 11 ? 4.0 : 8.0)) : 1.5))
#define ITERATIONS 12
#define SUBPIXEL_QUALITY 1.75

// #define SAMPLE_FXAA

float rgb2luma(vec3 rgb)
{
    return sqrt(dot(rgb, vec3(0.299, 0.587, 0.114)));
}

void main()
{
    vec4 centerColor = texture(lightTexture,vTexCoord);

    float lumaCenter = rgb2luma(centerColor.rgb);
    float lumaDown = rgb2luma(textureLodOffset(lightTexture,vTexCoord,0.0,ivec2(0,-1)).rgb);   ///< Down
    float lumaUp = rgb2luma(textureLodOffset(lightTexture,vTexCoord,0.0,ivec2(0,1)).rgb);      ///< Up
    float lumaLeft = rgb2luma(textureLodOffset(lightTexture,vTexCoord,0.0,ivec2(-1,0)).rgb);   ///< Left
    float lumaRight = rgb2luma(textureLodOffset(lightTexture,vTexCoord,0.0,ivec2(1,0)).rgb);   ///< Right

    /// 对比度
    float lumaMin = min(lumaCenter,min(min(lumaDown,lumaUp),min(lumaLeft,lumaRight)));
    float lumaMax = max(lumaCenter,max(max(lumaDown,lumaUp),max(lumaLeft,lumaRight)));

    // Compute the delta.  -- 对比度
    float lumaRange = lumaMax - lumaMin;

    if(lumaRange < max(EDGE_THRESHOLD_MIN,lumaMax * EDGE_THRESHOLD_MAX))
    {
        FragColor = vec4(0.0,1.0,0.0,1.0);
        FragColor = centerColor;
        return;
    }

    /// TODO: do FXAA
    float lumaDownLeft = rgb2luma(textureLodOffset(lightTexture,vTexCoord,0.0,ivec2(-1,-1)).rgb);
    float lumaUpRight = rgb2luma(textureLodOffset(lightTexture,vTexCoord,0.0,ivec2(1,1)).rgb);
    float lumaUpLeft = rgb2luma(textureLodOffset(lightTexture,vTexCoord,0.0,ivec2(-1,1)).rgb);
    float lumaDownRight = rgb2luma(textureLodOffset(lightTexture,vTexCoord,0.0,ivec2(1,-1)).rgb);

    float lumaDownUp = lumaDown + lumaUp;
    float lumaLeftRight = lumaLeft + lumaRight;

    float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
    float lumaDownCorners = lumaDownLeft + lumaDownRight;
    float lumaRightCorners = lumaDownRight + lumaUpRight;
    float lumaUpCorners = lumaUpRight + lumaUpLeft;

    // Compute an estimation of the gradient along the horizontal and vertical axis.
    /// 水平对比度
    float edgeHorizontal =  abs(-2.0 * lumaLeft + lumaLeftCorners)  + abs(-2.0 * lumaCenter + lumaDownUp ) * 2.0    + abs(-2.0 * lumaRight + lumaRightCorners);

    /// 垂直对比度
    float edgeVertical =    abs(-2.0 * lumaUp + lumaUpCorners)      + abs(-2.0 * lumaCenter + lumaLeftRight) * 2.0  + abs(-2.0 * lumaDown + lumaDownCorners);

    // Is the local edge horizontal or vertical ? 判断当前的边界是水平还是垂直的
    bool isHorizontal = (edgeHorizontal >= edgeVertical);

    /// 计算偏移的方向 在前面计算的水平 or 垂直的基础之上计算梯度变化大的方向 以边界构建水平轴 得到一个垂直水平轴的方向 我们规定水平向右为水平轴的方向 同理以垂直水平轴向上作为垂直轴的方向
    /// 当我们得到梯度变化方向(垂直轴的方向)之后 以此计算文素的偏移步长
    float luma1 = isHorizontal ? lumaDown : lumaLeft;
    float luma2 = isHorizontal ? lumaUp : lumaRight;
    // Compute gradients in this direction.
    float gradient1 = luma1 - lumaCenter;
    float gradient2 = luma2 - lumaCenter;
    bool is1Steepest = abs(gradient1) >= abs(gradient2);

    float gradientScaled = 0.25 * max(abs(gradient1),abs(gradient2));

    /// 纹素的偏移的步长 - 一个纹素的长度 (inverse Resolution)
    float stepLength = isHorizontal ? 1.0 / Size.y : 1.0 / Size.x;
    float lumaLocalAverage = 0.0;

    /// 朝这个方向移动半个像素，并计算此时的平均亮度。
    /// 平均亮度是指 沿着边界的梯度变化的方向上的像素(此像素邻近当前UV坐标采样的像素)和当前采样的像素的亮度的平均值

#ifdef SAMPLE_FXAA
    vec2 PixelStep = isHorizontal ? vec2(0, 1.0 / Size.y) : vec2(1.0 / Size.x, 0);
#endif

    if(is1Steepest)
    {
        // Switch the direction
        stepLength = - stepLength;
        lumaLocalAverage = 0.5*(luma1 + lumaCenter);
#ifdef SAMPLE_FXAA
        PixelStep = -PixelStep;
#endif
    }
    else
    {
        lumaLocalAverage = 0.5*(luma2 + lumaCenter);
    }

#ifdef SAMPLE_FXAA
    float Filter = 2 * (lumaUp + lumaDown + lumaRight + lumaLeft) + lumaDownLeft + lumaUpRight + lumaUpLeft + lumaDownRight;
    Filter = Filter / 12.0;
    // 计算出基于亮度的混合系数
    Filter = abs(Filter -  lumaCenter);
    Filter = clamp(Filter / lumaRange,0.0f,1.0f);
    // 使输出结果更加平滑
    float PixelBlend = smoothstep(0.0f, 1.0f, Filter);
    PixelBlend = PixelBlend * PixelBlend;
    vec3 cccc = textureLod(lightTexture, vTexCoord + PixelStep * PixelBlend, 0.0).rgb;
    FragColor.rgb = cccc;
    return ;
#endif

    /// 将UV坐标偏移半个像素 -- 沿着我们计算的方向
    vec2 currentUv = vTexCoord;
    if(isHorizontal)
    {
        currentUv.y += stepLength * 0.5;
    }
    else
    {
        currentUv.x += stepLength * 0.5;
    }


    vec2 offset = isHorizontal ? vec2(1.0 / Size.x,0.0) : vec2(0.0,1.0 / Size.y);

    vec2 uv1 = currentUv - offset * QUALITY(0);
    vec2 uv2 = currentUv + offset * QUALITY(0);

    float lumaEnd1 = rgb2luma(textureLod(lightTexture,uv1,0.0f).rgb);
    float lumaEnd2 = rgb2luma(textureLod(lightTexture,uv2,0.0f).rgb);

    lumaEnd1 -= lumaLocalAverage;
    lumaEnd2 -= lumaLocalAverage;

    bool reached1 = abs(lumaEnd1) >= gradientScaled;
    bool reached2 = abs(lumaEnd2) >= gradientScaled;
    bool reachedBoth = reached1 && reached2;

    if(!reached1){
        uv1 -= offset * QUALITY(1);
    }
    if(!reached2){
        uv2 += offset * QUALITY(1);
    }
    if(!reachedBoth)
    {
        for (int i = 2; i < ITERATIONS; i++){
            // If needed, read luma in 1st direction, compute delta.
            if (!reached1){
                lumaEnd1 = rgb2luma(textureLod(lightTexture, uv1, 0.0).rgb);
                lumaEnd1 = lumaEnd1 - lumaLocalAverage;
            }
            // If needed, read luma in opposite direction, compute delta.
            if (!reached2){
                lumaEnd2 = rgb2luma(textureLod(lightTexture, uv2, 0.0).rgb);
                lumaEnd2 = lumaEnd2 - lumaLocalAverage;
            }
            // If the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge.
            reached1 = abs(lumaEnd1) >= gradientScaled;
            reached2 = abs(lumaEnd2) >= gradientScaled;
            reachedBoth = reached1 && reached2;

            // If the side is not reached, we continue to explore in this direction, with a variable quality.
            if (!reached1){
                uv1 -= offset * QUALITY(i);
            }
            if (!reached2){
                uv2 += offset * QUALITY(i);
            }

            // If both sides have been reached, stop the exploration.
            if (reachedBoth){ break; }
        }
    }

    // Compute the distances to each side edge of the edge (!).
    float distance1 = isHorizontal ? (vTexCoord.x - uv1.x) : (vTexCoord.y - uv1.y);
    float distance2 = isHorizontal ? (uv2.x - vTexCoord.x) : (uv2.y - vTexCoord.y);

    // In which direction is the side of the edge closer ?
    bool isDirection1 = distance1 < distance2;
    float distanceFinal = min(distance1, distance2);

    // Thickness of the edge.
    float edgeThickness = (distance1 + distance2);

    // Is the luma at center smaller than the local average ?
    bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

    // If the luma at center is smaller than at its neighbour, the delta luma at each end should be positive (same variation).
    bool correctVariation1 = (lumaEnd1 < 0.0) != isLumaCenterSmaller;
    bool correctVariation2 = (lumaEnd2 < 0.0) != isLumaCenterSmaller;

    // Only keep the result in the direction of the closer side of the edge.
    bool correctVariation = isDirection1 ? correctVariation1 : correctVariation2;

    // UV offset: read in the direction of the closest side of the edge.
    float pixelOffset = - distanceFinal / edgeThickness + 0.5;

    // If the luma variation is incorrect, do not offset.
    float finalOffset = correctVariation ? pixelOffset : 0.0;

    // Sub-pixel shifting
    // Full weighted average of the luma over the 3x3 neighborhood.
    float lumaAverage = (1.0/12.0) * (2.0 * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);
    // Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood.
    float subPixelOffset1 = clamp(abs(lumaAverage - lumaCenter)/lumaRange,0.0,1.0);
    float subPixelOffset2 = (-2.0 * subPixelOffset1 + 3.0) * subPixelOffset1 * subPixelOffset1;
    // Compute a sub-pixel offset based on this delta.
    float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

    // Pick the biggest of the two offsets.
    finalOffset = max(finalOffset,subPixelOffsetFinal);
    // Compute the final UV coordinates.
    vec2 finalUv = vTexCoord;
    if(isHorizontal){
        finalUv.y += finalOffset * stepLength;
    } else {
        finalUv.x += finalOffset * stepLength;
    }

    // Read the color at the new UV coordinates, and use it.
    vec3 finalColor = textureLod(lightTexture, finalUv, 0.0).rgb;
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