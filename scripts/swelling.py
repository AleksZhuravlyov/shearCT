from scripts import swelling_schema

params = dict()

params["imageAFileName"] = '/Users/bigelk/tmp/microct/zak/CO2Z0al_01.nc'
params["imageBFileName"] = '/Users/bigelk/tmp/microct/zak/CO2Z0al_10.nc'
params["valueName"] = 'array';

params["shiftZ"] = float(0.001186)
params["initZ"] = float(0.0008895)
params["xCenter"] = float(0.00360544)
params["yCenter"] = float(0.00364102)
params["xWidth"] = float(0.0047)
params["yWidth"] = float(0.0047)
params["nX"] = int(300)
params["nY"] = int(300)

params["baseConstraintsMin"] = [-30e-6, -30e-6, -30e-6, -0.0785, -0.0785, -0.0785, 0.99]
params["baseConstraintsMax"] = [30e-6, 30e-6, 30e-6, 0.0785, 0.0785, 0.0785, 1.01]

params["topConstraintsMin"] = [-30e-6, -30e-6, -30e-6, -0.2094, -0.2094, -0.2094, 0.99]
params["topConstraintsMax"] = [30e-6, 30e-6, 30e-6, 0.2094, 0.2094, 0.2094, 1.01]

answer = swelling_schema(params)

print(answer)
