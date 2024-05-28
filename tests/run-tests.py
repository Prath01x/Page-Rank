#!/usr/bin/env python3
# vim: tabstop=8 expandtab shiftwidth=4 softtabstop=4

if (__name__ == '__main__'):
    import argparse
    import importlib
    import json
    import os
    import re
    import sys
    import timeit
    import traceback
    from common.utils import TestFailure

    cwd = os.path.dirname(os.path.abspath(__file__))

    defaultTestDir = cwd

    parser = argparse.ArgumentParser()
    parser.add_argument('-v', '--verbose',
                        action='store_true',
                        help='be verbose')
    parser.add_argument('-d', '--debug', action='store_true',
                        default=False, help='show test outputs explicitly')
    parser.add_argument('-f', '--filter', type=str, metavar='<regex>',
                        help='only execute tests matching this regex')
    parser.add_argument('-j', '--json', action='store_true',
                        help='output json on stderr')
    parser.add_argument('-l', '--list', action='store_true',
                        help='only list tests, don\'t execute')
    parser.add_argument('-t', '--testdir',
                        type=str, metavar='<dir>', default=defaultTestDir,
                        help='directory containing test definitions '
                             '(default: {})'.format(defaultTestDir))
    parser.add_argument('sut', nargs='?',
                        type=str, metavar='<sut>', help='binary of the system '
                                                        'under test (sut)')

    args = parser.parse_args()

    if not args.list and args.sut is None:
        print('No binary given for testing.')
        exit(1)

    sut = os.path.abspath(args.sut) if args.sut is not None else None
    debug = args.debug
    verbose = args.verbose or debug
    testFilter = None
    if args.filter:
        testFilter = re.compile(args.filter)

    tests_executed = 0
    tests_passed = 0
    tests_failed = 0
    tests_error = 0

    sys.path.append(os.path.abspath(args.testdir))

    if verbose:
        print('Searching for tests in {}:'.format(args.testdir))

    all_tests = []

    for root, dirs, files in os.walk(args.testdir):
        if root.startswith('.') or root.startswith('__') or root == 'common':
            dirs.clear()
            continue
        dirs[:] = (d for d in dirs if not d.startswith('.'))

        for candidate in files:
            if not candidate.endswith('.py') or candidate.startswith('.'):
                continue
            candFile = os.path.join(root, candidate)
            candRelFile = os.path.relpath(candFile, args.testdir)
            candModName = candRelFile.replace(os.sep, '.')[:-3]

            if testFilter and not testFilter.match(candModName):
                if debug:
                    print('filter does not match: {}'.format(candModName))
                continue

            if debug:
                print('importing {}'.format(candModName))
            T = importlib.import_module(candModName)
            if not hasattr(T, 'run_test'):
                continue
            if debug:
                print('-> has run_test function')

            all_tests.append((candModName, T))

    if verbose:
        print('Found {} tests to execute.'.format(len(all_tests)))

    time_before_all = timeit.default_timer()

    all_tests.sort(key=lambda e: e[0])

    test_results = {test[0]: '?' for test in all_tests}

    jsonData = {}

    def add_json(test, result, msg):
        category, name = test.split('.', 1)
        exercise = 'pagerank'
        if not name:
            category, name = 'none', test
        if len(category) == 2 and category[:1] == 'm':
            category, exercise = 'milestone', 'Milestone '+category[1:]
        if category not in jsonData:
            jsonData[category] = {}
        if exercise not in jsonData[category]:
            jsonData[category][exercise] = {}
        jsonData[category][exercise][name] = {'result': result,
                                              'message': msg,
                                              'trace': None}

    if args.list:
        for test, T in all_tests:
            print(test)
        exit(0)

    for test, T in all_tests:
        if verbose:
            print('- {}: '.format(test))
            if debug:
                print('--------------------------------' \
                      '--------------------------------')
        time_before = timeit.default_timer()
        if not (os.path.isfile(sut) and os.access(sut, os.X_OK)):
            text = "Tested binary is not an executable file!"
            add_json(test, 'failure', text)
            print('- {} failed: {}'.format(test, text))
            continue
        try:
            T.run_test(sut, verbose, debug)
            test_results[test] = 'PASS'
            add_json(test, 'successful', 'Test passed')
        except TestFailure as e:
            text = str(e.msg)
            if len(text) > 1000:
                text = text[:990]+' [...]'
            test_results[test] = 'FAIL'
            add_json(test, 'failure', text)
            if verbose:
                print('{}'.format(text))
            else:
                print('- {} failed: {}'.format(test, text))
        except KeyboardInterrupt:
            print('\nKeyboard interrupt. Skipping further tests.')
            break
        except Exception as e:
            test_results[test] = 'ERROR'
            if verbose:
                print('Unexpected error while invoking '
                      'the test:\n{}'.format(e))
                traceback.print_exc()
        if verbose:
            if debug:
                print('--------------------------------'
                      '--------------------------------')
            seconds = timeit.default_timer() - time_before
            print('  ## {} ({:.3f} seconds)'.format(
                test_results[test], seconds))

    seconds_total = timeit.default_timer() - time_before_all
    tests_passed = len([t for t, r in test_results.items() if r == 'PASS'])
    tests_failed = len([t for t, r in test_results.items() if r == 'FAIL'])
    tests_error = len([t for t, r in test_results.items() if r == 'ERROR'])
    print('Results: {} of {} tests passing, {} failing, {} in error ' \
          '(total: {:.3f} seconds)'. \
          format(tests_passed, len(all_tests), tests_failed, tests_error,
                 seconds_total))

    if args.json:
        json.dump(jsonData, sys.stderr, sort_keys=True, indent=2)

    exit(0 if tests_passed == len(all_tests) else -1)
