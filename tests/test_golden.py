from flatten_json import flatten, inflate

_golden_test = {
    '0': [1, True, None, 2.1234, 0.1, {'a': 1}],
    '%T3§"\\\\\\..$\'...': None,
    'Some Crazy Unicode': '������������������ࠀ𐀀������������߿',
    'foo': {
        'bar.': {
            'test': 'a',
            'lorem': {
                'ipsum': True,
                'kaboom': {
                    'f': 1,
                }
            }
        },
        'a': 1,
    }

}


def test_golden():
    flattened = flatten(_golden_test)

    assert flattened == {
        '0.[0]': 1,
        '0.[1]': True,
        '0.[2]': None,
        '0.[3]': 2.1234,
        '0.[4]': 0.1,
        '0.[5].a': 1,
        '%T3§"\\\\\\\\\\\\\\.\\.$\'\\.\\.\\.': None,
        'Some Crazy Unicode': '������������������ࠀ𐀀������������߿',
        'foo.bar\\..test': 'a',
        'foo.bar\\..lorem.ipsum': True,
        'foo.bar\\..lorem.kaboom.f': 1,
        'foo.a': 1
    }

    inflated = inflate(flattened)

    assert inflated == _golden_test
