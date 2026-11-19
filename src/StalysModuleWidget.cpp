#include "StalysModuleWidget.hpp"
#include "Skin.hpp"

StalysModuleWidget::StalysModuleWidget() {
  Skins::skins().registerDefaultSkinChangeListener(this);
}

StalysModuleWidget::~StalysModuleWidget() {
  Skins::skins().deregisterDefaultSkinChangeListener(this);
}

static void registerSkinListener(Module *module, Widget *w) {
  if (!module)
    return;
  auto l = dynamic_cast<SkinChangedListener *>(w);
  if (l) {
    auto m = dynamic_cast<StalysModule *>(module);
    assert(m);
    m->addSkinChangedListener(l);
  }
}

void StalysModuleWidget::addParam(ParamWidget *param) {
  ModuleWidget::addParam(param);
  registerSkinListener(module, param);
}

void StalysModuleWidget::addInput(PortWidget *input) {
  ModuleWidget::addInput(input);
  registerSkinListener(module, input);
}

void StalysModuleWidget::addOutput(PortWidget *output) {
  ModuleWidget::addOutput(output);
  registerSkinListener(module, output);
}

void StalysModuleWidget::skinChanged(const std::string &skin) { updatePanel(); }

void StalysModuleWidget::updatePanel() {
  std::string skin = Skins::skins().defaultSkin();

  if (module) {
    auto m = dynamic_cast<StalysModule *>(module);
    assert(m);
    if (m->theme != "" && m->theme != "default") {
      skin = m->theme;
    }
  }

  std::string svg = "res/" + skin + "/" + slug + ".svg";

  SvgPanel *panel = new SvgPanel();
  panel->setBackground(Svg::load(asset::plugin(pluginInstance, svg)));
  // ModuleWidget::setPanel() removes and deletes the previous panel.
  ModuleWidget::setPanel(panel);
  box.size = size;
}

void StalysModuleWidget::setPanel(Vec panelSize, std::string moduleSlug) {
  size = panelSize;
  slug = moduleSlug;
  if (module) {
    auto m = dynamic_cast<StalysModule *>(module);
    assert(m);
    m->addSkinChangedListener(this);
  }
  updatePanel();
}

void StalysModuleWidget::appendContextMenu(Menu *menu) {
  auto m = dynamic_cast<StalysModule *>(module);
  assert(m);
  Skins *skins = &Skins::skins();

  if (skins->available().empty())
    return;

  menu->addChild(new MenuSeparator);
  menu->addChild(createSubmenuItem("Panel", "", [=](Menu *menu) {
    menu->addChild(createCheckMenuItem(
        "Default", "", [=]() { return m->theme == "default"; },
        [=]() { m->setSkin("default"); }));
    for (const Skin &skin : skins->available()) {
      std::string key = skin.key;
      menu->addChild(createCheckMenuItem(
          skin.display, "", [=]() { return m->theme == key; },
          [=]() { m->setSkin(key); }));
    }

    menu->addChild(new MenuSeparator);
    for (const Skin &skin : skins->available()) {
      std::string key = skin.key;
      menu->addChild(createCheckMenuItem(
          "Default to " + skin.display, "",
          [=]() { return skins->defaultSkin() == key; },
          [=]() { skins->setDefaultSkin(key); }));
    }
  }));
}

void StalysModuleWidget::defaultSkinChanged(const std::string &skin) {
  if (module) {
    auto m = dynamic_cast<StalysModule *>(module);
    assert(m);
    if (m->theme == "default") {
      m->setSkin("default");
    }
  } else {
    updatePanel();
  }
}
