Реализовано два кеша:
1. Реализован LFU кеш, который выталкивает страницу с наименьшим числом использований, реализация находится по следующему пути: ```include/lfu.hpp```
2. Реализован идеальный кеш, который при отсутствии свободного места выталкивают ту страницу, которая встретиться позже всех, его реализация находится по следующему пути: ```include/ideal.hpp```

Для сборки проекта:
```
mkdir build
cd build
cmake ..
```

Для сборки бинарника "релизной версии"
```
make release
```

Для сборки бинарника debug версии, где есть сравнение работы LFU с ideal:
```
make debug
```

Для прогонки unit тестов:
```
make tests
```
