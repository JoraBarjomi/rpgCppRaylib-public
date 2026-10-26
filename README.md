# rpgCppRaylib

Top-down экшн-RPG на C++ и [raylib](https://www.raylib.com/): бой в реальном времени, враг с простым ИИ, уровни, инвентарь и интерфейс.

![Gameplay](gamePlay.gif)

Учебный проект. Начинался как консольная игра, затем получил графику на raylib. Логика игры (`core`) отделена от отрисовки (`render`).

## Возможности

- Бой в реальном времени: удар по клику мыши, попадание засчитывается на нужном кадре анимации, хитбоксы и зона удара.
- Враги (скелет) преследуют героя и атакуют, получают урон и умирают.
- Герой со слоями экипировки: волосы, борода, броня, перчатки, сапоги и оружие рисуются отдельными слоями с общей анимацией (стойка, бег, атака, получение урона, смерть).
- Опыт и уровни, окно распределения очков характеристик (сила, ловкость).
- Инвентарь с предметами (зелья, аксессуары), стаками и использованием предметов.
- Интерфейс: полоска здоровья, окно персонажа, окно инвентаря, окно прокачки, лог событий на экране.
- Система сигналов: здоровье, смерть, уровень и опыт отправляют события, на которые подписан лог.
- Камера с зумом колесом мыши.

## Управление

| Клавиша                     | Действие                              |
| --------------------------- | ------------------------------------- |
| `W` `A` `S` `D` или стрелки | движение                              |
| Левая кнопка мыши           | удар (герой поворачивается к курсору) |
| `I`                         | инвентарь                             |
| `C`                         | окно персонажа                        |
| `Esc`                       | закрыть меню                          |
| Колесо мыши                 | зум камеры                            |
| `Q`                         | выход из игры                         |

## Сборка и запуск

Протестировано на Ubuntu. Нужны:

- `g++` с поддержкой C++23
- `make`

```bash
git clone https://github.com/JoraBarjomi/rpgCppRaylib-public.git
cd rpgCppRaylib-public
cd build && ./premake5 gmake && cd ..
make
./bin/Debug/rpgCppRaylib
```

Запускать нужно из корня проекта.

Сборки под Windows и macOS есть в шаблоне ([raylib-quickstart](https://github.com/raylib-extras/raylib-quickstart)), но я их не проверял.

## Ассеты

Картинки в репозиторий **не входят** (лицензии авторов не разрешают их раздавать). Чтобы игра запустилась, скачай паки по ссылкам из раздела «Авторы ассетов» и положи файлы в папку `resources/` так, как они названы в коде:

```
resources/
├── heroAnims/
│   ├── Idle/      Character_Idle.png, Equipment/Character_Idle_*.png
│   ├── Move/      Character_Move.png, Equipment/Character_Move_*.png
│   ├── Attack/    Character_Attack.png, Equipment/Character_Attack_*.png
│   └── Death/     Character_Death.png
├── Skeleton/      Skeleton_Idle.png, Skeleton_Move.png, ...
├── Items/         Icons_Potions.png
└── ...            тайлы, иконки
```

## Структура кода

```
src/
├── main.cpp            игровой цикл, ввод, камера
├── random.h            рандомизатор
├── core/               игровая логика без привязки к графике
│   ├── hero/           герой: состояния, характеристики, уровни
│   ├── item/           предметы, база предметов, инвентарь
│   ├── skelet/         враг-скелет
│   └── unit/           базовый класс юнита (урон, броня, здоровье)
│
└── render/             всё, что рисует
    ├── heroView/       анимации и слои героя, HUD, окна статов и прокачки
    ├── skeletView/     анимации скелета
    ├── itemsView/      текстуры предметов
    ├── tilesView/      тайлы карты
    ├── logger/         лог событий на экране
    ├── TextureManager  атласы и анимации (нарезка спрайт-листов)
    ├── Observer        паттерн Observer
    ├── SoundManager    загрузка звуков
    └── Ui              отрисовка компонентов ui
```

Идея: `core` ничего не знает про raylib и про картинки, а `render` читает состояние объектов и решает, как его показать.

## Авторы ассетов

Ассеты принадлежат их авторам и не входят в лицензию кода.

- Герой: [Monster Pack Main Character](https://admurin.itch.io/free-monster-pack-character), Admurin
- Скелет: [Monster Pack Skeletons](https://opengameart.org/content/monster-pack-skeletons), Admurin, лицензия CC-BY 4.0
- Тайлы: [Tiny Swords](https://pixelfrog-assets.itch.io/tiny-swords), Pixel Frog
- Зелья: [Tiny Swords](https://admurin.itch.io/mega-admurins-freebies), Pixel Frog

Шаблон проекта: [raylib-quickstart](https://github.com/raylib-extras/raylib-quickstart) (CC0).
