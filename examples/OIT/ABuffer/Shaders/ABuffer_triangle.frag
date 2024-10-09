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
    ivec2 fragCoord = ivec2(gl_FragCoord.xy);

    bool StayInLoop = true;
    while(StayInLoop)
    {
        if(LockSemaphore(fragCoord))
        {
            /// 判断是否打到最大层数
            uint Count = imageLoad( abufferCounter, fragCoord ).r;
            imageAtomicAdd(abufferCounter, fragCoord, 1u);

            vec4 abufferVal = vec4(uColor.rgb, gl_FragCoord.z);
            imageStore(abuffers, ivec3(fragCoord, Count), abufferVal);

            // FragColor = vec4(uColor.rgb, 1.0);
            FreeSemaphore(fragCoord);
            StayInLoop = false;
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
