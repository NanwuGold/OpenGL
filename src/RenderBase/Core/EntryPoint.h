#ifndef OBASE_ENTRYPOINT_H
#define OBASE_ENTRYPOINT_H

extern std::unique_ptr<OBase::Application> OBase::CreateApplication();

int main(int argc, char** argv)
{
    auto app = OBase::CreateApplication();
    app->Run();

    app.reset(nullptr);
    return 0;
}

#endif