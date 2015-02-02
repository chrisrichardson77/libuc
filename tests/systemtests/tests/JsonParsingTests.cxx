#include <gmock/gmock.h>

#include <ucontainer.h>
#include <ucio.h>
#include <buffer.h>

TEST(JsonParsingTests, parseSimpleJson) {
  std::string json("{\"key\" : \"value\"}");

  JAD::Buffer buffer(const_cast<char*>(json.c_str()));
  try {
    JAD::uc_decode_json(&buffer);

  } catch (JAD::UniversalContainer& ex) {
    FAIL() << "Exception thrown while parsing json string:"
        << static_cast<std::string>(ex["message"]);
  }
}

TEST(JsonParsingTests, parseMultiValueJson) {
  std::string json("{\"key\" : \"value\", \"key2\" : \"value2\"}");

  JAD::Buffer buffer(const_cast<char*>(json.c_str()));
  try {
    JAD::uc_decode_json(&buffer);

  } catch (JAD::UniversalContainer& ex) {
    FAIL() << "Exception thrown while parsing json string:"
        << static_cast<std::string>(ex["message"]);
  }
}

TEST(JsonParsingTests, parseMultiLineJson) {
  std::string json("{\n\"key\" : \"value\",\n \"key2\" : \"value2\"\n}");

  JAD::Buffer buffer(const_cast<char*>(json.c_str()));
  try {
    JAD::uc_decode_json(&buffer);

  } catch (JAD::UniversalContainer& ex) {
    FAIL() << "Exception thrown while parsing json string:"
        << static_cast<std::string>(ex["message"]);
  }
}

TEST(JsonParsingTests, parseJsonStringValueWithCurlyBraces) {
  std::string json("{\"key\" : \"{value}\"}");

  JAD::Buffer buffer(const_cast<char*>(json.c_str()));
  try {
    JAD::uc_decode_json(&buffer);

  } catch (JAD::UniversalContainer& ex) {
    FAIL() << "Exception thrown while parsing json string:"
        << static_cast<std::string>(ex["message"]);
  }
}

TEST(JsonParsingTests, parseJsonStringValueWithColons) {
  std::string json("{\"key\" : \"{value:value}\"}");

  JAD::Buffer buffer(const_cast<char*>(json.c_str()));
  try {
    JAD::uc_decode_json(&buffer);

  } catch (JAD::UniversalContainer& ex) {
    FAIL() << "Exception thrown while parsing json string:"
        << static_cast<std::string>(ex["message"]);
  }
}

TEST(JsonParsingTests, parseJsonStringValueWithCommas) {
  std::string json("{\"key\" : \"{value,value}\"}");

  JAD::Buffer buffer(const_cast<char*>(json.c_str()));
  try {
    JAD::uc_decode_json(&buffer);

  } catch (JAD::UniversalContainer& ex) {
    FAIL() << "Exception thrown while parsing json string:"
        << static_cast<std::string>(ex["message"]);
  }
}

TEST(JsonParsingTests, parseMultiLineJsonValues) {
  std::string json("{\n\"key\" : \"value\",\n \"key2\" : \"value2\"\n}");

  JAD::Buffer buffer(const_cast<char*>(json.c_str()));
  try {
    JAD::UniversalContainer root = JAD::uc_decode_json(&buffer);

    // We should have a map...
    EXPECT_EQ(JAD::uc_Map, root.get_type());

    // ...with keys "key" and "key2", but not "key3"...
    EXPECT_TRUE(root.exists("key"));
    EXPECT_TRUE(root.exists("key2"));
    EXPECT_FALSE(root.exists("key3")); // Make sure a key that shouldn't exist doesn't.

    // ...and matching values "value" and "value2".
    EXPECT_EQ("value", static_cast<std::string>(root["key"]));
    EXPECT_EQ("value2", static_cast<std::string>(root["key2"]));

    // Non-existent keys return the string "null"
    EXPECT_EQ("null", static_cast<std::string>(root["key3"]));


  } catch (JAD::UniversalContainer& ex) {
    FAIL() << "Exception thrown while parsing json string:"
        << static_cast<std::string>(ex["message"]);
  }
}
