python binding for pandax bamboo-shoot3 files
===============================

## How to compile
Please make sure you have and `bamboo-shoot3` installed.

```bash
git submodule init
git submodule update
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## How to use it
```python
import ppbs_px4 as px4
help(px4)

# open a merged physical event file
data = px4.PhysicalEventDataEventIndexReader('event_mpe.bsd')

# read from the start using next
idx, event = data.next()
print(idx.runNumber, idx.fileNumber, idx.eventNumber)

# read signals of this event
for sig in event.signals:
    print(sig.type, sig.startTime)
	

```
