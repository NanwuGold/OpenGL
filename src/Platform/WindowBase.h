#ifndef RENDERBASE_WINDOW_BASE_H_
#define RENDERBASE_WINDOW_BASE_H_

#include <iostream>


namespace OBase
{
    struct WindowPro
    {
        WindowPro(int w,int h,std::string wt)
        :m_w(w),m_h(h),m_title(std::move(wt))
        {}

        std::string m_title;
        int m_w = 800;
        int m_h = 600;

        int width(){return m_w;}
        int height(){return m_h;}

        std::string& title(){return m_title;}
    };
}

#endif

