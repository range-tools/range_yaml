#ifndef NOTES_H_
#define NOTES_H_

// some scratch space

// #define debug
#ifdef debug
switch (event.type) {
case YAML_STREAM_START_EVENT:
  std::cout << "YAML_STREAM_START_EVENT\n";
  break;
case YAML_STREAM_END_EVENT:
  std::cout << "YAML_STREAM_END_EVENT\n";
  break;
case YAML_DOCUMENT_START_EVENT:
  std::cout << "YAML_DOCUMENT_START_EVENT\n";
  break;
case YAML_DOCUMENT_END_EVENT:
  std::cout << "YAML_DOCUMENT_END_EVENT\n";
  break;
case YAML_MAPPING_START_EVENT:
  ++mapping_level;
  std::cout << "YAML_MAPPING_START_EVENT mapping_level=" << mapping_level
            << '\n';
  break;
case YAML_MAPPING_END_EVENT:
  --mapping_level;
  std::cout << "YAML_MAPPING_END_EVENT mapping_level=" << mapping_level << '\n';
  break;
case YAML_SEQUENCE_START_EVENT:
  std::cout << "YAML_SEQUENCE_START_EVENT\n";
  break;
case YAML_SEQUENCE_END_EVENT:
  std::cout << "YAML_SEQUENCE_END_EVENT\n";
  break;
case YAML_SCALAR_EVENT: {
  const char *val = (const char *)event.data.scalar.value;
  std::cout << "YAML_SCALAR_EVENT: " << val << '\n';
} break;
default:
  std::cout << event.type << '\n';
}
#endif

#endif // NOTES_H_
