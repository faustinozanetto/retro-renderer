#include "rtpch.h"
#include "transform.h"

namespace retro::scene
{
	transform::transform(const glm::vec3 &location, const glm::quat &rotation, const glm::vec3 &scale)
	{
		m_location = location;
		m_rotation = rotation;
		m_scale = scale;
		m_transform_dirty = false;
		m_parent = nullptr;
		recalculate_transform();
	}

	transform::~transform()
	{
	}

	const glm::mat4 &transform::get_transform()
	{
		RT_PROFILE;
		if (!m_transform_dirty)
			return m_transform;

		recalculate_transform();
		return m_transform;
	}

	void transform::set_location(const glm::vec3 &location)
	{
		RT_PROFILE;
		m_location = location;
		m_transform_dirty = true;
	}

	void transform::set_rotation(const glm::quat &rotation)
	{
		RT_PROFILE;
		m_rotation = rotation;
		m_transform_dirty = true;
	}

	void transform::set_scale(const glm::vec3 &scale)
	{
		RT_PROFILE;
		m_scale = scale;
		m_transform_dirty = true;
	}

	void transform::recalculate_transform()
	{
		RT_PROFILE;
		if (m_parent)
			m_transform = m_parent->get_transform();
		else
			m_transform = glm::mat4(1.0f);

		m_transform = glm::translate(m_transform, m_location);
		m_transform *= glm::toMat4(m_rotation);
		m_transform = glm::scale(m_transform, m_scale);
	}
}