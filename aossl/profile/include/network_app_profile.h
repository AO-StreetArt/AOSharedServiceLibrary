/*
MIT License Block

Copyright (c) 2016 Alex Barry

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "aossl/consul/include/consul_interface.h"
#include "aossl/consul/include/factory_consul.h"
#include "aossl/profile/include/tiered_app_profile.h"

#ifndef AOSSL_PROFILE_INCLUDE_NETWORK_APP_PROFILE_H_
#define AOSSL_PROFILE_INCLUDE_NETWORK_APP_PROFILE_H_

namespace AOSSL {

//! Extends the TieredApplicationProfile, adding service discovery logic.

//! Use is the same as a TieredApplicationProfile.
//! Methods are added for finding particular services.
class NetworkApplicationProfile: public TieredApplicationProfile {
  ConsulComponentFactory consul_factory;
  static const int DISCOVERY_LEVEL_UNTAGGED = 0;
  static const int DISCOVERY_LEVEL_SECONDARY = 1;
  static const int DISCOVERY_LEVEL_PRIMARY = 2;
 public:
  NetworkApplicationProfile(int argc, char* argv[]) : \
    TieredApplicationProfile(argc, argv) {}

  NetworkApplicationProfile(int argc, char* argv[], std::string app_name, \
    std::string prof_name) : \
      TieredApplicationProfile(argc, argv, app_name, prof_name) {}

  explicit NetworkApplicationProfile(const std::vector<std::string>& args) : \
    TieredApplicationProfile(args) {}

  NetworkApplicationProfile(const std::vector<std::string>& args, \
    std::string app_name, std::string prof_name) : \
      TieredApplicationProfile(args, app_name, prof_name) {}

  NetworkApplicationProfile(std::string app_name, std::string prof_name) : \
    TieredApplicationProfile(app_name, prof_name) {}
  ~NetworkApplicationProfile() {}

  inline ServiceInterface* get_service(std::string service_identifier) {
    ServiceInterface* return_service = consul_factory.get_service_interface();
    AOSSL::StringBuffer *services_buf = \
        ApplicationProfile::get_consul()->services();

    // Parse the Services List that we get back from the agent
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseStopWhenDoneFlag>(services_buf->val.c_str());
    if (doc.HasParseError()) {
      if (services_buf) delete services_buf;
      throw std::invalid_argument(GetParseError_En(doc.GetParseError()));
    } else if (doc.IsObject()) {
      // We now have a parsed JSON Object which contains
      // a list of known services to our local Consul Agent
      // We need to pull a service instance out of this.  We have 3 levels:
      //  1. A Service with the tag 'Primary'
      //  2. A Service with the tag 'Secondary'
      //  3. Any Neo4j Service
      // the last_discovery_lvl and discovery_lvl let us try to
      // find all 3 layers in one iteration of the loop.  We also
      // break out of the loop as soon as a primary node is found.
      bool service_found = false;
      int last_discovery_lvl = -1;
      int discovery_lvl = -1;
      for (auto& itr : doc.GetObject()) {
        std::vector<std::string> current_obj_tags;
        if (service_found) break;
        rapidjson::Value::ConstMemberIterator service_itr = \
            itr.value.FindMember("Service");
        if (service_itr != itr.value.MemberEnd()) {
          if (!(service_itr->value.IsNull())) {
            std::string service_name(service_itr->value.GetString());
            if (service_name == service_identifier) {
              //
              if (discovery_lvl < DISCOVERY_LEVEL_UNTAGGED) {
                discovery_lvl = DISCOVERY_LEVEL_UNTAGGED;
              }
              // We have identified a Neo4j node
              rapidjson::Value::ConstMemberIterator tags_itr = \
                  itr.value.FindMember("Tags");
              if (tags_itr != itr.value.MemberEnd()) {
                if (!(tags_itr->value.IsNull()) && tags_itr->value.IsArray()) {
                  for (auto& tag_array_itr : tags_itr->value.GetArray()) {
                    if (service_found) break;
                    std::string tag(tag_array_itr.GetString());
                    current_obj_tags.push_back(tag);
                    // First, we should look for a primary node
                    if (tag == "Primary") {
                      last_discovery_lvl = discovery_lvl;
                      discovery_lvl = DISCOVERY_LEVEL_PRIMARY;
                    // Second, we should fallback to a secondary node
                    } else if (tag == "Secondary") {
                      if (discovery_lvl < DISCOVERY_LEVEL_SECONDARY) {
                        last_discovery_lvl = discovery_lvl;
                        discovery_lvl = DISCOVERY_LEVEL_SECONDARY;
                      }
                    }
                  }
                }
              }
              if (discovery_lvl > last_discovery_lvl) {
                // Start by clearing out the return scene
                return_service->clear_tags();
                for (auto& tag : current_obj_tags) {
                  return_service->add_tag(tag);
                }
                rapidjson::Value::ConstMemberIterator address_itr = \
                    itr.value.FindMember("Address");
                rapidjson::Value::ConstMemberIterator port_itr = \
                    itr.value.FindMember("Port");
                rapidjson::Value::ConstMemberIterator id_itr = \
                    itr.value.FindMember("ID");
                return_service->set_address(address_itr->value.GetString());
                return_service->set_port(port_itr->value.GetString());
                return_service->set_id(id_itr->value.GetString());
              }
              if (discovery_lvl == DISCOVERY_LEVEL_PRIMARY) {
                service_found = true;
              }
            }
          }
        }
      }
    }
    if (services_buf) delete services_buf;
    return return_service;
  }
};

}  // namespace AOSSL

#endif  // AOSSL_PROFILE_INCLUDE_NETWORK_APP_PROFILE_H_
