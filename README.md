# RangeYaml

C++ range for loop interface for yaml files.

Example usage:

```c++
    RangeYaml yaml(yaml_string);
    
    for (const RangeYaml::Node *n : *yaml) {
    switch (n->type) {
    case (RangeYaml::SCALAR):
      std::cout << n->type << "  ";
      std::cout << n->key << ": " << n->scalar << ' ' << '\n';
      break;
    case (RangeYaml::MAPPING):
      std::cout << n->type << "  ";
      std::cout << n->key << ":\n";
      break;
    }
  }
```

See `tests/parse_and_print.cpp` for a full example.

## Notes/TODO:
- Support for yaml sequences (i.e. lists) is WIP, so don't expect those to work,
  but even then this is still useful for simple nested key-value yaml files.

License: MIT
