#pragma once
#include "Common.hpp"

struct port : SvgPort, SkinChangedListener {
  port() {
    shadow->opacity = 0;
    skinChanged("");
  }

  void skinChanged(const std::string& skin) override;
};
