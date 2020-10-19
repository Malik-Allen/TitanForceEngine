#ifndef SYSTEM_H
#define SYSTEM_H

#include "ISystem.h"

#include <tuple>
#include <vector>

namespace ECS {

	class EntityManager;

	template <typename... Comps>
	class System : public ISystem
	{
		using ComponentTuple = std::tuple <Comps* ... > ;

	protected:

		std::vector<ComponentTuple> m_components;

	public:

		System(EntityManager* entityComponent) : ISystem(entityComponent) {}
		virtual ~System() {}

		

	};


}


#endif // !SYSTEM_H








