#include <gmock/gmock.h>

#include <uccontract.h>
#include <ucontainer.h>
#include <ucio.h>
#include <buffer.h>

#include <string>

std::string streamContainer(JAD::UniversalContainer& container);
std::string streamContainer(JAD::UniversalContainer& container) {
  std::stringstream buf;
  if (container.get_type() == JAD::uc_Array) {
    buf << "{";
    for (JAD::UniversalArray::iterator it = container.vector_begin(); it != container.vector_begin(); ++it) {
      buf << streamContainer(*it);
    }
    buf << "}";
  } else if (container.get_type() == JAD::uc_Boolean) {
    buf << static_cast<bool>(container) << ", ";
  } else if (container.get_type() == JAD::uc_Character) {
    buf << static_cast<char>(container) << ", ";
  } else if (container.get_type() == JAD::uc_Integer) {
    buf << static_cast<int>(container) << ", ";
  } else if (container.get_type() == JAD::uc_Map) {
    buf << "{";
    for (JAD::UniversalMap::iterator it = container.map_begin(); it != container.map_end(); ++it) {
      buf << it->first << ":";
      buf << streamContainer(it->second);
    }
    buf << "}";
  } else if (container.get_type() == JAD::uc_Null) {
    buf << "null, ";
  } else if (container.get_type() == JAD::uc_Real) {
    buf << static_cast<double>(container) << ", ";
  } else if (container.get_type() == JAD::uc_String || container.get_type() == JAD::uc_WString) {
    buf << static_cast<std::string>(container) << ", ";
  } else {
    buf << "<unknown, ";
  }

  return buf.str();
};

std::runtime_error toStdException(JAD::UniversalContainer& exception);
std::runtime_error toStdException(JAD::UniversalContainer& exception){
  std::stringstream buf;
  try {
    buf << "From JAD::UniversalContainer exception: ";
    buf << streamContainer(exception);

  } catch (JAD::UniversalContainer& ) {
    return std::runtime_error("JAD::UniversalContainer exception thrown while handling JAD::UniversalContainer exception. Giving up.");

  } catch (...) {
    std::cout << "Failed to interpret JAD::UniversalContainer exception" << std::endl;
    throw;
  }

  return std::runtime_error(buf.str());
}

JAD::UCContract getContract(const std::string contract);
JAD::UCContract getContract(const std::string contract) {
  try {
    JAD::Buffer buffer(const_cast<char*>(contract.c_str()));
    JAD::UniversalContainer cspec = JAD::uc_decode_json(&buffer);

    return JAD::UCContract(cspec);
  } catch (JAD::UniversalContainer& ex) {
    ex["source"] = "getContract";
    throw toStdException(ex);
  }
}

TEST(JsonSchemaContractTests, testTrivialContract) {
  JAD::UCContract contract = getContract("{\"type\":\"map\"}");

  try {
    std::string json("{}");
    JAD::Buffer buffer(const_cast<char*>(json.c_str()));
    JAD::UniversalContainer container = JAD::uc_decode_json(&buffer);

    contract.compare_and_throw(container);

  } catch (JAD::UniversalContainer& ex) {
    ex["source"] = "testTrivialContract";
    throw toStdException(ex);
  }
}

TEST(JsonSchemaContractTests, testMapWithKeysContractEmptyJson) {
  JAD::UCContract contract = getContract("{\"type\":\"map\", \"optional_members\": {\"key\" : { \"type\" : \"string\"}}}");

  try {
    std::string json("{}"); // Everything is optional
    JAD::Buffer buffer(const_cast<char*>(json.c_str()));
    JAD::UniversalContainer container = JAD::uc_decode_json(&buffer);

    contract.compare_and_throw(container);

  } catch (JAD::UniversalContainer& ex) {
    ex["source"] = "testMapWithKeysContract";
    throw toStdException(ex);
  }
}

TEST(JsonSchemaContractTests, testMapWithKeysContractFilledJson) {
  JAD::UCContract contract = getContract("{\"type\":\"map\", \"optional_members\": {\"key\" : { \"type\" : \"string\"}}}");

  try {
    std::string json("{\"key\" : \"value\"}"); // "key" is an allowed key
    JAD::Buffer buffer(const_cast<char*>(json.c_str()));
    JAD::UniversalContainer container = JAD::uc_decode_json(&buffer);

    contract.compare_and_throw(container);

  } catch (JAD::UniversalContainer& ex) {
    ex["source"] = "testMapWithKeysContractFilledJson";
    throw toStdException(ex);
  }
}

TEST(JsonSchemaContractTests, testMapWithKeysContractInvalidFilledJson) {
  JAD::UCContract contract = getContract("{\"type\":\"map\", \"optional_members\": {\"key\" : { \"type\" : \"string\"}}}");

  try {
    std::string json("{\"key1\" : \"value\"}"); // "key1" is not an allowed key
    JAD::Buffer buffer(const_cast<char*>(json.c_str()));
    JAD::UniversalContainer container = JAD::uc_decode_json(&buffer);

    try {
      contract.compare_and_throw(container);

      FAIL() << "Expected exception not thrown";

    } catch (JAD::UniversalContainer& expected) {}

  } catch (JAD::UniversalContainer& ex) {
    ex["source"] = "testMapWithKeysContractFilledJson";
    throw toStdException(ex);
  }
}

TEST(JsonSchemaContractTests, testMapWithKeysContractSomeFilledJson) {
  // This contract specifies a map of 3 keys, "key", "key1" and "key2", all of which are optional.
  JAD::UCContract contract = getContract("{\"type\":\"map\", \"optional_members\": {"
      "\"key\" : { \"type\" : \"string\"}, \"key1\" : { \"type\" : \"string\"}, \"key2\" : { \"type\" : \"string\"}}}");

  try {
    std::string json("{\"key1\" : \"value\"}");
    JAD::Buffer buffer(const_cast<char*>(json.c_str()));
    JAD::UniversalContainer container = JAD::uc_decode_json(&buffer);

    contract.compare_and_throw(container);

  } catch (JAD::UniversalContainer& ex) {
    ex["source"] = "testMapWithKeysContractFilledJson";
    throw toStdException(ex);
  }
}

TEST(JsonSchemaContractTests, contractWithBlankId) {
  JAD::UCContract contract = getContract("{\"id\": \"\", \"type\" : \"string\"}");

  // Seems we can't have json schema headers...?
//  try {
//    std::string json("{\"id\" : \"an id\"}");
//    JAD::Buffer buffer(const_cast<char*>(json.c_str()));
//    JAD::UniversalContainer container = JAD::uc_decode_json(&buffer);
//
//    contract.compare_and_throw(container);
//
//  } catch (JAD::UniversalContainer& ex) {
//    ex["source"] = "testTrivialContract";
//    throw toStdException(ex);
//  }
}

TEST(JsonSchemaContractTests, contractWithId) {
  JAD::UCContract contract = getContract("{\"id\": \"http://company.eu/entry-schema#\", \"type\" : \"string\"}");
}

TEST(JsonSchemaContractTests, contractWithIdAndSchema) {
  JAD::UCContract contract = getContract("{\"id\": \"http://company.eu/entry-schema#\","
      "\"$schema\":\"http://json-schema.org/draft-04/schema#\", \"type\" : \"string\"}");
}

TEST(JsonSchemaContractTests, contractWithIdAndSchemaAndDescription) {
  JAD::UCContract contract = getContract("{\"id\": \"http://company.eu/entry-schema#\","
      "\"$schema\":\"http://json-schema.org/draft-04/schema#\","
      "\"description\": \"schema for some json config\", \"type\" : \"string\"}");
}

TEST(JsonSchemaContractTests, contractWithIdAndSchemaAndDescriptionAndTitle) {
  JAD::UCContract contract = getContract("{\"id\": \"http://company.eu/entry-schema#\","
      "\"$schema\":\"http://json-schema.org/draft-04/schema#\","
      "\"description\": \"schema for some json config\","
      "\"title\": \"Some Json Schema\", \"type\" : \"string\"}");
}

TEST(JsonSchemaContractTests, contractWithIdAndSchemaAndDescriptionAndTitleAndCustomString) {
  JAD::UCContract contract = getContract("{\"id\": \"http://company.eu/entry-schema#\","
      "\"$schema\":\"http://json-schema.org/draft-04/schema#\","
      "\"description\": \"schema for some json config\","
      "\"title\": \"Some Json Schema\", \"type\" : \"map\"}");
}

//{
//    "id": "http://fourc.eu/entry-schema#",
//    "$schema": "http://json-schema.org/draft-04/schema#",
//    "description": "schema for fourc-messenger json config",
//  "title": "FourC Messenger Json Config Schema",
//  "type": "map",
//  "optional_members": {
//    "connection.url": {
//      "description": "Broker host name/address",
//      "type": "string"
//    },
//    "connection.options": {
//      "type": "string"
//    },
//    "sender-exchange": {
//      "type": "string"
//    },
//    "application": {
//      "type": "string"
//    },
//    "device": {
//      "type": "string"
//    },
//    "product": {
//      "type": "string"
//    },
//    "system": {
//      "type": "string"
//    }
//  }
//}

