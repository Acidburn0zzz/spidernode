#!/usr/bin/env python

import subprocess
import os
import sys

def main():
    base_dir = get_base_dir()
    if not os.path.isdir(base_dir):
        print >> sys.stderr, 'Please run this script in the SpiderNode checkout directory.'
        return 1

    ran_test = False
    targets = get_targets(base_dir)
    matrix = [(test, config) for test in targets for config in ['Debug', 'Release']]

    for test, config in matrix:
        cmd = "%s/out/%s/%s" % (base_dir, config, test)
        ran_test = run_test([cmd]) or ran_test

    for node in get_node_binaries(base_dir):
        ran_test = run_test([node, '-e', 'console.log("hello, world")']) or ran_test

    if not ran_test:
        print >> sys.stderr, 'Did not run any tests! Did you forget to build?'
    return 0 if ran_test else 1

def get_base_dir():
    try:
        process = subprocess.Popen("git rev-parse --show-toplevel".split(" "),
                                   stdout=subprocess.PIPE)
        return process.communicate()[0].strip()
    except e:
        return None

def is_executable(f):
    return os.path.isfile(f) and os.access(f, os.X_OK)

def get_targets(base_dir):
    sys.path.append("%s/tools/gyp/pylib" % base_dir)
    import gyp

    path = "deps/spidershim/tests.gyp"
    params = {
      b'parallel': True,
      b'root_targets': None,
    }
    includes = [
      "config.gypi",
      "common.gypi",
    ]
    default_variables = {
      b'OS': 'linux',
      b'STATIC_LIB_PREFIX': '',
      b'STATIC_LIB_SUFFIX': '',
    }

    # Use the gypd generator as very simple generator, since we're only
    # interested in reading the target names.
    generator, flat_list, targets, data = \
        gyp.Load([path], format='gypd', \
                 params=params, includes=includes, \
                 default_variables=default_variables)

    return [target['target_name'] for target in data['deps/spidershim/tests.gyp']['targets']]

def get_node_binaries(base_dir):
    nodes = []
    for name in ['node', 'node_g']:
        path = "%s/%s" % (base_dir, name)
        if is_executable(path):
            nodes.append(path)

    return nodes

def run_test(cmd):
    if is_executable(cmd[0]):
        try:
            subprocess.check_call(cmd)
        except:
            print >> sys.stderr, '%s failed, see the log above' % (" ".join(cmd))
            return False
        return True
    else:
        print >> sys.stderr, 'Skipping %s since it does not exist' % (" ".join(cmd))
    return False

if __name__ == '__main__':
    sys.exit(main())
