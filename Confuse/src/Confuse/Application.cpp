#include "Application.h"

#include "Confuse/Events/ApplicationEvent.h"
#include "Confuse/Log.h"

namespace Confuse{
    Application::Application(){}
    Application::~Application(){}

    void Application::run(){
        WindowResizeEvent e(1600, 900);

        if(e.isInCategory(EventCategoryApplication))
            CE_TRACE(e);
        if(e.isInCategory(EventCategoryInput))
            CE_TRACE(e);

        while(true){}
    }
}