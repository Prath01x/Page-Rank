import os
from common.utils import run, expect_scores


def run_test(sut, verbose, debug):
    this_dir = os.path.dirname(os.path.abspath(__file__))
    args = '-m 1 ../graphs/empty.dot'.split()

    proc, out = run(sut, args, this_dir, 3, verbose, debug)

    expect_scores(proc, out, {}, 0.0, verbose, debug)
