import pytest
from tabular_dict import split_path, escape_type


def test_split_escaped_dot():
    path = split_path('\\.', '.', escape_type.SLASH, False)
    assert path == ['.']


def test_split_escaped_dot_beginning():
    path = split_path('\\.a', '.', escape_type.SLASH, False)
    assert path == ['.a']


def test_split_escaped_dot_end():
    path = split_path('a\\.', '.', escape_type.SLASH, False)
    assert path == ['a.']


def test_split_two_escaped_dots():
    path = split_path('\\.\\.', '.', escape_type.SLASH, False)
    assert path == ['..']


def test_split_two_fields():
    path = split_path('a.b', '.', escape_type.SLASH, False)
    assert path == ['a', 'b']


def test_split_two_fields_with_escaping():
    path = split_path('a\\..\\.b', '.', escape_type.SLASH, False)
    assert path == ['a.', '.b']


def test_split_two_escapes():
    path = split_path('a\\\\.b', '.', escape_type.SLASH, False)
    assert path == ['a\\', 'b']


def test_split_three_escapes():
    path = split_path('a\\\\\\.b', '.', escape_type.SLASH, False)
    assert path == ['a\\.b']


def test_split_four_escapes():
    path = split_path('a\\\\\\\\.b', '.', escape_type.SLASH, False)
    assert path == ['a\\\\', 'b']


def test_split_single_number():
    path = split_path('[0]', '.', escape_type.SLASH, False)
    assert path == [0]


def test_split_two_numbers():
    path = split_path('[0].[1]', '.', escape_type.SLASH, False)
    assert path == [0, 1]


def test_split_left_bracket_missing():
    path = split_path('0]', '.', escape_type.SLASH, False)
    assert path == ['0]']


def test_split_right_bracket_missing():
    path = split_path('0]', '.', escape_type.SLASH, False)
    assert path == ['0]']


def test_split_non_int_inner_value():
    path = split_path('[12_3]', '.', escape_type.SLASH, False)
    assert path == ['[12_3]']


def test_split_long_string():
    path = split_path('some\\.key.With many.\\\'stepsa.[0].b.c.[1].[0.[_0]', '.', escape_type.SLASH, False)
    assert path == ['some.key', 'With many', "\\'stepsa", 0, 'b', 'c', 1, '[0', '[_0]']
