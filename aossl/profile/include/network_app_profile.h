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
#include <atomic>

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
  std::atomic<int> last_returned_index{-1};
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
      bool service_found = false;
      while (!service_found) {
        int discovery_index = 0;
        for (auto& itr : doc.GetObject()) {
          std::vector<std::string> current_obj_tags;
          rapidjson::Value::ConstMemberIterator service_itr = \
              itr.value.FindMember("Service");
          if (service_itr != itr.value.MemberEnd()) {
            if (!(service_itr->value.IsNull())) {
              std::string service_name(service_itr->value.GetString());
              if ((service_name == service_identifier) \
                  && (discovery_index > last_returned_index)) {
                last_returned_index = discovery_index;
                service_found = true;
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
                return_service->set_port(std::to_string(port_itr->value.GetInt()));
                return_service->set_id(id_itr->value.GetString());
                return_service->set_name(service_identifier);
              }
            }
          }
          discovery_index++;
        }
        if (!service_found) last_returned_index = -1;
      }
    }
    if (services_buf) delete services_buf;
    return return_service;
  }
};

}  // namespace AOSSL

#endif  // AOSSL_PROFILE_INCLUDE_NETWORK_APP_PROFILE_H_
