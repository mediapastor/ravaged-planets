#include <functional>
#include <memory>

#include <framework/packet_buffer.h>
#include <framework/exception.h>

#include <game/simulation/orders.h>
#include <game/entities/builder_component.h>
#include <game/entities/orderable_component.h>
#include <game/entities/moveable_component.h>
#include <game/entities/position_component.h>
#include <game/entities/weapon_component.h>
#include <game/entities/pathing_component.h>

namespace game {

//-------------------------------------------------------------------------
// this is a helper macro for registering order types with the order_factory
#define ORDER_REGISTER(type) \
  std::shared_ptr<order> create_ ## type () { return std::shared_ptr<order>(new type()); } \
  order_registrar reg_ ## type(type::identifier, &create_ ## type)

typedef std::function<std::shared_ptr<order>()> create_order_fn;
typedef std::map<uint16_t, create_order_fn> order_registry_map;
static order_registry_map *g_order_registry = nullptr;

// this is a helper class that you use indirectly via the ORDER_REGISTER macro
// to register an order.
class order_registrar {
public:
  order_registrar(uint16_t id, create_order_fn fn);
};

order_registrar::order_registrar(uint16_t id, create_order_fn fn) {
  if (g_order_registry == 0)
    g_order_registry = new order_registry_map();

  (*g_order_registry)[id] = fn;
}

// creates the order object from the given order identifier
std::shared_ptr<order> create_order(uint16_t id) {
  create_order_fn fn = (*g_order_registry)[id];
  if (fn == nullptr) {
    BOOST_THROW_EXCEPTION(
        fw::exception() << fw::message_error_info("order does not exist: " + boost::lexical_cast<std::string>(id)));
  }

  return fn();
}

//-------------------------------------------------------------------------
ORDER_REGISTER(build_order);
ORDER_REGISTER(move_order);

//-------------------------------------------------------------------------

order::order(std::string const &state_name) :
    _state_name(state_name) {
}

order::~order() {
}

void order::begin(std::weak_ptr<ent::entity> const &ent) {
  _entity = ent;
}

void order::serialize(fw::net::packet_buffer &) {
}

void order::deserialize(fw::net::packet_buffer &) {
}

//-------------------------------------------------------------------------

build_order::build_order() :
    order("building"), _builder(0) {
}

build_order::~build_order() {
}

void build_order::begin(std::weak_ptr<ent::entity> const &ent) {
  order::begin(ent);

  std::shared_ptr<ent::entity> entity(_entity);
  _builder = entity->get_component<ent::builder_component>();
  if (_builder != nullptr) {
    _builder->build(template_name);
  }
}

bool build_order::is_complete() {
  if (_builder == 0)
    return true;

  return !_builder->is_building();
}

void build_order::serialize(fw::net::packet_buffer &buffer) {
  buffer << template_name;
}

void build_order::deserialize(fw::net::packet_buffer &buffer) {
  buffer >> template_name;
}

//-------------------------------------------------------------------------

move_order::move_order() :
    order("moving") {
}

move_order::~move_order() {
}

void move_order::begin(std::weak_ptr<ent::entity> const &ent) {
  order::begin(ent);
  std::shared_ptr<ent::entity> entity(_entity);

  // if we've got a pathing component, use that to move towards the target
  auto pathing = entity->get_component<ent::pathing_component>();
  if (pathing != nullptr) {
    pathing->set_goal(goal);
  } else {
    // if, instead, we've got a moveable component, move towards the target
    auto moveable = entity->get_component<ent::moveable_component>();
    if (moveable != 0) {
      moveable->set_goal(goal);
    }
  }

  // if it's got a weapon, clear the target (since we've now moving instead)
  ent::weapon_component *weapon = entity->get_component<ent::weapon_component>();
  if (weapon != 0) {
    weapon->clear_target();
  }
}

bool move_order::is_complete() {
  std::shared_ptr<ent::entity> entity = _entity.lock();
  if (entity) {
    ent::pathing_component *pathing = entity->get_component<ent::pathing_component>();
    if (pathing != nullptr) {
      return !pathing->is_following_path();
    }

    ent::position_component *pos = entity->get_component<ent::position_component>();
    return (pos->get_direction_to(goal).length_squared() < 1.1f);
  }

  return true;
}

void move_order::serialize(fw::net::packet_buffer &buffer) {
  buffer << goal;
}

void move_order::deserialize(fw::net::packet_buffer &buffer) {
  buffer >> goal;
}

}
