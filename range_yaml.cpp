#include "range_yaml.h"

#include <iostream> // debugging

static void pop_node_stack(std::vector<RangeYaml::Node> &node_stack) {
  if (node_stack.empty())
    return;

  RangeYaml::Node &n{node_stack.back()};
  if (n.nested) {
    delete n.nested;
  }
  node_stack.pop_back();
}

void RangeYaml::State::next_node() {
  // TODO: Here we should do different things depending on wheter the thing
  // on top of the stack is a mapping or a sequence, the following code already
  // handles mappings properly, so we just need to handle sequences
  bool node_constructed{false};
  bool key_seen{false};
  while (!node_constructed && !done) {
    next_event();
    switch (event.type) {
    case (YAML_SCALAR_EVENT): {
      const char *scalar_value = (const char *)event.data.scalar.value;
      if (key_seen) {
        Node &n{node_stack.back()};
        key_seen = false; // next key wasn't seen yet
        n.type = SCALAR;
        n.scalar = scalar_value;

        node_constructed = true;
      } else {
        Node &n{node_stack.back()};

        n.key = scalar_value;
        key_seen = true;
      }
    } break;
    case (YAML_MAPPING_START_EVENT):
      if (key_seen) {
        key_seen = false;
        // modify first, query later to avoid dangling reference
        Node &n{node_stack.emplace_back()};
        const Node &pn{node_stack[node_stack.size() - 2]};

        n.type = MAPPING;
        n.key = pn.key;
        const int depth = (pn.nested) ? pn.nested->depth + 1 : 1;
        n.nested = new RangeYaml(this, depth);
        node_constructed = true;
      }
      break;
    case (YAML_MAPPING_END_EVENT):
      pop_node_stack(node_stack);
      break;
    default:
      break;
    }
  }
}

void RangeYaml::State::clean_event() {
  if (should_clean_event) {
    yaml_event_delete(&event);
    should_clean_event = false;
  }
}

void RangeYaml::State::next_event() {
  if (done)
    return;

  clean_event();

  int ok{yaml_parser_parse(&parser, &event)};
  if (!ok) {
    std::cout << "ERROR: RangeYaml parsing failed at line " << line + 1 << '\n';
    // std::cout << "YAML PARSER ERROR at line " << event.start_mark.line <<
    // '\n';
  } else {
    should_clean_event = true;
    line = event.end_mark.line;
  }

  if (event.type == YAML_STREAM_END_EVENT || !ok) {
    done = true;
    return;
  }
}
