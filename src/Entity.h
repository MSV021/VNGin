#ifndef ENTITY_H
#define ENTITY_H

#include "Module.h"
#include "Transform.h"

#include <vector> 
#include <string> 
#include <algorithm>

namespace VNGin {
    class Scene;
    class Entity {
        friend class Scene;
    public: 
        Entity(Scene* scene, std::string name = "New Entity", Vector position = Vector::zero, double rotation = 0.0, Vector scale = Vector::one);
        Entity(const Entity&) = delete;
        Entity(Entity&&) = delete;

        template<typename T> 
        T* GetModule() {
            T* target;
            for(auto& m : modules) {
                if((target = dynamic_cast<T*>(m)) != nullptr) 
                    return target;
            }

            return nullptr;
        }

        template<typename T, typename ...Args> 
        T* AddModule(Args... args) {
            if(GetModule<T>() == nullptr) {
                T* newModule = new T(this, args...);
                modules.push_back(newModule);
                return newModule;
            }

            return nullptr;
        }

        template<typename T> 
        void RemoveModule() {
            auto target = GetModule<T>(); 
            if(target != nullptr) {
                auto iter = std::find(modules.begin(), modules.end(), target);
                modules.erase(iter);
                target->Destroy();
            }
        }

        void SwitchScene(Scene* newScene);
        Scene* GetScene() { return scene; }

        void Destroy(void);

        Transform* transform;
    private: 
        void UpdateModules(void);

        Scene* scene;
        std::string name;
        std::vector<Module*> modules;
    };
}

#endif