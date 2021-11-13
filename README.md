python binding for pandax bamboo-shoot3 files
===============================

## How to compile
Please make sure you have `pybind11` and `bamboo-shoot3` installed.

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## How to use it
```python
import ppbs
group = ppbs.GroupReader('group_run02337_file00003.bsd')
data = group.getNext()
```
