## 颜色混合方程

### 兼容混合半透明与不透明颜色

```glsl

vec4 blendColor(vec3 srcColor,vec3 dstColor,float srcAlpha,float dstAlpha)
{
    vec4 color;
    // color.rgb = dstColor * dstAlpha + (1-dstAlpha) * srcColor * srcAlpha;
    color.rgb = dstColor * dstAlpha + (1 - dstAlpha) * srcColor * srcAlpha; ///< 可以提前对 源颜色进行预乘 减少管线的输出

    color.a = srcAlpha - srcAlpha * dstAlpha + dstAlpha;
    /// color.a = srcAlpha *( 1.0 - dstAlpha) + dstAlpha
    color.rgb = color.rgb / color.a;
    
    return color;
}


```


