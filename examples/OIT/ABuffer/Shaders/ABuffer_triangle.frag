#version 450 core

layout(binding = 1, r32ui) coherent uniform uimage2D abufferCounter;
layout(binding = 2, rgba32f) coherent uniform image2DArray abuffers;

/// 写数据时候的锁 保证写入的时候的安全
layout(binding = 3, r32ui) coherent uniform uimage2D writeSemphores;

out vec4 FragColor;
uniform vec4 uColor;

bool LockSemaphore(ivec2 coord);
void FreeSemaphore(ivec2 coord);

void main()
{
    ivec2 pixel = ivec2(gl_FragCoord.xy);

    bool StayInLoop = true;
    while(StayInLoop)
    {
        if(LockSemaphore(pixel))
        {
            ivec2 fragCoord = ivec2(gl_FragCoord.xy);

            /// 获取当前像素位置存在的层 -> 判断是否打到最大层数
            /// TODO: 对新增的颜色层处理 判断是否达到最大层数
            uint Count = imageLoad( abufferCounter, fragCoord ).r;
            // imageAtomicAdd(abufferCounter, fragCoord, 1u);

            /// 获取结果 判断深度 手动深度测试
            float colorZipValue = uintBitsToFloat(packUnorm4x8(uColor));

            vec4 abufferVal = vec4(colorZipValue, gl_FragCoord.z, 0.0 ,1.0);
            imageStore(abuffers, ivec3(fragCoord, Count), abufferVal);

            /// A-Buffer的第Count层被记录， 将记录的层数+1
            imageStore( abufferCounter, fragCoord, uvec4( Count + 1 ) );

            /// 释放锁
            FreeSemaphore(pixel);
            StayInLoop = false;  /// 结束循环
        }
    }

}

bool LockSemaphore(ivec2 coord)
{

    if(imageLoad(writeSemphores, coord).r == 1)
    {
        return false;
    }

    return imageAtomicExchange(writeSemphores, coord, 1) == 0;
}


void FreeSemaphore(ivec2 coord)
{
    imageStore(writeSemphores, coord, uvec4(0));
}
