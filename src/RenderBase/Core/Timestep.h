#ifndef HAZEL_TIMESTEP_H_
#define HAZEL_TIMESTEP_H_

namespace OBase
{
    class Timestep
    {
    public:
        explicit Timestep(float time = 0.0f)
            : m_Time(time)
        {
        }

        explicit operator float() const { return m_Time; }
        [[nodiscard]] float GetSeconds() const { return m_Time; }
        [[maybe_unused]] [[nodiscard]] float GetMilliseconds() const { return m_Time * 1000.0f; }

    private:
        float m_Time = 0.0f;
    };
};

#endif //! HAZEL_TIMESTEP_H_
