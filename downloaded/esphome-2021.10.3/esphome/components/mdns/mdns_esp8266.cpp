#if defined(USE_ESP8266) && defined(USE_ARDUINO)

#include "mdns_component.h"
#include "esphome/core/log.h"
#include "esphome/components/network/ip_address.h"
#include "esphome/components/network/util.h"
#include <ESP8266mDNS.h>

namespace esphome {
namespace mdns {

static const char *const TAG = "mdns";

void MDNSComponent::setup() {
  network::IPAddress addr = network::get_ip_address();
  MDNS.begin(compile_hostname_().c_str(), (uint32_t) addr);

  auto services = compile_services_();
  for (const auto &service : services) {
    // Strip the leading underscore from the proto and service_type. While it is
    // part of the wire protocol to have an underscore, and for example ESP-IDF
    // expects the underscore to be there, the ESP8266 implementation always adds
    // the underscore itself.
    auto proto = service.proto.c_str();
    while (*proto == '_') {
      proto++;
    }
    auto service_type = service.service_type.c_str();
    while (*service_type == '_') {
      service_type++;
    }
    MDNS.addService(service_type, proto, service.port);
    for (const auto &record : service.txt_records) {
      MDNS.addServiceTxt(service_type, proto, record.key.c_str(), record.value.c_str());
    }
  }
}

void MDNSComponent::loop() { MDNS.update(); }

}  // namespace mdns
}  // namespace esphome

#endif