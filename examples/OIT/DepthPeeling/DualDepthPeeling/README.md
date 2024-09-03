##  1. Dual Depth Peeling

- `dual Depth Peeling`方法是`Depth Peeling`方法的升级，由原来的每次剥离单层，升级为每次剥离双层
  - 单层的深度剥离，每次剥离最前的一层，然后通过`front to back`的方式依次合并剥离的结果(也可以每次交换剥离交换合并)。
  - `dual Depth Peeling` 每次剥离两层，最前面的层选择从前向后合并，最后面的层，每次剥离完成之后使用一个新的`pass`进行合并。



## 2. Dual Depth Peeling 实现

### 2.1 资源初始化

- 每次剥离前后两层，同时记录剥离的深度范围(只有当前层的深度在这个范围之内才可以进行剥离)
  - `Color Texture * 2` 每次剥离使用记录剥离的层的颜色 - `RGBA8`
  - `Depth Texture * 1` 记录下一次可用的深度范围 - `RG32F`
  - 第一次渲染将记录深度的纹理清空为`vec4(-1,-1,0,0)`
  - 第一次渲染将记录颜色的纹理清空为`vec4(0,0,0,0)`

- 使用`6`张纹理 每次使用`3`张纹理交替上述的剥离过程，将所有的纹理绑定到同一个`frame buffer`通过`glDrawBuffers` 切换绘制的附件

- 每次剥离前面的层由于使用的是`front to back`的合并方式，所以可以在每次的`dual depth peeling`剥离过程中将剥离的前面的层合并到某一个颜色附件上，剥离的最后面的层则必须在一个新的`pass`中进行合并(`back to front`)
  - 额外需要一个新的`framebuffer` - `backBlenderFramebuffer`
  - 额外的`texture` - `RGBA8`
    - <font color=red>此纹理初始化为背景色</font>
    - 每次剥离后，将剥离的纹理渲染到这个纹理上(`back to front`)

- 最后将 上述的`7`张纹理做为颜色附件绑定到第一张`framebuffer - peeling FramBuffer`
- 第七张纹理绑定到额外的`framebuffer - backBlenderFramebuffer`

- 在记录剥离层的深度的时候，我们使用纹理的`R,G`通道，`glBlendEquation` 设置为`GL_MAX`。取更大的像素覆盖。

### 2.2 渲染过程分析

#### 2.2.1 渲染初始化

- 关闭深度测试，开启颜色混合，设置混合方程为`GL_MAX`
- `bind` `backBlenderFramebuffer`帧缓冲，清空第一次渲染使用的纹理的颜色
  - `color`  `vec4(0,0,0,0)`
  - `depth` `vec4(-1,-1,0,0)`

#### 2.2.2 第一次绘制

- 绑定到`glDrawBuffer` - `GL_COLOR_ATTACHMENT0`,渲染一次场景。

  - 获得场景的最外层两层的深度 - 这两层对应的是接下来要剥离的两层对应的深度

    ```GLSL
    #version 330 core
    out vec4 FragColor;
    
    void main()
    {
        FragColor = vec4(-gl_FragCoord.z, gl_FragCoord.z, 0.0, 0.0);
    }
    // gl_FragCoord.z 的范围是从 [0.0, 1.0] 
    // FragColor的R分量会纪录(最大的负深度，一定是最前面一层的深度)
    // FragColor的G分量会纪录(最大的深度，一定是最远的深度)
    
    // 此处我们得到下一次剥离两侧对应的深度
    
    ```

- 只有深度等于我们上一步获取的深度的时候，我们才会在下一次剥离

#### 2.2.3 剥离

##### 2.2.3.1 剥离并合并最近层

```C++
        int outFrontIndex = 0;
        for (int pass = 1; pass < g_numPasses; pass++)
        {
            /// 一共六个颜色缓冲 每次使用三个颜色缓冲分别记录当前被剥离层的深度 以及前后被剥离的颜色
            const auto currentIndex = pass % 2; ///< 获取当前后方向被剥离的结果
            const auto prevIndex = 1 - currentIndex; ///< 获取前一次记录的深度信息 在剥离的时候使用
            const auto bufferIndex = currentIndex * 3; ///< 记录当前使用的颜色缓冲的起点 往后数三则为当前次剥离使用的颜色缓冲
            outFrontIndex = currentIndex;
            /// 清空缓存
            glDrawBuffers(static_cast<GLsizei>(m_Buffers.size()), m_Buffers.data());
            glClearBufferfv(GL_COLOR, bufferIndex, glm::value_ptr(m_DepthClearValue));
            glClearBufferfv(GL_COLOR, bufferIndex + 1, glm::value_ptr(m_ColorClearBuffer));
            glClearBufferfv(GL_COLOR, bufferIndex + 2, glm::value_ptr(m_ColorClearBuffer));
            
         /// ......   
        }
```

- 前面我们通过一个`pass`获得被剥离的最远层和最近层的深度，

- 绑定到新的`drawBuffer上`

  - 清空纹理
  - 设置混合函数`GL_MAX`
  - 将记录上一次剥离的最近层的颜色纹理，做位采样器传递到剥离着色器中。

  - 将上一步剥离得到的最远最近层的深度纹理做位采样器传递到当前的剥离着色器中，渲染场景，开始剥离，记录在剥离深度纹理上对应最远最近层的颜色。

```GLSL
#version 330 core
layout(location = 0) out vec2 fDepth;
layout(location = 1) out vec4 FragColor1;   /// 输出最近层的颜色
layout(location = 2) out vec4 FragColor2;   /// 输出最远层的颜色

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

    /// 获取因该被剥离层的深度
    vec2 depthRange = texture(DepthTexture,TexCoord).xy;
    
    /// 最近层的颜色
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

    /// 当前层应该被剥离
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
```

##### 2.2.3.2 合并剥离的最远层

- `FragColor2`输出了被剥离的最远的层的颜色。
- 在新的`pass`将输出的最远层结果`FragColor2`合并。

```C++
glBlendEquation(GL_FUNC_ADD);
glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
```

#### 2.2.4 遮挡查询

```C++
glEndQuery(GL_SAMPLES_PASSED);
GLuint sampleCount;
glGetQueryObjectuiv(g_queryId,GL_QUERY_RESULT,&sampleCount);
if(!sampleCount)
{
    break;
}
```

- 判单是否剥离完成
- 当剥离的最远层为空白纹理的时候，`sampleCount`将会得到一个空的结果



#### 2.2.4 合并结果

- 合并累计的结果
- 将累计的最远层和最近层合并(`back  to front`)
