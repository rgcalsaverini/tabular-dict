import pytest
from tabular_dict import make_path, escape_type


def test_first_key():
    path = make_path('', 'key', '', escape_type.NO_ESCAPE)
    assert path == 'key'


def test_simple_merge():
    path = make_path('path', 'key', '.', escape_type.NO_ESCAPE)
    assert path == 'path.key'


def test_escape_double_single_char():
    path = make_path('', '.', '.', escape_type.DOUBLE)
    assert path == '..'


def test_escape_double_start_one():
    path = make_path('', '.a', '.', escape_type.DOUBLE)
    assert path == '..a'


def test_escape_double_start_many():
    path = make_path('', '.test', '.', escape_type.DOUBLE)
    assert path == '..test'


def test_escape_double_end_one():
    path = make_path('', 'a.', '.', escape_type.DOUBLE)
    assert path == 'a..'


def test_escape_double_end_many():
    path = make_path('', 'test.', '.', escape_type.DOUBLE)
    assert path == 'test..'


def test_escape_double_middle_one():
    path = make_path('', 'a.b', '.', escape_type.DOUBLE)
    assert path == 'a..b'


def test_escape_double_middle_many():
    path = make_path('', 'aaaa.bbbb', '.', escape_type.DOUBLE)
    assert path == 'aaaa..bbbb'


def test_escape_double_consecutive():
    path = make_path('', '...', '.', escape_type.DOUBLE)
    assert path == '......'


def test_escape_double_mixed():
    path = make_path('', 'a.b.c.d', '.', escape_type.DOUBLE)
    assert path == 'a..b..c..d'


def test_escape_slash_single_char():
    path = make_path('', '.', '.', escape_type.SLASH)
    assert path == '\\.'


def test_escape_slash_start_one():
    path = make_path('', '.a', '.', escape_type.SLASH)
    assert path == '\\.a'


def test_escape_slash_start_many():
    path = make_path('', '.test', '.', escape_type.SLASH)
    assert path == '\\.test'


def test_escape_slash_end_one():
    path = make_path('', 'a.', '.', escape_type.SLASH)
    assert path == 'a\\.'


def test_escape_slash_end_many():
    path = make_path('', 'test.', '.', escape_type.SLASH)
    assert path == 'test\\.'


def test_escape_slash_middle_one():
    path = make_path('', 'a.b', '.', escape_type.SLASH)
    assert path == 'a\\.b'


def test_escape_slash_middle_many():
    path = make_path('', 'aaaa.bbbb', '.', escape_type.SLASH)
    assert path == 'aaaa\\.bbbb'


def test_escape_slash_consecutive():
    path = make_path('', '...', '.', escape_type.SLASH)
    assert path == '\\.\\.\\.'


def test_escape_slash_mixed():
    path = make_path('', 'a.b.c.d', '.', escape_type.SLASH)
    assert path == 'a\\.b\\.c\\.d'


def test_no_escape():
    path = make_path('', 'a.b.c.d..', '.', escape_type.NO_ESCAPE)
    assert path == 'a.b.c.d..'


def test_double_dont_escape_original_path():
    path = make_path('..a..b', 'key', '.', escape_type.DOUBLE)
    assert path == '..a..b.key'


def test_slash_dont_escape_original_path():
    path = make_path('..a..b', 'key', '.', escape_type.SLASH)
    assert path == '..a..b.key'


def test_double_string_separator():
    path = make_path('', 'abc-#TEST#-def-#TEST#-gh', '-#TEST#-', escape_type.DOUBLE)
    assert path == 'abc-#TEST#--#TEST#-def-#TEST#--#TEST#-gh'


def test_slash_string_separator():
    path = make_path('', 'abc-#TEST#-def-#TEST#-gh', '-#TEST#-', escape_type.SLASH)
    assert path == 'abc\\-#TEST#-def\\-#TEST#-gh'


def test_slash_with_slash_at_end():
    path = make_path('', 'some_key\\', '.', escape_type.SLASH)
    assert path == 'some_key\\\\'
