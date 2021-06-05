# MiniCSVReader

>If you need to use csvreader, just include this file in your project.

```cpp
#include"CSVReader.hpp"
```

The interface is as follows:

|Interface||
|--|--|
|Save|Savefile|
|operator[rod_index]|Return a map representing a row of data according to the corresponding row index|
|operator[Col name]|Return the corresponding column according to the name of the corresponding column|
|AppendRow|Add a new line at the end|
|AppendCol|Add a new column|


**This file reading is not very fast, at least for large csv (for example, 2GB or more)**

**IIf you have a new requirement, please let me add**
