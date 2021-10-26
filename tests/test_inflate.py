from tabular_dict import inflate


def test_inflate_minimal():
    res = inflate({'a': 1})
    assert res == {'a': 1}


def test_inflate_depth_1():
    data = {str(v): v for v in range(100)}
    res = inflate(data)
    assert res == data


def test_inflate_depth_2():
    flat = {}
    expected = {}
    for i in range(5):
        expected[str(i)] = {}
        for j in range(5):
            expected[str(i)][str(j)] = i * 5 + j
            flat[f'{i}.{j}'] = i * 5 + j

    res = inflate(flat)
    assert res == expected


def test_inflate_array_root():
    res = inflate({'[0]': 1, '[1]': 2, '[2].[0]': 3, '[3].[0].[0].[0].[0].[0].[0].[0].[0].[0]': 4})
    assert res == [1, 2, [3], [[[[[[[[[4]]]]]]]]]]


def test_inflate_deep():
    depth = 10000
    key = '.'.join([str(i) for i in range(depth)])
    data = {key: 1}
    res = inflate(data)

    for i in range(depth):
        assert list(res.keys()) == [str(i)]
        res = res[str(i)]
    assert res == 1
