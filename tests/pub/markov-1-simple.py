import os
from common.utils import run, expect_scores


def run_test(sut, verbose, debug):
    this_dir = os.path.dirname(os.path.abspath(__file__))
    args = '-m 1 ../graphs/simple.dot'.split()

    proc, out = run(sut, args, this_dir, 3, verbose, debug)

    scores = {
        'A': 0.081250,
        'B': 0.306250,
        'C': 0.193750,
        'E': 0.418750
    }

    expect_scores(proc, out, scores, 1e-15, verbose, debug)
