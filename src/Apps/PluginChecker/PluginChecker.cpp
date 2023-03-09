/**
 * @brief A small application to check plugins for compatibility
 *
 * @param argc
 * @param argv
 *
 * Call by: PluginChecker <PluginPath> <PluginName>
 */
#include <string>
#include <iostream>
#include <map>
#include <filesystem>
#include <dlfcn.h>
#include <FIPP/PipelineElements/IGenericPipelineElement.hpp>
#include <FIPP/Logging/ILogging.hpp>
#include <FIPP/Point/Point.hpp>
#include <yaml-cpp/yaml.h>
using namespace std;
namespace fs = filesystem;

typedef void (*makePipeElement_t)(std::shared_ptr<FIPP::pipe::IGenericPipelineElement> & pipeElemPtr, YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);

void getAllPluginsInFolder(string path, map<string, makePipeElement_t*> &plugins)
{
    for (const auto &entry : fs::directory_iterator(path))
    {
        cout << entry.path().c_str() << endl;
        void *handle = dlopen(entry.path().c_str(), RTLD_NOW);
        cout << entry.path().c_str() << endl;
        if (!handle)
        {
            cerr << "Cannot open library: " << dlerror() << '\n';
            break;
        }
        typedef void (*getElementIdentifier_t)(std::string & name);
        getElementIdentifier_t getElementIdentifier = (getElementIdentifier_t)dlsym(handle, "getElementIdentifier");
        const char *dlsym_error = dlerror();
        if (dlsym_error)
        {
            cerr << "Cannot load symbol 'hello': " << dlsym_error << '\n';
            dlclose(handle);
            break;
        }
        std::string identifier;
        getElementIdentifier(identifier);
        cout << "Identifier: " << identifier << endl;
        
        makePipeElement_t makePipeElement = (makePipeElement_t)dlsym(handle, "makePipeElement");
        plugins[identifier] = &makePipeElement;
    }
}

int main(int argc, char *argv[])
{
    string path = string(argv[1]);
    string pluginName = string(argv[2]);
    map<string, makePipeElement_t*> plugins;
    //openDependencies();
    getAllPluginsInFolder(path, plugins);
    return 0;
}