from argparse import ArgumentParser
from pathlib import Path
from os import listdir
from os.path import isfile, join
from difflib import unified_diff
from dataclasses import dataclass

import subprocess
import termcolor
import math

OK_TAG = f"[{termcolor.colored('OK', 'green')}]"
KO_TAG = f"[{termcolor.colored('KO', 'red')}]"

@dataclass
class TestCase:
    file: str
    stdout: str
    stderr: bool
    exitcode: int

def diff(expected: str, actual: str) -> str:
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)
    return '' .join(unified_diff(expected_lines, actual_lines, fromfile="expected", tofile="actual"))

def run_shell(*cmd_line) -> subprocess.CompletedProcess:
    return subprocess.run(cmd_line, capture_output=True, text=True)

def perform_checks(expected: subprocess.CompletedProcess, actual: subprocess.CompletedProcess):
    assert(not expected.stderr) or actual.stderr != "", \
            "Something was expected on stderr"

    assert expected.returncode == actual.returncode, \
            f"Exited with {actual.returncode} expected {expected.returncode}"

    assert expected.stdout == actual.stdout, \
            f"stdout differ\n{diff(expected.stdout, actual.stdout)}"


nb_tests = 0
test_succeed = 0

if __name__ == '__main__':
    parser = ArgumentParser("TestSuite")
    parser.add_argument("--binary", required=True, type=Path)
    args = parser.parse_args()

    binary_path = args.binary.absolute()
    print(termcolor.colored('------------------------------------------------------RUNNING THE TEST SUITE------------------------------------------------------', 'cyan'))
    test_repo = "simple_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------SIMPLE TEST TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass

    test_repo = "for_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------FOR TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass        
    test_repo = "echo_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------ECHO TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass  
    test_repo = "double_quote_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------DOUBLE QUOTE TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass
    test_repo = "if_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------IF TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass                                    
    test_repo = "pipe_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------PIPE TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass
    test_repo = "redir_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------REDIR TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass
    test_repo = "and_or_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------AND_OR TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass
    test_repo = "while_until_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------WHILE & UNTIL TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass


    test_repo = "variable_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------VARIABLE TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass
 
    test_repo = "negation_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------NEGATION TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass
    test_repo = "cd_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------CD TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass

    test_repo = "break_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------BREAK TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass
    test_repo = "continue_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------CONTINUE TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass
    test_repo = "brackets_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------BRACKETS TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass 
    test_repo = "function_test"
    test_files = [join(test_repo, f) for f in listdir(test_repo) if isfile(join(test_repo, f))]
    print(termcolor.colored('---------FUNCTION TESTING---------', 'magenta'))
    for testcase in test_files:
        file = testcase

        sh_proc = run_shell(binary_path, file)
        sh_ref = run_shell("bash", "--posix", file)
        nb_tests = nb_tests + 1;

        try:
            perform_checks(sh_ref, sh_proc)
        except AssertionError as err:
            print(f"{KO_TAG} {file}\n{err}")
        else:
            print(f"{OK_TAG} {file}")
            test_succeed = test_succeed + 1
            pass                                                                              
def round_up(n, decimals=0):
    multiplier = 10 ** decimals
    return math.ceil(n * multiplier) / multiplier
percentage = round_up(test_succeed / nb_tests * 100,1)                                                                                        
print(f"{test_succeed} test succeed on {nb_tests} tests in total")
if test_succeed > nb_tests/1.4:
    print(termcolor.colored(f"({percentage}%)", 'green'))
if test_succeed < nb_tests/1.4:
    print(termcolor.colored(f"({percentage}%)", 'red'))
