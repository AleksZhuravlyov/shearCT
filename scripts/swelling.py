import sys
import os
import json

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

answers = dict()

for file in argv[2:12]:
    params['imageBFileName'] = file
    answers[file[0:-3]] = swelling_schema(params)
    print()
    print(answers[file[0:-3]])
    print()


with open('answers.json', 'w') as f:
    json.dump(answers, f, sort_keys=True, indent=4 * ' ')

print(answers)
