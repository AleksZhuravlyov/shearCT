import sys
import os
import json

from scripts import swelling_schema

argv = sys.argv

params = dict()

params['imageAFileName'] = argv[1]
params['valueName'] = 'array'

params['shiftZ'] = float(0.001186)
params['initZ'] = float(0.0008895)
params['xCenter'] = float(0.00360544)
params['yCenter'] = float(0.00364102)
params['xWidth'] = float(0.0047)
params['yWidth'] = float(0.0047)
params['nX'] = int(300)
params['nY'] = int(300)

params['baseAccuracy'] = float(1.e-10)
params['topAccuracy'] = float(1.e-9)

params['baseConstraintsMin'] = [-30e-6, -30e-6, -30e-6, -0.0785, -0.0785, -0.0785, 0.99]
params['baseConstraintsMax'] = [30e-6, 30e-6, 30e-6, 0.0785, 0.0785, 0.0785, 1.01]

params['topConstraintsMin'] = [-30e-6, -30e-6, -30e-6, -0.2094, -0.2094, -0.2094, 0.99]
params['topConstraintsMax'] = [30e-6, 30e-6, 30e-6, 0.2094, 0.2094, 0.2094, 1.01]

answers = dict()
for file in argv[1:3]:
    params['imageBFileName'] = file
    answers[file[0:-3]] = swelling_schema(params)


with open('answers.json', 'w') as f:
    json.dump(answers, f, sort_keys=True, indent=4 * ' ')

print(answers)
