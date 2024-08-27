## Linked List 解析

- 环境
```GLSL
- OpenGL4.5
- GLSL 450
```

### 实现思路
在渲染期间，记录像素的所有的中间片元信息(深度、颜色)，最后在渲染到屏幕的时候，将所有的片元信息按照深度进行混合，得到最终的颜色值。
将每一次记录的片元信息包装为一个结构体，并将其记录在链表中，链表的节点为结构体，结构体中包含下一个节点在链表中的索引。

### 资源

- `List` 记录所有像素的在不同深度渲染时候得到的片元的值，记录
    - 深度
    - 颜色
需要将同一个像素的所有偏远链接起来，因此需要记录下一个片元信息的索引
    - 下一个片元索引


```C++

/// 链表节点

struct FragmentNode
{
    glm::vec4 color;
    float depth;
    uint next;
};

```

---

### 实现
#### first
##### 正常渲染场景
- 使用`OpenGL`的`ShaderStorageBufferObject`记录链表 
    - `NodeList Array`

- 使用`OpenGL`的原子计数器生成每一个片元的的索引
    - `Counter gen N`
    - 初始化为`0`
    - 每一帧渲染之前都需要初始化

- 使用`OpenGL`的纹理记录每个像素对应链表的头
    - `HeadPointer`
    - 初始化为`0XFFFFFFFF`
    - 将`Counter`生成的索引(`N`)存储到当前的`HeadPointer`纹理中，将上一次记录的结果作为链表节点的`next`,记录到节点中。
    - 每一帧渲染之前都需要重新清空为`0XFFFFFFFF`

- 将所有像素的所有片元的节点存储到`ShaderStorageBufferObject`中
    - 使用`Counter`生成的`Index`作为`NodeList Array`的索引，存储节点到`NodeList Array`中

#### second
##### 合并渲染结果
- 绘制一个`Quad`, `Quad`的大小为屏幕的大小
- 从`HeadPointer`获取头节点的索引，并根据头节点获取当前像素所有记录的片元。
    - `FragNodes List`
    - 根据节点中记录的片元的深度对记录的`FragNodes List`进行排序

- 合并所有节点的color
    - front-back
    - back-front



