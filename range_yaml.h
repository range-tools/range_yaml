#ifndef RANGE_YAML_H_
#define RANGE_YAML_H_

#include <string>
#include <string_view>
#include <vector>
#include <yaml.h>

class RangeYaml {
  class State;

public:
  RangeYaml(const char *data) : top_level{true}, state{}, depth{0} {
    state = new State(data);
  }
  RangeYaml(State *state, int depth)
      : top_level{false}, state{state}, depth{depth} {}
  ~RangeYaml() {
    if (top_level)
      delete state;
  }

  // SEQ_* types appear within a yaml sequence, and others appear within yaml
  // mapping SEQ_* types differ from MAP_* types in that they don't have keys,
  // only values (scalar or nested).
  // TODO: add SEQUENCE type
  enum ValueType { SCALAR, MAPPING, SEQUENCE };

  // in_sequence=true means that keys have no names, otherwise keys have names
  struct Node {
    ValueType type;
    bool in_sequence; // are we inside a sequence or mapping
    // TODO: use std::string here to make it possible to free yaml events
    std::string key;

    // possible values
    std::string scalar;
    RangeYaml *nested{nullptr};
  };

  class Iterator {
  public:
    // iterator traits
    using difference_type = int;
    using value_type = const Node;
    using pointer = const Node *;
    using reference = const Node &;
    using iterator_category = std::forward_iterator_tag;

    Iterator(State &state, bool done, int depth)
        : state{state}, done{done}, depth{depth} {}

    Iterator &operator++() {
      // if coming back from deeper level, we don't need to step
      // because deeper level stepped for us
      if (state.coming_from_deeper_level)
        state.coming_from_deeper_level = false;
      else
        state.next_node();

      while (true) {
        if (state.done)
          break;
        // skip all deep nodes

        Node &n{state.node_stack.back()};
        if (n.type == SCALAR && state.node_stack.size() > depth + 1 ||
            state.node_stack.size() > depth + 2)
          state.next_node();
        else {
          break;
        }
      }
      Node &n{state.node_stack.back()};
      done = state.done || (state.node_stack.size() < depth + 1) ||
             (state.node_stack.size() < depth + 2 && n.type == MAPPING);

      if (done)
        state.coming_from_deeper_level = true;

      return *this;
    }
    Iterator operator++(int) {
      Iterator retval = *this;
      ++(*this);
      return retval;
    }
    bool operator==(Iterator other) const { return done == other.done; }
    bool operator!=(Iterator other) const { return !(*this == other); }

    const Node *operator*() { return &state.node_stack.back(); }

  private:
    State &state;
    bool done;
    const int depth;
  };

  Iterator begin() {
    // before entering nested level, step to the first element of nested layer
    if (!top_level)
      state->next_node();
    return {*state, false, depth};
  }
  Iterator end() { return {*state, true, depth}; }

private:
  class State {
    friend class RangeYaml;
    friend class RangeYaml::Iterator;

  public:
    State(const char *data) {
      yaml_parser_initialize(&parser);
      yaml_parser_set_input_string(&parser, (const unsigned char *)data,
                                   strlen(data));

      // initial node will be used if next even doesn't require the stack to
      // grow
      node_stack.emplace_back();

      next_event(); // initialize
      next_event(); // DOCUMENT_START_EVENT
      next_event(); // MAPPING_START_EVENT or SEQUENCE_START_EVENT
      next_node();
    }
    ~State() {
      clean_event();
      yaml_parser_delete(&parser);
    }

  private:
    yaml_parser_t parser;
    yaml_event_t event;
    int line{0};
    bool should_clean_event{false};
    bool done{false};

    std::vector<Node> node_stack;
    bool coming_from_deeper_level{false};

    void next_node();

    void clean_event();
    void next_event();
  };

  const bool top_level;
  State *state;

  // yaml fields deepter than depth will be skipped by this
  // RangeYaml object. Use deeper RangeYaml objects to access deeper yaml
  // layers.
  const int depth;
};

#endif // RANGE_YAML__
