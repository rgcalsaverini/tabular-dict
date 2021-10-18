import time
from flatten_json import flatten, escape_type
import collections


def py_flatten(d, parent_key='', sep='.'):
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
                    items.extend(py_flatten(a, item_k, sep=sep).items())
        elif isinstance(v, collections.abc.MutableMapping):
            items.extend(py_flatten(v, new_key, sep=sep).items())
        else:
            items.append((new_key, v))
    return dict(items)


some_dict = {
    'some weir´d châŕäcter$': {
        'te.st': [1],
        'a': {'b': {'c': {'d': {'e': {'f': {'g': 'h'}}}}}}
    },
}


def test_timing():
    start = time.time()
    count = 0
    while time.time() - start < 0.5:
        count += 1
        py_flatten(some_dict, sep='.')
    json_calls_per_sec = count / (time.time() - start)
    start = time.time()
    count = 0
    while time.time() - start < 0.5:
        count += 1
        flatten(some_dict, escape=escape_type.SLASH)
    flatten_calls_per_sec = count / (time.time() - start)
    ratio = flatten_calls_per_sec / json_calls_per_sec
    assert ratio > 1.5
