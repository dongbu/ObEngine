#pragma once

#include <memory>
#include <vector>

#include <Script/GlobalState.hpp>
#include <Types/Identifiable.hpp>
#include <Types/Serializable.hpp>

namespace obe::Component
{
    class ComponentBase : public Types::Identifiable, public Types::Serializable
    {
    protected:
		static std::vector<ComponentBase*> Components;
		static void AddComponent(ComponentBase* component);
		static void RemoveComponent(ComponentBase* component);
    public:
		ComponentBase(const std::string& id);
		~ComponentBase() override = default;
        virtual void inject(unsigned int envIndex) = 0;
		virtual void remove() = 0;
	   
	    void dump(vili::ComplexNode& target) const override = 0;
	    void load(vili::ComplexNode& data) override = 0;
    };

    template <class T>
    class Component : public ComponentBase
    {
    public:
	    explicit Component(const std::string& id);
		~Component() override = default;

        static std::vector<std::unique_ptr<T>> Pool;
        static T& create(const std::string& id);
		
		void remove() override;
        void inject(unsigned int envIndex) override;
	    
	    void dump(vili::ComplexNode& target) const override = 0;
	    void load(vili::ComplexNode& data) override = 0;
    };

	template <class T>
	Component<T>::Component(const std::string& id) : ComponentBase(id)
	{
	}

	template<class T>
    inline T& Component<T>::create(const std::string& id)
    {
		T* ref = Pool.emplace_back(std::make_unique<T>(id)).get();
		AddComponent(ref);
		return *ref;
    }

	template <class T>
	void Component<T>::inject(unsigned int envIndex)
    {
	    Script::ScriptEngine["__ENVIRONMENTS"][envIndex]["Components"][m_id] = static_cast<T*>(this);
    }

	template <class T>
	void Component<T>::remove()
	{
		RemoveComponent(this);
		T::Pool.erase(std::remove_if(T::Pool.begin(), T::Pool.end(), [&](auto& elem) { return (this == elem.get()); }), T::Pool.end());
	}

	template <class T>
	std::vector<std::unique_ptr<T>> Component<T>::Pool;
}