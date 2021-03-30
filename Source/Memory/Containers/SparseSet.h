#pragma once

#include <vector>
#include <exception>

namespace LCN
{
	using Entity = size_t;

	template<class Component>
	class SparseSet
	{
	public:
		using Iterator = typename std::vector<Component>::iterator;

		bool HasComponent(Entity entity)
		{
			return m_Sparse[entity] >= 0;
		}

		template<class ... Args>
		Component& Emplace(Entity entity, Args&& ... args)
		{
			m_Components.emplace_back(std::forward<Args>(args)...);
			m_Dense.emplace_back(entity);

			if (entity > m_Sparse.size())
				m_Sparse.resize(entity + 1, -1);

			m_Sparse[entity] = m_Dense.size() - 1;

			return m_Components.back();
		}

		Component& GetComponent(Entity entity)
		{
			if (!this->HasComponent(entity))
				throw std::exception("Component does not exist.");

			return m_Components[m_Sparse[entity]];
		}

		void RemoveComponent(Entity entity)
		{
			if (!this->HasComponent(entity))
				throw std::exception("Component does not exist.");

			int cmpIdxToRemove = m_Sparse[entity];
			m_Sparse[entity] = -1;

			m_Components[cmpIdxToRemove].~Component();
			m_Components[cmpIdxToRemove] = std::move(m_Components.back());
			m_Components.pop_back();

			m_Dense[cmpIdxToRemove] = m_Dense.back();
			m_Dense.pop_back();

			m_Sparse[m_Dense[cmpIdxToRemove]] = cmpIdxToRemove;
		}

		Iterator begin() { return m_Components.begin(); }
		Iterator end()   { return m_Components.end(); }

	private:
		std::vector<Component> m_Components;
		std::vector<Entity>    m_Dense;
		std::vector<int>       m_Sparse;
	};
}