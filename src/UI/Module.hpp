#pragma once


namespace Grog {
    class Application;

    class Module {
    public:
        Module() = default;
        virtual ~Module() = default;

        virtual void Initialize();
        virtual void Terminate();

        virtual void Draw();

    protected:
        Application* GetApplication();
    
    private:
        Application* app;

        friend class Application;
    };

}