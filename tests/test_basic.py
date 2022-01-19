import colmapeasy as ce


def test_main():
    assert ce.__version__ == "0.0.1"
    assert ce.add(1, 2) == 3
    assert ce.subtract(1, 2) == -1
