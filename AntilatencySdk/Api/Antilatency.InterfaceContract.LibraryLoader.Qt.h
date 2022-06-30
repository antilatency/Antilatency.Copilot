#pragma once

#include <Antilatency.Api.h>
#include <QLibrary>

namespace Antilatency {
    namespace InterfaceContract {
        template<typename LibraryInterface>
        class LibraryLoader : public Antilatency::InterfaceContract::Library<LibraryInterface> {
        public:
            LibraryLoader(const char* libraryName){
                _library.setFileName(libraryName);
                _library.load();
            }
            typename Library<LibraryInterface>::LibraryEntryPoint getEntryPoint() override{
                if(_library.isLoaded()){
                    return reinterpret_cast<typename Library<LibraryInterface>::LibraryEntryPoint>(_library.resolve("getLibraryInterface"));
                }
                return nullptr;
            }
            bool isLoaded() {
                return _library.isLoaded();
            }
        protected:
            void unloadLibraryImpl() override{

            }
        private:
            QLibrary _library;
        };
    }
}
