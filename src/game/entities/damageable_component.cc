#include <functional>
#include <boost/foreach.hpp>

#include <framework/framework.h>
#include <framework/graphics.h>

#include <game/entities/entity_factory.h>
#include <game/entities/entity_attribute.h>
#include <game/entities/entity_manager.h>
#include <game/entities/damageable_component.h>
#include <game/entities/position_component.h>

namespace ent {

using namespace std::placeholders;

// register the damageable component with the entity_factory
ENT_COMPONENT_REGISTER("Damageable", damageable_component);

damageable_component::damageable_component() {
}

damageable_component::~damageable_component() {
}

void damageable_component::apply_template(luabind::object const &tmpl) {
  for (luabind::iterator it(tmpl), end; it != end; ++it) {
    if (it.key() == "Explosion") {
      _expl_name = luabind::object_cast<std::string>(*it);
    }
  }
}

void damageable_component::initialize() {
  std::shared_ptr<entity> entity(_entity);
  entity_attribute *health = entity->get_attribute("health");
  if (health != nullptr) {
    health->sig_value_changed.connect(std::bind(&damageable_component::check_explode, this, _2));
  }
}

void damageable_component::apply_damage(float amt) {
  std::shared_ptr<entity> entity(_entity);
  entity_attribute *attr = entity->get_attribute("health");
  if (attr != nullptr) {
    float curr_value = attr->get_value<float>();
    if (curr_value > 0) {
      attr->set_value(curr_value - amt);
    }
  }
}

// this is called whenever our health attribute changes value. we check whether it's
// hit 0, and explode if it has
void damageable_component::check_explode(boost::any health_value) {
  if (boost::any_cast<float>(health_value) <= 0) {
    explode();
  }
}

void apply_damage(std::shared_ptr<entity> ent, float amt) {
  damageable_component *damageable = ent->get_component<damageable_component>();
  if (damageable != 0) {
    damageable->apply_damage(amt);
  }
}

void damageable_component::explode() {
  std::shared_ptr<entity> entity(_entity); // entity is always valid while we're valid...
  entity->get_manager()->destroy(_entity);

  if (_expl_name != "") {
    // create an explosion entity
    entity->get_manager()->create_entity(entity, _expl_name, 0);
  }

  position_component *our_position = entity->get_component<position_component>();
  if (our_position != nullptr) {
    std::list<std::weak_ptr<ent::entity>> entities;
    our_position->get_entities_within_radius(5.0f, std::back_inserter(entities));
    BOOST_FOREACH(std::weak_ptr<ent::entity> const &wp, entities) {
      std::shared_ptr<ent::entity> ent = wp.lock();

      // don't damange ourselves...
      if (!ent || ent == entity)
        continue;

      ent::apply_damage(ent, (5.0f - our_position->get_direction_to(ent).length()));
    }
  }
}

}
