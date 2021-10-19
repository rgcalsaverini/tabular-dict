import time
from flatten_json import flatten, inflate
import collections


def py_flatten(d, parent_key=''):
    sep = '.'
    items = []
    for k, v in d.items():
        if sep in k:
            k = k.replace('.', '\\.')
        new_key = parent_key + sep + k if parent_key else k
        if isinstance(v, list):
            for i, a in enumerate(v):
                item_k = new_key + sep + f'[{i}]'
                if isinstance(a, (int, str, bool, float)):
                    items.append((item_k, a))
                else:
                    items.extend(py_flatten(a, item_k).items())
        elif isinstance(v, collections.abc.MutableMapping):
            items.extend(py_flatten(v, new_key).items())
        else:
            items.append((new_key, v))
    return dict(items)


some_dict = {
    'some weir´d châŕäcter$': {
        'te.st': [1],
        'a': {'b': {'c': {'d': {'e': {'f': {'g': 'h'}}}}}}
    },
}

some_flat_dict = {
    'a.b': 'b',
    'a.c.[0]': 99,
    'a.c.[1]': 98,
    'c': 'c',
    'a.c[2].0.1.2.3.4.5': 1,
}


def test_timing_flatten():
    start = time.time()
    count = 0
    while time.time() - start < 0.5:
        count += 1
        py_flatten(some_dict)
    py_calls_per_sec = count / (time.time() - start)
    start = time.time()
    count = 0
    while time.time() - start < 0.5:
        count += 1
        flatten(some_dict)
    flatten_calls_per_sec = count / (time.time() - start)
    ratio = flatten_calls_per_sec / py_calls_per_sec
    assert ratio > 1.5


def test_timing_inflate():
    start = time.time()
    count = 0
    while time.time() - start < 0.5:
        count += 1
        flatten(some_dict)
    flatten_calls_per_sec = count / (time.time() - start)
    start = time.time()
    count = 0
    while time.time() - start < 0.5:
        count += 1
        inflate(some_flat_dict)
    inflate_calls_per_sec = count / (time.time() - start)
    ratio = inflate_calls_per_sec / flatten_calls_per_sec
    assert ratio > 0.75
