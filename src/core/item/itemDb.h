#pragma once
#include <array>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <variant>

#include "../../random.h"
#include "item.h"
#include "potion.h"
#include "ring.h"

std::unique_ptr<Item> createItem(int id);

std::unique_ptr<Item> createRndItem();