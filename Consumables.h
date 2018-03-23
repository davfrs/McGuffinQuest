#ifndef CONSUMABLES
#define CONSUMABLES

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "Inventory/Inventory.h"

const std::string TORCH_NAME("torch");
const std::string POTION_NAME("potion");
const std::string MAXPOTION_NAME("maxpotion");

const std::string TORCH_DESC("Exposes nearby tiles");
const int TORCH_WORTH(50);

const int POTION_HEAL = 10;
const std::string POTION_DESC("Heals up to 10 HP");
const int POTION_WORTH(50);

const std::string MAXPOTION_DESC("Fully heals you");
const int MAXPOTION_WORTH(500);

#endif