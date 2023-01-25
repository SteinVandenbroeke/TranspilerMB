# Transpiler to JS
project MB 2022 a c like language to js

## TODO
- [ ] Cfg for language
- [ ] File to tokens
- [ ] Tokens to table with LR(1)
- [ ] Table to concrete tree
- [ ] tree to ast
- [ ] type error detection
- [ ] Undefined var detection

## cfg
Can be found in
./docs/taal.cfg
./docs/taal.json

## Als crow.h niet werkt omdat:
```
#include <boost/algorithm/string/predicate.hpp>
#include <boost/functional/hash.hpp>
```
niet zijn geinstaleerd
install deze dan met:
```
sudo apt-get install libboost-all-dev
```

## Hoe gebruiken?
-maak een map
```
mkdir build
```
-run cmake in folder
```
cd build
cmake ../.
```
-Build project
```
make
```
-test project in browser
```
./webInterface
```
-test project in cli
```
./transpiler [staal file] [export file]
```


