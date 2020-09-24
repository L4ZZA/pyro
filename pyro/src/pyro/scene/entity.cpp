#include "pyro_pch.h"
#include "entity.h"


pyro::entity::entity(entt::entity handle, pyro::scene *scene)
	: m_entity_handle(handle), m_scene(scene)
{
}
