# Flatten JSON

A Python3 library to flatten and inflate lists and dictionaries:

For example, flattening something like this:

```python
from flatten_json import flatten

flatten({
    'some_key': {
        'foo': 'bar',
    },
    'WithA.Dot': [True, None],
})
```

will render you a flat tabular structure like this:

```python
{
    'some_key.foo': 'bar',
    'WithA\\.Dot.[0]': True,
    'WithA\\.Dot.[1]': None
}
```

which can be inflated to the original without loss like this:

```python
from flatten_json import inflate

inflate({
    'some_key.foo': 'bar',
    'WithA\\.Dot.[0]': True,
    'WithA\\.Dot.[1]': None
})
```

## How to install

## Documentation

### flatten(data, separator = '.', escape = escape_type.SLASH, mark_arrays = True)

Flattens a dict or list into a single-dimension dict, with a path value mapping. Paths are joined consecutive keys bound
by a pre-defined separator. Arrays are specially marked with square brackets.

Only valid JSON dictionaries and lists are accepted (for example, dict keys must always be strings)

#### Arguments

- **data: Union[dict, list]** - Dictionary or list to be flattened
- **separator: str = '.'** - Separator to be added between different levels on the data, defaults to a dot
- **separator: escape_type = escape_type.SLASH** - Type of escaping, defaults to `SLASH`. Currently only `SLASH`
  and `NO_ESCAPE` are supported
- **mark_arrays: bool = True** - If set to True, will mark array index keys with a bracket.

#### Return value

A dict with the flattened structure.

#### Examples

```python
from flatten_json import flatten

flatten({
    'a': {'b': {'c': {'d': 'e'}}},
    'b': [[[[{'foo': 'bar'}]]]],
})
```

Output:

```python
{
    'a.b.c.d': 'e',
    'b.[0].[0].[0].[0].foo': 'bar',
}
```

### inflate(data, separator = '.', escape = escape_type.SLASH, mark_arrays = True)

Only valid JSON dictionaries and lists are accepted (for example, dict keys must always be strings)

#### Arguments

- **flat_data: dict** - Flat dictionary, with keys as strings and values as primitives (int, str, None, bool, float)
- **separator: str = '.'** - Separator that delimits different levels on the data, defaults to a dot
- **separator: escape_type = escape_type.SLASH** - Type of escaping, defaults to `SLASH`. Currently only `SLASH`
  and `NO_ESCAPE` are supported
- **mark_arrays: bool = True** - If set to True, will recover array keys when enclosed in brackets.

#### Return value

A dict or list with the inflated data

#### Examples

```python
from flatten_json import inflate

inflate({
    'a.b.c.d': 'e',
    'b.[0].[0].[0].[0].foo': 'bar',
})

# Also works with roots that are lists
inflate({
    '[0].a': 1,
    '[1]': 2,
    '[2].a.b.c.d.e.f.g': 'h',
})

```

Output:

```python
{
    'a': {'b': {'c': {'d': 'e'}}},
    'b': [[[[{'foo': 'bar'}]]]],
}

# Also works with roots that are lists
[
    {'a': 1},
    2,
    {'a': {'b': {'c': {'d': {'e': {'f': {'g': 'h'}}}}}}},
]
```

### escape_type

**:warning: For now, avoid using anything other than SLASH.**

There are three accepted escape types:

- SLASH - Will add a slash before the separator and duplicate slashes. This is the default and suggested type
- NO_ESCAPE - Skips escaping altogether
- DOUBLE - Duplicates the separator to escape it (Only partial support for now, avoid this)

Example:

```python
from flatten_json import escape_type, make_path

# Gives you "\\some\-example"
print(make_path('', '\some-example', separator='-', escape=escape_type.SLASH))

# Gives you "\some-example"
print(make_path('', '\some-example', separator='-', escape=escape_type.NO_ESCAPE))

# Gives you "\some--example"
print(make_path('', '\some--example', separator='-', escape=escape_type.DOUBLE))
```

### make_path(path, key, separator = '.', escape = escape_type.SLASH)

This is mostly an internal function, you will probably not use it.

Concatenates a new key into a path, considering the separator and escape rule.

#### Arguments

- **path: str** - Current path, will concatenate new keys into it. Pass empty string if this is the root.
- **key: str** - Separator that delimits different levels on the data, defaults to a dot
- **separator: str = '.'** - Separator that delimits different levels on the data, defaults to a dot
- **separator: escape_type = escape_type.SLASH** - Type of escaping, defaults to `SLASH`.

#### Return value

A string with the concatenated path and key

#### Examples

```python3
from flatten_json import make_path

# Gives you "key"
print(make_path('', 'key'))

# Gives you "path,key"
print(make_path('path', 'key', separator=','))

# Gives you "a,longer,path,key_with_\,_comma"
print(make_path('a,longer,path', 'key_with_,_comma', separator=','))
```

### split_path(path, key, separator = '.', escape = escape_type.SLASH)

This is mostly an internal function, you will probably not use it.

Concatenates a new key into a path, considering the separator and escape rule.

#### Arguments

- **path: str** - Full path generated by make_path. Will split it into individual keys.
- **separator: str = '.'** - Separator that delimits different levels on the data, defaults to a dot
- **separator: escape_type = escape_type.SLASH** - Type of escaping, defaults to `SLASH`.
- **mark_arrays: bool = True** - If set to True, will recover array keys when enclosed in brackets.

#### Return value

A list with the individual keys that make up the path

#### Examples

```python3
from flatten_json import split_path

# Gives you ["1"]
print(split_path('1'))

# Gives you [1]
print(split_path('[1]'))

# Gives you ["one", "tw,o", "three", 4]
print(split_path('one,tw\\,o,three,[4]', separator=','))
```