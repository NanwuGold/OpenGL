#ifndef CSM_H_
#define CSM_H_

#include <RenderBase/Core/OGLBase.h>

namespace OBase
{
    class CSM : public Application
    {
    public:
        CSM()
            :Application("CSM Window")
        {
        }

    };

    std::unique_ptr<Application> CreateApplication()
    {
        return std::make_unique<CSM>();
    }
}




#endif // !CSM_H_
