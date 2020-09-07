import sys
import os
import json

from multiprocessing import Pool

from scripts import swelling_schema

argv = sys.argv

params = dict()

params['imageAFileName'] = argv[1]
params['valueName'] = 'array'

params['shiftZ'] = float(0.004)
params['initZ'] = float(0.00033)
params['xCenter'] = float(0.0037)
params['yCenter'] = float(0.0037)
params['xWidth'] = float(0.0035)
params['yWidth'] = float(0.0035)
params['nX'] = int(100)
params['nY'] = int(100)

params['baseAccuracy'] = float(1.e-12)
params['topAccuracy'] = float(1.e-12)

params['baseConstraintsMin'] = [-10e-6, -10e-6, -10e-6, -0.001, -0.001, -0.001, 0.99]
params['baseConstraintsMax'] = [10e-6, 10e-6, 10e-6, 0.001, 0.001, 0.001, 1.01]

params['topConstraintsMin'] = [-10e-6, -10e-6, -10e-6, -0.001, -0.001, -0.001, 0.99]
params['topConstraintsMax'] = [10e-6, 10e-6, 17e-6, 0.003, 0.002, 0.001, 1.01]


def process_parallel(file_name):
    params['imageBFileName'] = file_name
    params['isVerbose'] = False
    answer = {file_name[0:-3]: swelling_schema(params)}
    print(answer, '\n\n')
    return answer


def process_sequential(file_name):
    params['imageBFileName'] = file_name
    params['isVerbose'] = True
    answer = {file_name[0:-3]: swelling_schema(params)}
    print(answer, '\n\n')
    return answer


if __name__ == '__main__':
    files = argv[2:]
    results = list()
    answers = dict()

    # parallel
    pool = Pool()
    results = pool.map(process_parallel, files)

    # sequential
    # for file in files:
    #     answer = process_sequential(file)
    #     results.append(answer)

    for result in results:
        for key in result:
            answers[key] = result[key]

    with open('answers.json', 'w') as f:
        json.dump(answers, f, sort_keys=True, indent=4 * ' ')

    print(answers)
