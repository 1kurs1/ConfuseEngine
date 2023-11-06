#include "editor/confuseEditor.hpp"

int main(){
    ConfuseGraphicsCore::Editor editor{};

    try{
        editor.startEditor();
    }catch(const std::exception &ex){
        std::cerr << ex.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}